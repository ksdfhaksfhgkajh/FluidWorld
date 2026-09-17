场景/参数
│
▼
Simulation World
├── Particle System
├── Boundary System
├── Neighbor Search
└── Solver
├── WCSPH
├── IISPH
├── DFSPH
└── PBF/PBD
│
▼
CUDA 粒子缓冲区 ── CUDA/OpenGL Interop ── OpenGL Renderer

# Dir
FluidSim/
├── CMakeLists.txt
├── cmake/
├── assets/
│   ├── shaders/
│   ├── scenes/
│   └── meshes/
├── include/fluidsim/
│   ├── core/
│   │   ├── Simulation.hpp
│   │   ├── SimulationConfig.hpp
│   │   ├── ParticleSet.hpp
│   │   ├── DeviceBuffer.hpp
│   │   └── Types.hpp
│   ├── neighbor/
│   │   ├── NeighborSearch.hpp
│   │   └── UniformGrid.hpp
│   ├── kernels/
│   │   ├── SmoothingKernel.hpp
│   │   └── CubicSplineKernel.hpp
│   ├── boundary/
│   │   ├── BoundaryModel.hpp
│   │   ├── ParticleBoundary.hpp
│   │   └── SDFBoundary.hpp
│   ├── solver/
│   │   ├── FluidSolver.hpp
│   │   ├── WCSPHSolver.hpp
│   │   ├── IISPHSolver.hpp
│   │   ├── DFSPHSolver.hpp
│   │   └── PBFSolver.hpp
│   ├── force/
│   │   ├── Viscosity.hpp
│   │   ├── SurfaceTension.hpp
│   │   └── ExternalForce.hpp
│   ├── renderer/
│   │   ├── Renderer.hpp
│   │   ├── ParticleRenderer.hpp
│   │   ├── SurfaceRenderer.hpp
│   │   └── CudaGLBuffer.hpp
│   └── scene/
│       ├── Scene.hpp
│       └── SceneLoader.hpp
├── src/
│   ├── core/
│   ├── neighbor/
│   ├── boundary/
│   ├── solver/
│   ├── renderer/
│   └── scene/
├── cuda/
│   ├── common/
│   │   ├── cuda_math.cuh
│   │   ├── kernel_utils.cuh
│   │   └── reduction.cuh
│   ├── neighbor/
│   │   ├── grid_hash.cu
│   │   ├── radix_sort.cu
│   │   └── neighbor_query.cu
│   ├── solver/
│   │   ├── wcsph.cu
│   │   ├── iisph.cu
│   │   ├── dfsph.cu
│   │   └── pbf.cu
│   ├── force/
│   └── boundary/
├── apps/
│   ├── viewer/
│   └── benchmark/
└── tests/
├── neighbor/
├── kernels/
└── solvers/