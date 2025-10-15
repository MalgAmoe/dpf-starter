# TanDist Plugin CMake Configuration

# Define plugin target using DPF's macros
dpf_add_plugin(TanDist
    TARGETS clap
    FILES_DSP
        TanDist.cpp
    FILES_UI
        TanDistUI.cpp
)

# Set plugin properties
target_include_directories(TanDist PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add Visage includes and libraries for UI
target_include_directories(TanDist-ui PUBLIC
    ${VISAGE_BUILD}/include
    ${VISAGE_ROOT}
    ${VISAGE_BUILD}/visage_graphics/VisageEmbeddedFonts_generated
    ${VISAGE_ROOT}/visage_file_embed
)

# Link Visage and its dependencies
target_link_libraries(TanDist-ui PUBLIC
    ${VISAGE_BUILD}/libvisage.a
    ${VISAGE_BUILD}/_deps/bgfx-build/cmake/bgfx/libbgfx.a
    ${VISAGE_BUILD}/_deps/bgfx-build/cmake/bimg/libbimg.a
    ${VISAGE_BUILD}/_deps/bgfx-build/cmake/bimg/libbimg_decode.a
    ${VISAGE_BUILD}/_deps/bgfx-build/cmake/bx/libbx.a
    ${VISAGE_BUILD}/_deps/freetype-build/libfreetype.a
    ${VISAGE_BUILD}/visage_graphics/libVisageEmbeddedFonts.a
    ${VISAGE_BUILD}/visage_graphics/libVisageEmbeddedShaders.a
    ${VISAGE_BUILD}/visage_graphics/libVisageEmbeddedIcons.a
    ${VISAGE_BUILD}/visage_graphics/third_party/nanosvg/libnanosvg.a
    ${VISAGE_BUILD}/visage_graphics/third_party/nanosvg/libnanosvgrast.a
)

# Plugin metadata is defined in DistrhoPluginInfo.h
