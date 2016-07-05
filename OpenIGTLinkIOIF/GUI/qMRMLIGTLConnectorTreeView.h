/*==============================================================================

  Program: 3D Slicer

  Copyright (c) 2010 Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qMRMLIGTLConnectorTreeView_h
#define __qMRMLIGTLConnectorTreeView_h

// qMRML includes
#include "qMRMLTreeView.h"

// OpenIGTLinkIOIF GUI includes
#include "qSlicerOpenIGTLinkIOIFModuleExport.h"

class qMRMLIGTLConnectorTreeViewPrivate;
class vtkMRMLNode;
class vtkMRMLScene;
class vtkSlicerOpenIGTLinkIOIFLogic;

/// \ingroup Slicer_QtModules_OpenIGTLinkIOIF
class Q_SLICER_QTMODULES_OPENIGTLINKIOIF_EXPORT qMRMLIGTLConnectorTreeView : public qMRMLTreeView
{
  Q_OBJECT

public:
  typedef qMRMLTreeView Superclass;
  qMRMLIGTLConnectorTreeView(QWidget *parent=0);
  virtual ~qMRMLIGTLConnectorTreeView();

  void setLogic(vtkSlicerOpenIGTLinkIOIFLogic* logic);

  void setSelectedNode(const char* id);

public slots:
  void setMRMLScene(vtkMRMLScene* scene);
//  void onSelectionChanged(const QItemSelection& index,const QItemSelection& beforeIndex);

protected:
  QScopedPointer<qMRMLIGTLConnectorTreeViewPrivate> d_ptr;
  #ifndef QT_NO_CURSOR
    void mouseMoveEvent(QMouseEvent* e);
    bool viewportEvent(QEvent* e);
  #endif
  virtual void mousePressEvent(QMouseEvent* event);

private:
  Q_DECLARE_PRIVATE(qMRMLIGTLConnectorTreeView);
  Q_DISABLE_COPY(qMRMLIGTLConnectorTreeView);

};

#endif
