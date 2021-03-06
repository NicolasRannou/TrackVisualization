# Use the include path and library for Qt that is used by VTK.
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${SOURCE_DATA_PATH}
)

# Set your files and resources here  
SET( SimpleViewSrcs 
  TrackVisualizationTest.cxx 
  ${SOURCE_DATA_PATH}/TrackWidget.cpp
  )

SET(SimpleViewUI ${SOURCE_DATA_PATH}/DesignerTrackWidget.ui)
SET(SimpleViewHeaders 
  ${SOURCE_DATA_PATH}/TrackWidget.h
  )
  

# The rest should just work (sure...)
QT4_WRAP_UI(UISrcs ${SimpleViewUI})
QT4_WRAP_CPP(MOCSrcs ${SimpleViewHeaders} )
QT4_ADD_RESOURCES(ResourceSrcs ${SimpleViewResources})

SOURCE_GROUP("Resources" FILES
  ${SimpleViewUI}
  ${SimpleViewResources}
  ${EXE_ICON}
)

SOURCE_GROUP("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs}
  ${RCS_SOURCES}
)

SET_SOURCE_FILES_PROPERTIES(${SimpleViewSrcs} PROPERTIES
                            OBJECT_DEPENDS "${UISrcs}")
                            
# It's nice to have the ui in the windows project file...just double click on it
# and designer comes up on that ui file :)
IF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
  SET (SimpleViewSrcs ${SimpleViewSrcs} ${SimpleViewUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
IF (${CMAKE_BUILD_TOOL} MATCHES "devenv")
  SET (SimpleViewSrcs ${SimpleViewSrcs} ${SimpleViewUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "devenv")

ADD_EXECUTABLE( TrackWidgetExe 
  MACOSX_BUNDLE 
  ${SimpleViewSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs})
  
TARGET_LINK_LIBRARIES( TrackWidgetExe
  QVTK
  ${QT_LIBRARIES}  
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
)