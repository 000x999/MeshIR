@echo off
setlocal

REM 
if not exist "build\MeshIR.exe" (
    echo [run_MeshIR] Error: build\MeshIR.exe not found.
    echo Build it first with: cmake --build --preset mingw-debug-build
    exit /b 1
)

echo [run_cppDL] Running MeshIR.exe...
"build\MeshIR.exe"

endlocal

