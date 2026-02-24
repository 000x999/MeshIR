#!/bin/bash

# run.sh
if [ ! -f "build/MeshIR" ]; then
    echo "[run_MeshIR] Error: build/MeshIR not found."
    echo "Build it first with: cmake --build --preset mingw-debug-build"
    exit 1
fi

echo "[run_MeshIR] Running MeshIR..."
./build/MeshIR
