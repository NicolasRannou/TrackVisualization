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
  ${SOURCE_DATA_PATH}/vtkFFMPEGRenderWindowRecorder.cxx
  )
  
SET( vtkFFMPEGRenderWindowRecorderTestSources 
  vtkFFMPEGRenderWindowRecorderTest.cxx
  ${SOURCE_DATA_PATH}/vtkFFMPEGRenderWindowRecorder.cxx
  )

#SET(vtkFFMPEGRenderWindowRecorderTestHeaders
#  ${SOURCE_DATA_PATH}/vtkFFMPEGRenderWindowRecorder.h
# )
  
SET( vtkTheoraRenderWindowRecorderTestSources 
  vtkTheoraRenderWindowRecorderTest.cxx
  ${SOURCE_DATA_PATH}/vtkTheoraRenderWindowRecorder.cxx
  )  

#SET(vtkTheoraRenderWindowRecorderTestHeaders
#  ${SOURCE_DATA_PATH}/vtkTheoraRenderWindowRecorder.h
#  )
  
SET(SimpleViewUI ${SOURCE_DATA_PATH}/DesignerTrackWidget.ui)
SET(SimpleViewHeaders 
  ${SOURCE_DATA_PATH}/TrackWidget.h
  )
  
# For the dialog recorder

SET( DialogRecorderSrcs 
  DialogRecorderTest.cxx 
  ${SOURCE_DATA_PATH}/DialogRecorder.cxx
  ${SOURCE_DATA_PATH}/vtkFFMPEGRenderWindowRecorder.cxx
  )  
  
SET(DialogRecorderUI ${SOURCE_DATA_PATH}/DesignerDialogRecorder.ui)
SET(DialogRecorderHeaders 
  ${SOURCE_DATA_PATH}/DialogRecorder.h
  ) 

QT4_WRAP_UI(RecorderUISrcs ${DialogRecorderUI})
QT4_WRAP_CPP(RecorderMOCSrcs ${DialogRecorderHeaders} )


ADD_DEFINITIONS(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)

SET_SOURCE_FILES_PROPERTIES(${DialogRecorderSrcs} PROPERTIES
                           OBJECT_DEPENDS "${RecorderUISrcs}")

ADD_EXECUTABLE( DialogRecorderExe
  MACOSX_BUNDLE 
  ${DialogRecorderSrcs} ${RecorderUISrcs} ${RecorderMOCSrcs})
  
TARGET_LINK_LIBRARIES( DialogRecorderExe
  QVTK
  ${QT_LIBRARIES}  
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
)
  # Set your files and resources here  
#SET( DialogRecorder 
#  DialogRecorderTest.cxx 
#  ${SOURCE_DATA_PATH}/TrackWidget.cpp
#  ${SOURCE_DATA_PATH}/vtkFFMPEGRenderWindowRecorder.cxx
#  )

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
  
ADD_EXECUTABLE( vtkFFMPEGRenderWindowRecorderTest 
  ${vtkFFMPEGRenderWindowRecorderTestSources})

TARGET_LINK_LIBRARIES( vtkFFMPEGRenderWindowRecorderTest
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
)

ADD_EXECUTABLE( vtkTheoraRenderWindowRecorderTest 
  ${vtkTheoraRenderWindowRecorderTestSources})

TARGET_LINK_LIBRARIES( vtkTheoraRenderWindowRecorderTest
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
)

#ADD_TEST( LoadAndDisplayPolydatas
#  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TrackWidgetExe
#  ${TESTING_DATA_PATH}
#)

ADD_TEST( FFMPEGRecorder
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vtkFFMPEGRenderWindowRecorderTest
  FFMPEGRecorder.avi
)

ADD_TEST( TheoraRecorder
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vtkTheoraRenderWindowRecorderTest
  TheoraRecorder.ogg
)

