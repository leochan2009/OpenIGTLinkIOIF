/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QtPlugin>

// OpenIGTLinkIOIF Logic includes
#include <vtkSlicerOpenIGTLinkIOIFLogic.h>

// OpenIGTLinkIOIF includes
#include "qSlicerOpenIGTLinkIOIFModule.h"
#include "qSlicerOpenIGTLinkIOIFModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerOpenIGTLinkIOIFModule, qSlicerOpenIGTLinkIOIFModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerOpenIGTLinkIOIFModulePrivate
{
public:
  qSlicerOpenIGTLinkIOIFModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerOpenIGTLinkIOIFModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerOpenIGTLinkIOIFModulePrivate::qSlicerOpenIGTLinkIOIFModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerOpenIGTLinkIOIFModule methods

//-----------------------------------------------------------------------------
qSlicerOpenIGTLinkIOIFModule::qSlicerOpenIGTLinkIOIFModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerOpenIGTLinkIOIFModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerOpenIGTLinkIOIFModule::~qSlicerOpenIGTLinkIOIFModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerOpenIGTLinkIOIFModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerOpenIGTLinkIOIFModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerOpenIGTLinkIOIFModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerOpenIGTLinkIOIFModule::icon() const
{
  return QIcon(":/Icons/OpenIGTLinkIOIF.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerOpenIGTLinkIOIFModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerOpenIGTLinkIOIFModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerOpenIGTLinkIOIFModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerOpenIGTLinkIOIFModule
::createWidgetRepresentation()
{
  return new qSlicerOpenIGTLinkIOIFModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerOpenIGTLinkIOIFModule::createLogic()
{
  return vtkSlicerOpenIGTLinkIOIFLogic::New();
}
