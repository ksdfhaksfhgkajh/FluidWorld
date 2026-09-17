# FluidWorld

FluidWorld is an incremental fluid-simulation library. The current milestone is a
small, testable CPU reference implementation of three-dimensional weakly
compressible SPH (WCSPH). CUDA solvers and OpenGL visualization will be added on
top of the same particle data model.

## Current features

- Structure-of-arrays particle storage
- Poly6 density, spiky pressure-gradient, and viscosity kernels
- CPU uniform-grid neighbor search
- Symmetric WCSPH pressure force and viscosity
- Semi-implicit Euler integration
- Axis-aligned box boundary
- Console example and dependency-free tests

## Build and run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/fluidworld_simple_sph
```

The example prints density, neighbor-count, and height diagnostics. It deliberately
does not render yet; keeping this first milestone headless makes the numerical core
easy to test before CUDA and OpenGL are introduced.

## Source layout

```text
include/fluidsim/core       particle data, configuration, simulation facade
include/fluidsim/kernel     SPH smoothing kernels
include/fluidsim/neighbor   spatial neighbor search
include/fluidsim/boundary   collision boundaries
include/fluidsim/solver     fluid solvers
src                         implementations
apps/simple_sph             minimal executable
tests                       numerical and integration smoke tests
cuda                        reserved for GPU implementations
assets                      reserved for shaders, scenes, and meshes
```

## Planned milestones

1. Validate the WCSPH reference with static-fluid and dam-break scenes.
2. Add CUDA device buffers and a sorted uniform-grid neighbor search.
3. Port WCSPH stages to CUDA and compare against the CPU implementation.
4. Add an OpenGL particle viewer and CUDA/OpenGL buffer interoperability.
5. Add IISPH, DFSPH, and PBF using the shared infrastructure.
