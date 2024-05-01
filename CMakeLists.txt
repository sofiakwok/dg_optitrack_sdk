#
# Copyright (c) 2019, New York University and Max Planck Gesellschaft.
#
# License BSD-3 clause
#

#
# set up the project
#
cmake_minimum_required(VERSION 3.10.2)

project(dg_optitrack_sdk)

# Using C++17
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

#
# Dependencies
#
# depend on ament macros
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(mpi_cmake_modules REQUIRED)
find_package(dynamic-graph REQUIRED)
find_package(dynamic-graph-python REQUIRED)
find_package(mocap4r2_optitrack_driver REQUIRED)

# export de dependencies
ament_export_dependencies(ament_cmake mpi_cmake_modules mocap4r2_optitrack_driver)

#
# manage the creation of the libraries and executables
#
add_subdirectory(src)

#
# manage the unit tests
#
# if(BUILD_TESTING)
#   add_subdirectory(tests)
# endif()

#
# manage the demos
#
# add_subdirectory(demos)

#
# Building documentation.
#
add_documentation()

#
# Install and export package.
#
install(DIRECTORY include/ DESTINATION include)

#
# Export the package as ament.
#
ament_package()
