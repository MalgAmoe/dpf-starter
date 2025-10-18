cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()

project(TanDist VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release build optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Enable Link-Time Optimization (LTO) for cross-module optimization
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "LTO not supported: ${ipo_error}")
    endif()

    add_link_options(-s)
    add_link_options(-Wl,--gc-sections)


    # Size optimization + function/data sections for dead code elimination
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -ffunction-sections -fdata-sections")

    # Strip symbols and garbage collect unused sections
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections")
endif()

# Set policy in DPF's scope as well
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# Add DPF as subdirectory
add_subdirectory(thirdparty/dpf)

# Visage paths (already built separately)
set(VISAGE_ROOT ${CMAKE_SOURCE_DIR}/thirdparty/visage)
set(VISAGE_BUILD ${VISAGE_ROOT}/build)

# Add plugins
add_subdirectory(plugins/TanDist)
