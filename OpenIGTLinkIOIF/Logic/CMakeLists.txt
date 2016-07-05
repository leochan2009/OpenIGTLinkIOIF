project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")
set(${KIT}_INCLUDE_DIRECTORIES
    ${OpenIGTLinkIO_Include}/Logic
    ${OpenIGTLinkIO_Build}/Logic
    ${OpenIGTLinkIO_Include}/Devices
    ${OpenIGTLinkIO_Build}/Devices
    ${OpenIGTLinkIO_Include}/Converter
    ${OpenIGTLinkIO_Build}/Converter
  )

set(${KIT}_SRCS
  vtkSlicer${MODULE_NAME}Logic.cxx
  vtkSlicer${MODULE_NAME}Logic.h
  vtkIGTLIOConnectorWrapper.cxx
  vtkIGTLIOConnectorWrapper.h
  )

LINK_DIRECTORIES("${OpenIGTLinkIO_Build}/bin")
set(${KIT}_TARGET_LIBRARIES
  ${ITK_LIBRARIES}
  igtlioDevices
  igtlioLogic
  igtlioConverter
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
