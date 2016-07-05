/*==========================================================================
 
 Portions (c) Copyright 2008-2009 Brigham and Women's Hospital (BWH) All Rights Reserved.
 
 See Doc/copyright/copyright.txt
 or http://www.slicer.org/copyright/copyright.txt for details.
 
 Program:   3D Slicer
 Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/OpenIGTLinkIF/vtkSlicerOpenIGTLinkIFLogic.h $
 Date:      $Date: 2010-06-10 11:05:22 -0400 (Thu, 10 Jun 2010) $
 Version:   $Revision: 13728 $
 
 ==========================================================================*/

/// This class manages the logic associated with tracking device for IGT.

#ifndef __vtkSlicerOpenIGTLinkIOIFLogic_h
#define __vtkSlicerOpenIGTLinkIOIFLogic_h


#include "vtkSlicerOpenIGTLinkIOIFModuleLogicExport.h"

// OpenIGTLink includes
#include <igtlImageMessage.h>
#include <igtlTransformMessage.h>

#include "vtkSlicerBaseLogic.h"
#include "vtkSlicerModuleLogic.h"
#include "vtkCallbackCommand.h"

// MRML includes
#include <vtkMRMLTransformNode.h>
#include <vtkMRMLFiducialListNode.h>

// VTK includes
#include <vtkMultiThreader.h>

#include "vtkIGTLIODevice.h"

typedef vtkSmartPointer<class vtkIGTLIOLogic> vtkIGTLIOLogicPointer;
typedef vtkSmartPointer<class vtkIGTLIOConnectorWrapper> vtkIGTLIOConnectorWrapperPointer;
typedef vtkSmartPointer<class vtkIGTLIODevice> vtkIGTLIODevicePointer;
typedef vtkSmartPointer<class vtkIGTLIOLogic> vtkIGTLIOLogicPointer;

// STD includes
#include <vector>

//class vtkMRMLIGTLConnectorNode;

/// \ingroup Slicer_QtModules_OpenIGTLinkIF
class VTK_SLICER_OPENIGTLINKIOIF_MODULE_LOGIC_EXPORT vtkSlicerOpenIGTLinkIOIFLogic : public vtkSlicerModuleLogic
{
public:
  
  enum {  // Events
    StatusUpdateEvent       = 50001,
    //SliceUpdateEvent        = 50002,
  };
  
  typedef struct {
    std::string name;
    std::string type;
    int io;
    std::string nodeID;
  } IGTLMrmlNodeInfoType;
  
  typedef std::vector<IGTLMrmlNodeInfoType>         IGTLMrmlNodeListType;
  
  // Work phase keywords used in NaviTrack (defined in BRPTPRInterface.h)
  
public:
  
  static vtkSlicerOpenIGTLinkIOIFLogic *New();
  vtkTypeMacro(vtkSlicerOpenIGTLinkIOIFLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream&, vtkIndent);
  
  /// The selected transform node is observed for TransformModified events and the transform
  /// data is copied to the slice nodes depending on the current mode
  
  virtual void SetMRMLSceneInternal(vtkMRMLScene * newScene);
  
  virtual void RegisterNodes();
  
  //----------------------------------------------------------------
  // Events
  //----------------------------------------------------------------
  
  virtual void OnMRMLSceneEndImport();
  
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* /*node*/);
  
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* /*node*/);
  
  virtual void OnMRMLNodeModified(vtkMRMLNode* /*node*/){}
  
  enum {
    //TODO: harmonize event handling for Logic, Connector, Device.
    ConnectionAddedEvent        = 118960,
    ConnectionAboutToBeRemovedEvent      = 118961,
    
    NewDeviceEvent        = 118949,
    //    DeviceModifiedEvent   = 118950, // must listen to each specific device in order to get this one.
    RemovedDeviceEvent    = 118951,
    CommandQueryReceivedEvent = vtkIGTLIODevice::CommandQueryReceivedEvent, // one of the connected COMMAND devices got a query
    CommandResponseReceivedEvent = vtkIGTLIODevice::CommandResponseReceivedEvent // one of the connected COMMAND devices got a response
  };

  
  //----------------------------------------------------------------
  // Connector and converter Management
  //----------------------------------------------------------------
  
  // Access connectors
  //vtkMRMLIGTLConnectorNode* GetConnector(const char* conID);
  vtkIGTLIOConnectorWrapperPointer CreateConnector();
  int RemoveConnector(int index);
  int GetNumberOfConnectors() const;
  vtkIGTLIOConnectorWrapperPointer GetConnector(int index);
  
  // Call timer-driven routines for each connector
  void                      CallConnectorTimerHander();
  
  //TODO: interface for accessing Devices
  int GetNumberOfDevices() const;
  void RemoveDevice(int index);
  vtkIGTLIODevicePointer GetDevice(int index);
  
  //----------------------------------------------------------------
  // MRML Management
  //----------------------------------------------------------------
  
  virtual void ProcessMRMLNodesEvents(vtkObject* caller, unsigned long event, void * callData);
  //virtual void ProcessLogicEvents(vtkObject * caller, unsigned long event, void * callData);
  
  void ProcCommand(const char* nodeName, int size, unsigned char* data);
  
  void GetDeviceNamesFromMrml(IGTLMrmlNodeListType &list);
  void GetDeviceNamesFromMrml(IGTLMrmlNodeListType &list, const char* mrmlTagName);
  //void GetDeviceTypes(std::vector<char*> &list);
  
protected:
  
  //----------------------------------------------------------------
  // Constructor, destructor etc.
  //----------------------------------------------------------------
  
  vtkSlicerOpenIGTLinkIOIFLogic();
  virtual ~vtkSlicerOpenIGTLinkIOIFLogic();
  
  static void DataCallback(vtkObject*, unsigned long, void *, void *);
  
  void AddMRMLConnectorNodeObserver(vtkIGTLIOConnectorWrapper * connectorNode);
  void RemoveMRMLConnectorNodeObserver(vtkIGTLIOConnectorWrapper * connectorNode);
  
  void RegisterMessageConverters(vtkIGTLIOConnectorWrapper * connectorNode);
  
  void UpdateAll();
  void UpdateSliceDisplay();
  vtkCallbackCommand *DataCallbackCommand;
  
private:
  
  int Initialized;
  
  //----------------------------------------------------------------
  // Connector Management
  //----------------------------------------------------------------
  
  std::vector<vtkIGTLIODevicePointer> CreateDeviceList() const;
  
  //int LastConnectorID;
  int RestrictDeviceName;
  
private:
  
  vtkSlicerOpenIGTLinkIOIFLogic(const vtkSlicerOpenIGTLinkIOIFLogic&); // Not implemented
  void operator=(const vtkSlicerOpenIGTLinkIOIFLogic&);               // Not implemented
};

#endif
