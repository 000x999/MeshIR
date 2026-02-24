#!/bin/bash

# build.sh
echo "========= BUILDING CPPDL ========="

# Arg1: configure preset MeshIR
# Arg2: build preset MeshIR
# Arg3: AVX flag (ON/OFF or 1/0, optional)
# Arg4: BLAS flag
# Arg5: BUILD DEBUG

CONFIG_PRESET="${1:-linux-release}"
BUILD_PRESET="${2:-linux-release-build}"
AVX2_FLAG="${3:-ON}"
AVX512_FLAG="${4:-ON}"
CUDA_FLAG="${5:-ON}"
ROCM_FLAG="${6:-ON}"
BLAS_FLAG="${7:-ON}"
BUILD_DEBUG="${8:-ON}"

echo "[build] Using CONFIG_PRESET=$CONFIG_PRESET"
echo "[build] Using BUILD_PRESET=$BUILD_PRESET"
echo "[build] Using USE_AVX256=$AVX2_FLAG"
echo "[build] Using USE_AVX512=$AVX512_FLAG"
echo "[build] Using USE_CUDA=$CUDA_FLAG"
echo "[build] Using USE_ROCM=$ROCM_FLAG"
echo "[build] Using USE_BLAS=$BLAS_FLAG"
echo "[build] Using DEBUG=$BUILD_DEBUG"

echo "[build] Configuring with CMake..."
cmake --preset "$CONFIG_PRESET" -DUSE_AVX256="$AVX2_FLAG" -DUSE_AVX512="$AVX512_FLAG" -DUSE_CUDA="$CUDA_FLAG" -DUSE_ROCM="$ROCM_FLAG" -DUSE_BLAS="$BLAS_FLAG" -DDEBUG="$BUILD_DEBUG"

if [ $? -ne 0 ]; then
    echo "========= BUILD FAILED ========="
    exit 1
fi

echo "[build] Building..."
cmake --build --preset "$BUILD_PRESET"

if [ $? -ne 0 ]; then
    echo "========= BUILD FAILED ========="
    exit 1
fi

echo "========= BUILD COMPLETE ========="
