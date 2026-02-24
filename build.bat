@echo off
setlocal

echo ========= BUILDING MeshIR =========
rem Arg1: configure preset MeshIR
rem Arg2: build preset MeshIR
rem Arg3: AVX flag (ON/OFF or 1/0, optional)
rem Arg4: AVX512 flag
rem Arg5: CUDA flag
rem Arg6: ROCM flag
rem Arg7: BLAS flag
rem Arg8: BUILD DEBUG

set CONFIG_PRESET=%1
set BUILD_PRESET=%2
set AVX2_FLAG=%3
set AVX512_FLAG=%4
set CUDA_FLAG=%5
set ROCM_FLAG=%6
set BLAS_FLAG=%7
set BUILD_DEBUG=%8
rem 
if "%CONFIG_PRESET%"=="" set CONFIG_PRESET=linux-release
if "%BUILD_PRESET%"=="" set BUILD_PRESET=linux-release-build
if "%AVX2_FLAG%"=="" set AVX2_FLAG=ON
if "%AVX512_FLAG%"=="" set AVX512_FLAG=ON
if "%CUDA_FLAG%"=="" set CUDA_FLAG=ON
if "%ROCM_FLAG%"=="" set ROCM_FLAG=ON
if "%BLAS_FLAG%"=="" set BLAS_FLAG=ON
if "%BUILD_DEBUG%"=="" set BUILD_DEBUG=ON

echo [build] Using CONFIG_PRESET=%CONFIG_PRESET%
echo [build] Using BUILD_PRESET=%BUILD_PRESET%
echo [build] Using USE_AVX256=%AVX2_FLAG%
echo [build] Using USE_AVX512=%AVX512_FLAG%
echo [build] Using USE_CUDA=%CUDA_FLAG%
echo [build] Using USE_ROCM=%ROCM_FLAG%
echo [build] Using USE_BLAS=%BLAS_FLAG%
echo [build] Using DEBUG=%BUILD_DEBUG%
echo [build] Configuring with CMake...

cmake --preset %CONFIG_PRESET% -DUSE_AVX256=%AVX2_FLAG% -DUSE_AVX512=%AVX512_FLAG% -DUSE_CUDA=%CUDA_FLAG% -DUSE_ROCM=%ROCM_FLAG% -DUSE_BLAS=%BLAS_FLAG% -DDEBUG=%BUILD_DEBUG%

if errorlevel 1 goto :fail
echo [build] Building...
cmake --build --preset %BUILD_PRESET%
if errorlevel 1 goto :fail
echo ========= BUILD COMPLETE =========
goto :end
:fail
echo ========= BUILD FAILED ==========
:end
endlocal