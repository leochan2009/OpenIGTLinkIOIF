
#-----------------------------------------------------------------------------
set(MODULE_NAME OpenIGTLinkIOIF)
set(MODULE_TITLE ${MODULE_NAME})

string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_SLICER_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")
set(OpenIGTLinkIO_Include CACHE STRING "OpenIGTLinkIO include")
set(OpenIGTLinkIO_Build CACHE STRING "OpenIGTLinkIO Build")
# Current_{source,binary} and Slicer_{Libs,Base} already included
#-----------------------------------------------------------------------------
add_subdirectory(Logic)

set(MODULE_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/GUI
  ${CMAKE_CURRENT_BINARY_DIR}/GUI
  ${CMAKE_CURRENT_SOURCE_DIR}/Logic
  ${CMAKE_CURRENT_BINARY_DIR}/Logic
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
  ${CMAKE_CURRENT_BINARY_DIR}/Widgets
  ${OpenIGTLinkIO_Include}/Logic
  ${OpenIGTLinkIO_Include}/Devices
  ${OpenIGTLinkIO_Include}/Converter
  ${OpenIGTLinkIO_Build}/Logic
  ${OpenIGTLinkIO_Build}/Devices
  ${OpenIGTLinkIO_Build}/Converter
  ${OpenIGTLink_Include}
  )

set(MODULE_SRCS
  qSlicer${MODULE_NAME}Module.cxx
  qSlicer${MODULE_NAME}Module.h
  qSlicer${MODULE_NAME}ModuleWidget.cxx
  qSlicer${MODULE_NAME}ModuleWidget.h
  GUI/qMRMLSceneIGTLConnectorModel.cxx
  GUI/qMRMLSceneIGTLConnectorModel.h
  GUI/qMRMLIGTLConnectorTreeView.cxx
  GUI/qMRMLIGTLConnectorTreeView.h
  GUI/qMRMLIGTLIOModel.cxx
  GUI/qMRMLIGTLIOModel.h
  GUI/qMRMLIGTLIOModel_p.h
  GUI/qMRMLIGTLIOTreeView.cxx
  GUI/qMRMLIGTLIOTreeView.h
  GUI/qSlicerIGTLConnectorPropertyWidget.cxx
  GUI/qSlicerIGTLConnectorPropertyWidget.h
  GUI/qSlicerIGTLIONodeSelectorWidget.cxx
  GUI/qSlicerIGTLIONodeSelectorWidget.h
  )

set(MODULE_MOC_SRCS
  qSlicer${MODULE_NAME}Module.h
  qSlicer${MODULE_NAME}ModuleWidget.h
  )

set(MODULE_UI_SRCS
  Resources/UI/qSlicer${MODULE_NAME}ModuleWidget.ui
  Resources/UI/qSlicerIGTLConnectorPropertyWidget.ui
  Resources/UI/qSlicerIGTLIONodeSelectorWidget.ui
  Resources/UI/qSlicerOpenIGTLinkIOIFModule.ui
  )
LINK_DIRECTORIES("${OpenIGTLink_Lib}")
LINK_DIRECTORIES("${OpenIGTLinkIO_Build}/bin")
set(MODULE_TARGET_LIBRARIES
  vtkSlicer${MODULE_NAME}ModuleLogic
  qSlicer${MODULE_NAME}ModuleWidgets
  OpenIGTLink
  igtlioConverter
  igtlioDevices
  )

set(MODULE_RESOURCES
  Resources/qSlicer${MODULE_NAME}Module.qrc
  )

#-----------------------------------------------------------------------------
slicerMacroBuildQtModule(
  NAME ${MODULE_NAME}
  TITLE ${MODULE_TITLE}
  EXPORT_DIRECTIVE ${MODULE_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  SRCS ${MODULE_SRCS}
  MOC_SRCS ${MODULE_MOC_SRCS}
  UI_SRCS ${MODULE_UI_SRCS}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
  RESOURCES ${MODULE_RESOURCES}
  WITH_GENERIC_TESTS
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
