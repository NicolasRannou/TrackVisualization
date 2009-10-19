cmake_minimum_required(VERSION 2.7)

PROJECT(TrackVisualization)

FIND_PACKAGE(VTK REQUIRED)
IF(NOT VTK_USE_RENDERING)
  MESSAGE(FATAL_ERROR "Example ${PROJECT_NAME} requires VTK_USE_RENDERING.")
ENDIF(NOT VTK_USE_RENDERING)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE(TrackVisualizationTest TrackVisualizationTest.cxx)
TARGET_LINK_LIBRARIES(TrackVisualizationTest vtkRendering vtkImaging vtkIO)

