# TanDist

Starting point for a DPF - DISTRHO project.

Basic Audio distortion plugin built with DPF and Visage for the ui.

## Build

```bash
# get dependencies
git submodule update --init --recursive

# steps to build the ui lib
cd thirdparty/visage
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ../../..

# configure project
cmake -B build-cmake

# build the clap plugin
cmake --build build-cmake
```

Output: `build-cmake/bin/TanDist.clap`

## Dependencies

- DPF (DISTRHO Plugin Framework)
- Visage (GPU-accelerated graphics library)

## Structure

- `plugins/TanDist/` - Plugin source code
- `thirdparty/dpf/` - DPF framework
- `thirdparty/visage/` - Visage graphics library