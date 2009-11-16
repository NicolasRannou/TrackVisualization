cmake_minimum_required(VERSION 2.6)

project(TrackVisualization)

SET( TESTING_DATA_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/test/polydatas
)

SET( SOURCE_DATA_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

ENABLE_TESTING()

add_subdirectory(src)
add_subdirectory(test)



