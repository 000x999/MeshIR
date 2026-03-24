cmake_minimum_required(VERSION 3.20)
project(MeshIR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(USE_AVX256    "Enable AVX2/FMA etc."    ON  )
option(USE_AVX512    "Enable AVX512/VNNI etc." ON  )
option(DEBUG_MESHIR  "Enable DEBUG macro"      OFF )
option(USE_VULKAN    "Enable Vulkan support"   OFF )
option(USE_CUDA      "Enable CUDA support"     OFF ) 
option(USE_ROCM      "Enable ROCm support"     OFF )
option(USE_OPENGL    "Enable OpenGL support"   OFF )

find_package(OpenMP)

file(GLOB_RECURSE MeshIR_IMPL_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ir_alloc_impl/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ir_core_impl/*.cpp
)

file(GLOB_RECURSE MeshIR_TEST_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/test_bed/src/*.cpp
)

#set(CRUSHBLAS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/core/include/CRUSHBLAS_MODULE)
#file(GLOB_RECURSE CRUSHBLAS_SOURCES CONFIGURE_DEPENDS
 # ${CRUSHBLAS_ROOT}/core/impl/*.cpp
  #${CRUSHBLAS_ROOT}/core/impl/*.cc
  #${CRUSHBLAS_ROOT}/core/impl/*.cxx
#)

add_executable(MeshIR
  ${MeshIR_TEST_SOURCES}
  ${MeshIR_IMPL_SOURCES}
 # ${CRUSHBLAS_SOURCES}
)

target_include_directories(MeshIR PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meshir
  #${CRUSHBLAS_ROOT}/core
  #${CRUSHBLAS_ROOT}/core/include
  #${CRUSHBLAS_ROOT}/core/impl
  #${CRUSHBLAS_ROOT}/core/BLAS
)

target_compile_definitions(MeshIR PRIVATE
  USE_AVX256=$<BOOL:${USE_AVX256}>
  USE_AVX512=$<BOOL:${USE_AVX512}>
  USE_VULKAN=$<BOOL:${USE_VULKAN}>
  USE_OPENGL=$<BOOL:${USE_OPENGL}>
  USE_CUDA=$<BOOL:${USE_CUDA}> 
  USE_ROCM=$<BOOL:${USE_ROCM}>
  DEBUG=$<BOOL:${DEBUG_MESHIR}>
)

if(MSVC)
  target_compile_options(MeshIR PRIVATE /W4 /O2)
  if(USE_AVX256)
    target_compile_options(MeshIR PRIVATE /arch:AVX2)
  endif()
  
  if(USE_AVX512)
    target_compile_options(MeshIR PRIVATE /arch:AVX512)
  endif()
else()
  target_compile_options(MeshIR PRIVATE
    -Wall -Wextra -Wpedantic
    -O3 -march=native
  )
  
  if(USE_AVX256)
    target_compile_options(MeshIR PRIVATE 
      -mavx2 -mfma -mavx
      -mavx512f -mavx512vl -mavx512bw -mavx512dq -mavx512cd
      -mavx512vbmi -mavx512vbmi2 -mavx512vnni -mavx512bitalg
      -mavx512vpopcntdq -mavx512ifma -mavx512bf16
    )
  endif()
  
  if(USE_AVX512)
    target_compile_options(MeshIR PRIVATE 
      -mavx2 -mfma -mavx
      -mavx512f -mavx512vl -mavx512bw -mavx512dq -mavx512cd
      -mavx512vbmi -mavx512vbmi2 -mavx512vnni -mavx512bitalg
      -mavx512vpopcntdq -mavx512ifma -mavx512bf16
    )
  endif()
endif()



if(OpenMP_CXX_FOUND)
  target_link_libraries(MeshIR PRIVATE OpenMP::OpenMP_CXX)
endif()

if(WIN32)
  add_custom_target(run
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/run.bat"
    DEPENDS MeshIR
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Building and running MeshIR via run.bat"
  )
endif()
