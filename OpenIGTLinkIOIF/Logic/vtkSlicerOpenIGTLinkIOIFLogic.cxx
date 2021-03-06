/*==========================================================================
 
 Portions (c) Copyright 2008-2009 Brigham and Women's Hospital (BWH) All Rights Reserved.
 
 See Doc/copyright/copyright.txt
 or http://www.slicer.org/copyright/copyright.txt for details.
 
 Program:   3D Slicer
 Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/OpenIGTLinkIF/vtkSlicerOpenIGTLinkIOIFLogic.cxx $
 Date:      $Date: 2011-06-12 14:55:20 -0400 (Sun, 12 Jun 2011) $
 Version:   $Revision: 16985 $
 
 ==========================================================================*/

// Slicer includes
#include <vtkObjectFactory.h>
#include <vtkSlicerColorLogic.h>
#include <vtkMRMLScene.h>

// OpenIGTLinkIF MRML includes
#include "vtkIGTLIOCircularBuffer.h"
#include "vtkIGTLIOConnectorWrapper.h"


// OpenIGTLinkIF Logic includes
#include "vtkSlicerOpenIGTLinkIOIFLogic.h"

// VTK includes
#include <vtkNew.h>
#include <vtkCallbackCommand.h>
#include <vtkImageData.h>
#include <vtkTransform.h>

//---------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerOpenIGTLinkIOIFLogic);

//---------------------------------------------------------------------------
vtkSlicerOpenIGTLinkIOIFLogic::vtkSlicerOpenIGTLinkIOIFLogic()
{
  
  // Timer Handling
  this->DataCallbackCommand = vtkCallbackCommand::New();
  this->DataCallbackCommand->SetClientData( reinterpret_cast<void *> (this) );
  this->DataCallbackCommand->SetCallback(vtkSlicerOpenIGTLinkIOIFLogic::DataCallback);
  
  this->Initialized   = 0;
  this->RestrictDeviceName = 0;
  
  
  //this->LocatorTransformNode = NULL;
}

//---------------------------------------------------------------------------
vtkSlicerOpenIGTLinkIOIFLogic::~vtkSlicerOpenIGTLinkIOIFLogic()
{
  
  
  if (this->DataCallbackCommand)
  {
    this->DataCallbackCommand->Delete();
  }
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkObject::PrintSelf(os, indent);
  
  os << indent << "vtkSlicerOpenIGTLinkIOIFLogic:             " << this->GetClassName() << "\n";
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> sceneEvents;
  sceneEvents->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  sceneEvents->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  sceneEvents->InsertNextValue(vtkMRMLScene::EndImportEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, sceneEvents.GetPointer());
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::RegisterNodes()
{
  vtkMRMLScene * scene = this->GetMRMLScene();
  if(!scene)
  {
    return;
  }
  
  scene->RegisterNodeClass(vtkNew<vtkIGTLIOConnectorWrapper>().GetPointer());
  
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::DataCallback(vtkObject *vtkNotUsed(caller),
                                               unsigned long vtkNotUsed(eid), void *clientData, void *vtkNotUsed(callData))
{
  vtkSlicerOpenIGTLinkIOIFLogic *self = reinterpret_cast<vtkSlicerOpenIGTLinkIOIFLogic *>(clientData);
  vtkDebugWithObjectMacro(self, "In vtkSlicerOpenIGTLinkIOIFLogic DataCallback");
  self->UpdateAll();
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::UpdateAll()
{
}

//----------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::AddMRMLConnectorNodeObserver(vtkIGTLIOConnectorWrapper * connectorNode)
{
  if (!connectorNode)
  {
    return;
  }
  // Make sure we don't add duplicate observation
  //vtkUnObserveMRMLNodeMacro(connectorNode);
  // Start observing the connector node
  vtkNew<vtkIntArray> connectorNodeEvents;
  connectorNodeEvents->InsertNextValue(vtkIGTLIOConnectorWrapper::DeviceModifiedEvent);
  vtkObserveMRMLNodeEventsMacro(connectorNode, connectorNodeEvents.GetPointer());
}

//----------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::RemoveMRMLConnectorNodeObserver(vtkIGTLIOConnectorWrapper * connectorNode)
{
  if (!connectorNode)
  {
    return;
  }
  vtkUnObserveMRMLNodeMacro(connectorNode);
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::RegisterMessageConverters(vtkIGTLIOConnectorWrapper * connectorNode)
{
  if (!connectorNode)
  {
    return;
  }
  for (unsigned short i = 0; i < this->GetNumberOfConverters(); i ++)
  {
    connectorNode->RegisterMessageConverter(this->GetConverter(i));
  }
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::OnMRMLSceneEndImport()
{
  // Scene loading/import is finished, so now start the command processing thread
  // of all the active persistent connector nodes
  
  std::vector<vtkMRMLNode*> nodes;
  this->GetMRMLScene()->GetNodesByClass("vtkMRMLIGTLConnectorNode", nodes);
  for (std::vector< vtkMRMLNode* >::iterator iter = nodes.begin(); iter != nodes.end(); ++iter)
  {
    vtkMRMLIGTLConnectorNode* connector = vtkMRMLIGTLConnectorNode::SafeDownCast(*iter);
    if (connector == NULL)
    {
      continue;
    }
    if (connector->GetPersistent() == vtkMRMLIGTLConnectorNode::PERSISTENT_ON)
    {
      this->Modified();
      if (connector->GetState()!=vtkMRMLIGTLConnectorNode::STATE_OFF)
      {
        connector->Start();
      }
    }
  }
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::OnMRMLSceneNodeAdded(vtkMRMLNode* node)
{
  //vtkDebugMacro("vtkSlicerOpenIGTLinkIOIFLogic::OnMRMLSceneNodeAdded");
  
  vtkMRMLIGTLConnectorNode * connectorNode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);
  if (connectorNode)
  {
    // TODO Remove this line when the corresponding UI option will be added
    connectorNode->SetRestrictDeviceName(0);
    
    this->AddMRMLConnectorNodeObserver(connectorNode);
    this->RegisterMessageConverters(connectorNode);
  }
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::OnMRMLSceneNodeRemoved(vtkMRMLNode* node)
{
  vtkMRMLIGTLConnectorNode * connectorNode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);
  if (connectorNode)
  {
    this->RemoveMRMLConnectorNodeObserver(connectorNode);
  }
}

//---------------------------------------------------------------------------
vtkMRMLIGTLConnectorNode* vtkSlicerOpenIGTLinkIOIFLogic::GetConnector(const char* conID)
{
  vtkMRMLNode* node = this->GetMRMLScene()->GetNodeByID(conID);
  if (node)
  {
    vtkMRMLIGTLConnectorNode* conNode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);
    return conNode;
  }
  else
  {
    return NULL;
  }
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::CallConnectorTimerHander()
{
  //ConnectorMapType::iterator cmiter;
  std::vector<vtkMRMLNode*> nodes;
  this->GetMRMLScene()->GetNodesByClass("vtkMRMLIGTLConnectorNode", nodes);
  
  std::vector<vtkMRMLNode*>::iterator iter;
  
  //for (cmiter = this->ConnectorMap.begin(); cmiter != this->ConnectorMap.end(); cmiter ++)
  for (iter = nodes.begin(); iter != nodes.end(); iter ++)
  {
    vtkMRMLIGTLConnectorNode* connector = vtkMRMLIGTLConnectorNode::SafeDownCast(*iter);
    if (connector == NULL)
    {
      continue;
    }
    connector->ImportDataFromCircularBuffer();
    connector->ImportEventsFromEventBuffer();
    connector->PushOutgoingMessages();
  }
}


//---------------------------------------------------------------------------
int vtkSlicerOpenIGTLinkIOIFLogic::SetRestrictDeviceName(int f)
{
  if (f != 0) f = 1; // make sure that f is either 0 or 1.
  this->RestrictDeviceName = f;
  
  std::vector<vtkMRMLNode*> nodes;
  this->GetMRMLScene()->GetNodesByClass("vtkMRMLIGTLConnectorNode", nodes);
  
  std::vector<vtkMRMLNode*>::iterator iter;
  
  for (iter = nodes.begin(); iter != nodes.end(); iter ++)
  {
    vtkMRMLIGTLConnectorNode* connector = vtkMRMLIGTLConnectorNode::SafeDownCast(*iter);
    if (connector)
    {
      connector->SetRestrictDeviceName(f);
    }
  }
  return this->RestrictDeviceName;
}

//---------------------------------------------------------------------------
int vtkSlicerOpenIGTLinkIOIFLogic::RegisterMessageConverter(BaseConverter* converter)
{
  if (converter == NULL)
  {
    return 0;
  }
  
  // Search the list and check if the same converter has already been registered.
  int found = 0;
  
  MessageConverterListType::iterator iter;
  for (iter = this->MessageConverterList.begin();
       iter != this->MessageConverterList.end();
       iter ++)
  {
    if ((converter->GetIGTLName() && strcmp(converter->GetIGTLName(), (*iter)->GetIGTLName()) == 0) &&
        (converter->GetMRMLName() && strcmp(converter->GetMRMLName(), (*iter)->GetMRMLName()) == 0))
    {
      found = 1;
    }
  }
  if (found)
  {
    return 0;
  }
  
  if (converter->GetIGTLName() && converter->GetMRMLName())
    // TODO: is this correct? Shouldn't it be "&&"
  {
    this->MessageConverterList.push_back(converter);
    converter->SetOpenIGTLinkIFLogic(this);
  }
  else
  {
    return 0;
  }
  
  // Add the converter to the existing connectors
  if (this->GetMRMLScene())
  {
    std::vector<vtkMRMLNode*> nodes;
    this->GetMRMLScene()->GetNodesByClass("vtkMRMLIGTLConnectorNode", nodes);
    
    std::vector<vtkMRMLNode*>::iterator niter;
    for (niter = nodes.begin(); niter != nodes.end(); niter ++)
    {
      vtkMRMLIGTLConnectorNode* connector = vtkMRMLIGTLConnectorNode::SafeDownCast(*niter);
      if (connector)
      {
        connector->RegisterMessageConverter(converter);
      }
    }
  }
  
  return 1;
}

//---------------------------------------------------------------------------
int vtkSlicerOpenIGTLinkIOIFLogic::UnregisterMessageConverter(vtkIGTLToMRMLBase* converter)
{
  if (converter == NULL)
  {
    return 0;
  }
  
  // Look up the message converter list
  MessageConverterListType::iterator iter;
  iter = this->MessageConverterList.begin();
  while ((*iter) != converter) iter ++;
  
  if (iter != this->MessageConverterList.end())
    // if the converter is on the list
  {
    this->MessageConverterList.erase(iter);
    // Remove the converter from the existing connectors
    std::vector<vtkMRMLNode*> nodes;
    if (this->GetMRMLScene())
    {
      this->GetMRMLScene()->GetNodesByClass("vtkMRMLIGTLConnectorNode", nodes);
      
      std::vector<vtkMRMLNode*>::iterator iter;
      for (iter = nodes.begin(); iter != nodes.end(); iter ++)
      {
        vtkMRMLIGTLConnectorNode* connector = vtkMRMLIGTLConnectorNode::SafeDownCast(*iter);
        if (connector)
        {
          connector->UnregisterMessageConverter(converter);
        }
      }
    }
    return 1;
  }
  else
  {
    return 0;
  }
}

//---------------------------------------------------------------------------
unsigned int vtkSlicerOpenIGTLinkIOIFLogic::GetNumberOfConverters()
{
  return this->MessageConverterList.size();
}

//---------------------------------------------------------------------------
vtkIGTLToMRMLBase* vtkSlicerOpenIGTLinkIOIFLogic::GetConverter(unsigned int i)
{
  
  if (i < this->MessageConverterList.size())
  {
    return this->MessageConverterList[i];
  }
  else
  {
    return NULL;
  }
}

//---------------------------------------------------------------------------
vtkIGTLToMRMLBase* vtkSlicerOpenIGTLinkIOIFLogic::GetConverterByMRMLTag(const char* mrmlTag)
{
  //Currently, this function cannot find multiple converters
  // that use the same mrmlType (e.g. vtkIGTLToMRMLLinearTransform
  // and vtkIGTLToMRMLPosition). A converter that is found first
  // will be returned.
  
  vtkIGTLToMRMLBase* converter = NULL;
  
  MessageConverterListType::iterator iter;
  for (iter = this->MessageConverterList.begin();
       iter != this->MessageConverterList.end();
       iter ++)
  {
    if (strcmp((*iter)->GetMRMLName(), mrmlTag) == 0)
    {
      converter = *iter;
      break;
    }
  }
  
  return converter;
}

//---------------------------------------------------------------------------
vtkIGTLToMRMLBase* vtkSlicerOpenIGTLinkIOIFLogic::GetConverterByDeviceType(const char* deviceType)
{
  vtkIGTLToMRMLBase* converter = NULL;
  
  MessageConverterListType::iterator iter;
  for (iter = this->MessageConverterList.begin();
       iter != this->MessageConverterList.end();
       iter ++)
  {
    if ((*iter)->GetConverterType() == vtkIGTLToMRMLBase::TYPE_NORMAL)
    {
      if (strcmp((*iter)->GetIGTLName(), deviceType) == 0)
      {
        converter = *iter;
        break;
      }
    }
    else
    {
      int n = (*iter)->GetNumberOfIGTLNames();
      for (int i = 0; i < n; i ++)
      {
        if (strcmp((*iter)->GetIGTLName(i), deviceType) == 0)
        {
          converter = *iter;
          break;
        }
      }
    }
  }
  
  return converter;
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::ProcessMRMLNodesEvents(vtkObject * caller, unsigned long event, void * callData)
{
  
  if (caller != NULL)
  {
    vtkSlicerModuleLogic::ProcessMRMLNodesEvents(caller, event, callData);
    
    vtkMRMLIGTLConnectorNode* cnode = vtkMRMLIGTLConnectorNode::SafeDownCast(caller);
    if (cnode && event == vtkMRMLIGTLConnectorNode::DeviceModifiedEvent)
    {
      // Check visibility
      int nnodes;
      
      // Incoming nodes
      nnodes = cnode->GetNumberOfIncomingMRMLNodes();
      for (int i = 0; i < nnodes; i ++)
      {
        vtkMRMLNode* inode = cnode->GetIncomingMRMLNode(i);
        if (inode)
        {
          vtkIGTLToMRMLBase* converter = GetConverterByMRMLTag(inode->GetNodeTagName());
          if (converter)
          {
            const char * attr = inode->GetAttribute("IGTLVisible");
            if (attr && strcmp(attr, "true") == 0)
            {
              converter->SetVisibility(1, this->GetMRMLScene(), inode);
            }
            else
            {
              converter->SetVisibility(0, this->GetMRMLScene(), inode);
            }
          }
        }
      }
      
      // Outgoing nodes
      nnodes = cnode->GetNumberOfOutgoingMRMLNodes();
      for (int i = 0; i < nnodes; i ++)
      {
        vtkMRMLNode* inode = cnode->GetOutgoingMRMLNode(i);
        if (inode)
        {
          vtkIGTLToMRMLBase* converter = GetConverterByMRMLTag(inode->GetNodeTagName());
          if (converter)
          {
            const char * attr = inode->GetAttribute("IGTLVisible");
            if (attr && strcmp(attr, "true") == 0)
            {
              converter->SetVisibility(1, this->GetMRMLScene(), inode);
            }
            else
            {
              converter->SetVisibility(0, this->GetMRMLScene(), inode);
            }
          }
        }
      }
    }
  }
  
  ////---------------------------------------------------------------------------
  //// Outgoing data
  //// TODO: should check the type of the node here
  //ConnectorListType* list = &MRMLEventConnectorMap[node];
  //ConnectorListType::iterator cliter;
  //for (cliter = list->begin(); cliter != list->end(); cliter ++)
  //  {
  //  vtkMRMLIGTLConnectorNode* connector =
  //    vtkMRMLIGTLConnectorNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(*cliter));
  //  if (connector == NULL)
  //    {
  //    return;
  //    }
  //
  //  MessageConverterListType::iterator iter;
  //  for (iter = this->MessageConverterList.begin();
  //       iter != this->MessageConverterList.end();
  //       iter ++)
  //    {
  //    if ((*iter)->GetMRMLName() && strcmp(node->GetNodeTagName(), (*iter)->GetMRMLName()) == 0)
  //      {
  //      // check if the name-type combination is on the list
  //      if (connector->GetDeviceID(node->GetName(), (*iter)->GetIGTLName()) >= 0)
  //        {
  //        int size;
  //        void* igtlMsg;
  //        (*iter)->MRMLToIGTL(event, node, &size, &igtlMsg);
  //        int r = connector->SendData(size, (unsigned char*)igtlMsg);
  //        if (r == 0)
  //          {
  //          // TODO: error handling
  //          //std::cerr << "ERROR: send data." << std::endl;
  //          }
  //        }
  //      }
  //    } // for (iter)
  //  } // for (cliter)
}


//
//
////----------------------------------------------------------------------------
//void vtkSlicerOpenIGTLinkIOIFLogic::ProcessLogicEvents(vtkObject *vtkNotUsed(caller),
//                                            unsigned long event,
//                                            void *callData)
//{
//  if (event ==  vtkCommand::ProgressEvent)
//    {
//    this->InvokeEvent(vtkCommand::ProgressEvent,callData);
//    }
//}


//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::ProcCommand(const char* vtkNotUsed(nodeName), int vtkNotUsed(size), unsigned char* vtkNotUsed(data))
{
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::GetDeviceNamesFromMrml(IGTLMrmlNodeListType &list)
{
  
  list.clear();
  
  MessageConverterListType::iterator mcliter;
  for (mcliter = this->MessageConverterList.begin();
       mcliter != this->MessageConverterList.end();
       mcliter ++)
  {
    if ((*mcliter)->GetMRMLName())
    {
      std::string className = this->GetMRMLScene()->GetClassNameByTag((*mcliter)->GetMRMLName());
      std::string deviceTypeName;
      if ((*mcliter)->GetIGTLName() != NULL)
      {
        deviceTypeName = (*mcliter)->GetIGTLName();
      }
      else
      {
        deviceTypeName = (*mcliter)->GetMRMLName();
      }
      std::vector<vtkMRMLNode*> nodes;
      this->GetApplicationLogic()->GetMRMLScene()->GetNodesByClass(className.c_str(), nodes);
      std::vector<vtkMRMLNode*>::iterator iter;
      for (iter = nodes.begin(); iter != nodes.end(); iter ++)
      {
        IGTLMrmlNodeInfoType nodeInfo;
        nodeInfo.name   = (*iter)->GetName();
        nodeInfo.type   = deviceTypeName.c_str();
        nodeInfo.io     = vtkMRMLIGTLConnectorNode::IO_UNSPECIFIED;
        nodeInfo.nodeID = (*iter)->GetID();
        list.push_back(nodeInfo);
      }
    }
  }
}

//---------------------------------------------------------------------------
void vtkSlicerOpenIGTLinkIOIFLogic::GetDeviceNamesFromMrml(IGTLMrmlNodeListType &list, const char* mrmlTagName)
{
  
  list.clear();
  
  MessageConverterListType::iterator mcliter;
  for (mcliter = this->MessageConverterList.begin();
       mcliter != this->MessageConverterList.end();
       mcliter ++)
  {
    if ((*mcliter)->GetMRMLName() && strcmp(mrmlTagName, (*mcliter)->GetMRMLName()) == 0)
    {
      const char* className = this->GetMRMLScene()->GetClassNameByTag(mrmlTagName);
      const char* deviceTypeName = (*mcliter)->GetIGTLName();
      std::vector<vtkMRMLNode*> nodes;
      this->GetMRMLScene()->GetNodesByClass(className, nodes);
      std::vector<vtkMRMLNode*>::iterator iter;
      for (iter = nodes.begin(); iter != nodes.end(); iter ++)
      {
        IGTLMrmlNodeInfoType nodeInfo;
        nodeInfo.name = (*iter)->GetName();
        nodeInfo.type = deviceTypeName;
        nodeInfo.io   = vtkMRMLIGTLConnectorNode::IO_UNSPECIFIED;
        nodeInfo.nodeID = (*iter)->GetID();
        list.push_back(nodeInfo);
      }
    }
  }
}

////---------------------------------------------------------------------------
//const char* vtkSlicerOpenIGTLinkIOIFLogic::MRMLTagToIGTLName(const char* mrmlTagName);
//{
//
//}
