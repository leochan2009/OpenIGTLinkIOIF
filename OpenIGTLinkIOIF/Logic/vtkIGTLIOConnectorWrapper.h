/*=auto=========================================================================

  Portions (c) Copyright 2009 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkMRMLCurveAnalysisNode.h,v $
  Date:      $Date: 2006/03/19 17:12:29 $
  Version:   $Revision: 1.3 $

=========================================================================auto=*/
#ifndef __vtkIGTLIOConnectorWrapper_h
#define __vtkIGTLIOConnectorWrapper_h

// OpenIGTLinkIF MRML includes

#include "vtkSlicerOpenIGTLinkIOIFModuleLogicExport.h"


// MRML includes
#include <vtkMRML.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLStorageNode.h>

// VTK includes
#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkWeakPointer.h>
#include <vtkIGTLIOConnector.h>

// STD includes
#include <string>
#include <map>
#include <vector>
#include <set>

class vtkMultiThreader;
class vtkMutexLock;
class vtkIGTLCircularBuffer;

class VTK_SLICER_OPENIGTLINKIOIF_MODULE_LOGIC_EXPORT vtkIGTLIOConnectorWrapper : public vtkMRMLNode, public vtkIGTLIOConnector
{

 public:

  //----------------------------------------------------------------
  // Standard methods for MRML nodes
  //----------------------------------------------------------------

  static vtkIGTLIOConnectorWrapper *New();
  //vtkTypeMacro(vtkIGTLIOConnectorWrapper,vtkMRMLNode);

  void PrintSelf(ostream& os, vtkIndent indent);

  virtual vtkMRMLNode* CreateNodeInstance();

  // Description:
  // Set node attributes
  virtual void ReadXMLAttributes( const char** atts);

  // Description:
  // Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  // Description:
  // Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node);

  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName()
    {return "IGTLConnector";};

  // method to propagate events generated in mrml
  virtual void ProcessMRMLEvents ( vtkObject *caller, unsigned long event, void *callData );
  
 protected:
  //----------------------------------------------------------------
  // Constructor and destroctor
  //----------------------------------------------------------------

  vtkIGTLIOConnectorWrapper();
  ~vtkIGTLIOConnectorWrapper();
  vtkIGTLIOConnectorWrapper(const vtkIGTLIOConnectorWrapper&);
  void operator=(const vtkIGTLIOConnectorWrapper&);


};

#endif

