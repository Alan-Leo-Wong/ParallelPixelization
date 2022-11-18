cmake_minimum_required(VERSION 3.22)
project(GPUProgram LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 14)

option(USE_OPENMP "Use OpenMP" ON)
option(USE_CUDA "Use CUDA" ON)
message(STATUS "USE_OPENMP: ${USE_OPENMP}")
message(STATUS "USE_CUDA: ${USE_CUDA}")

add_subdirectory(src)
add_subdirectory(include)

include_directories(include)

# CPU
set(CPU_LIBS pthread_pixel openmp_pixel)

# add target
string(CONCAT CPU_MAIN_NAME "CPU_PIXEL" ".out")
add_executable(${CPU_MAIN_NAME} ${PROJECT_SOURCE_DIR}/pixelCPUTest.cpp)
target_link_libraries(${CPU_MAIN_NAME} PRIVATE ${CPU_LIBS})

if (USE_OPENMP)
    target_compile_definitions(${CPU_MAIN_NAME} PRIVATE USE_OPENMP)
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_OPENMP")
endif ()

# Eigen used vectorization
if (TARGET Eigen3::Eigen)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-march=native" _march_native_works)
    check_cxx_compiler_flag("-xHOst" _xhost_works)
    set(_CXX_FLAGS)
    if (_march_native_works)
        message(STATUS "Using processor's vector instructions (-march=native compiler flag set)")
        set(_CXX_FLAGS "-march=native")
    elseif (_xhost_works)
        message(STATUS "Using processor's vector instructions (-xHost compiler flag set)")
        set(_CXX_FLAGS "-xHost")
    else ()
        message(STATUS "No suitable compiler flag found for vectorization")
    endif ()
    target_compile_options(${CPU_MAIN_NAME} PRIVATE ${_CXX_FLAGS})
endif ()

if (USE_CUDA)
    if (NOT DEFINED CMAKE_CUDA_COMPILER)
        set(CMAKE_CUDA_COMPILER /usr/local/cuda-11.8/bin/nvcc)
    endif ()
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 14)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)

    string(CONCAT GPU_MAIN_NAME "GPU_PIXEL" ".out")
    add_executable(${GPU_MAIN_NAME} ${PROJECT_SOURCE_DIR}/pixelCUDATest.cu)
    target_link_libraries(${GPU_MAIN_NAME} PRIVATE point_data pixel_data)
#    target_link_directories(${GPU_MAIN_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/CUDA)

    target_compile_features(${GPU_MAIN_NAME} PRIVATE cuda_std_14)

    set_target_properties(${GPU_MAIN_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    if (NOT DEFINED CUDA_ARCHITECTURES)
        set_property(TARGET ${GPU_MAIN_NAME} PROPERTY CUDA_ARCHITECTURES 86-real 86-virtual)
    endif ()
endif ()