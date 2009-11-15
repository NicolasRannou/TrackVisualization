cmake_minimum_required(VERSION 2.6)

project(TrackVisualization)

SET( TESTING_DATA_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/TestPolydatas
)

ENABLE_TESTING()

add_subdirectory(src)
add_subdirectory(test)



