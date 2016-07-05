cmake_minimum_required(VERSION 2.8.9)

project(OpenIGTLinkIOIF)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "http://slicer.org/slicerWiki/index.php/Documentation/Nightly/Extensions/OpenIGTLinkIOIF")
set(EXTENSION_CATEGORY "Examples")
set(EXTENSION_CONTRIBUTORS "John Doe (AnyWare Corp.)")
set(EXTENSION_DESCRIPTION "This is an example of an extension using SuperBuild to build a dependent project named Foo")
set(EXTENSION_ICONURL "http://www.example.com/Slicer/Extensions/OpenIGTLinkIOIF.png")
set(EXTENSION_SCREENSHOTURLS "http://www.example.com/Slicer/Extensions/OpenIGTLinkIOIF/Screenshots/1.png")
set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
set(EXTENSION_BUILD_SUBDIRECTORY inner-build)

set(SUPERBUILD_TOPLEVEL_PROJECT inner)

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})
mark_as_superbuild(Slicer_DIR)

find_package(Git REQUIRED)
mark_as_superbuild(GIT_EXECUTABLE)


#-----------------------------------------------------------------------------
# Extension modules
set(OpenIGTLinkIO_Include "default value" CACHE STRING "OpenIGTLinkIO source file")
set(OpenIGTLinkIO_Lib "default value" CACHE STRING "OpenIGTLinkIO library file")
set(OpenIGTLink_Include "default value" CACHE STRING "OpenIGTLink source file")
set(OpenIGTLink_Lib "default value" CACHE STRING "OpenIGTLink library file")
add_subdirectory(OpenIGTLinkIOIF)
## NEXT_MODULE

#-----------------------------------------------------------------------------
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
#set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${Foo_DIR};Foo;RuntimeLibraries;/")
include(${Slicer_EXTENSION_CPACK})
