# CMakeFiles.txt inside "AsyncDelay-1.1.2" folder
cmake_minimum_required(VERSION 3.5)
idf_component_register(INCLUDE_DIRS "src" # Only include directory is needed
                    REQUIRES "arduino"    # Dependency on Arduino
)


project(AsyncDelay)