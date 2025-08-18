# Photon

CPU path tracer written in modern C++20. Photon loads scene descriptions, traces Monte Carlo light paths, and writes progressive render output.

## Architecture

```
apps/photon_cli     CLI entry point
include/photon/     Public headers (math, geometry, accel, materials, ...)
src/                Core library implementation
tests/              Unit tests
scenes/             Example scene files
benchmarks/         Performance benchmarks
```

The renderer is organized around a tile-based worker pool, BVH-accelerated intersection, principled PBR materials, and deterministic sampling.

## Build

Requirements: CMake 3.25+, Ninja, a C++20 compiler.

```bash
cmake --preset default
cmake --build --preset default
ctest --preset default
```

Run the CLI health check:

```bash
./build/default/photon --health
```

Environment defaults can be overridden via `.env` values listed in `.env.example`.

## Platforms

Tested on macOS and Linux with Clang and GCC. Windows builds should work with MSVC 19.3+ using the same presets.

## Scene format

Scenes are JSON documents describing camera, materials, meshes, lights, and render settings. Example scenes will live under `scenes/`.

## Status

Early scaffold. Math, geometry, materials, integrator, and renderer modules are landing incrementally.

## License

MIT
