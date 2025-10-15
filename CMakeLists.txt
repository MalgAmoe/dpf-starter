cmake_minimum_required(VERSION 3.15)
project(FlangerPlugins VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add DPF as subdirectory
add_subdirectory(thirdparty/dpf)

# Visage paths (already built separately)
set(VISAGE_ROOT ${CMAKE_SOURCE_DIR}/thirdparty/visage)
set(VISAGE_BUILD ${VISAGE_ROOT}/build)

# Add plugins
add_subdirectory(plugins/TanDist)
