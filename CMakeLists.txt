cmake_minimum_required(VERSION 3.9)
project(GeodeticSolid VERSION 1.0.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")
if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif()

# Dependencies
find_package(Eigen3 CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

# Collect linked libraries
set(GEODETIC_LIBS Eigen3::Eigen Threads::Threads GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

# Add subdirectories
add_subdirectory(ExportParaview)
add_subdirectory(src)
add_subdirectory(src_test)

# -- Main executable ----------------------------------------------------------------
add_executable(${PROJECT_NAME}
  main.cpp
)
# Attach sources & headers
target_sources(${PROJECT_NAME} PRIVATE
  ${GeodeticSolid_sources}
  ${GeodeticSolid_headers}
  ${src_paraview_sources}
)
# Include paths
target_include_directories(${PROJECT_NAME} PRIVATE
  ${GeodeticSolid_includes}
  ${src_paraview_includes}
)
# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${GEODETIC_LIBS})

# -- Test executable ----------------------------------------------------------------
add_executable(${PROJECT_NAME}_test
  main_test.cpp
)
# Attach test sources & headers
target_sources(${PROJECT_NAME}_test PRIVATE
  ${GeodeticSolid_sources}
  ${GeodeticSolid_headers}
  ${src_paraview_sources}
  ${src_test_sources}
  ${src_test_headers}
)
# Include paths for tests
target_include_directories(${PROJECT_NAME}_test PRIVATE
  ${GeodeticSolid_includes}
  ${src_paraview_includes}
  ${src_test_includes}
)
# Link libraries for tests
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${GEODETIC_LIBS})

# PIC option
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)
target_compile_options(${PROJECT_NAME}_test PUBLIC -fPIC)
