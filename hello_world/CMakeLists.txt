# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Set the path to the shared components directory
set(EXTRA_COMPONENT_DIRS "../../../esp/water-level-sensor-esp-idf/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hello_world)
