//
// Created by lei on 22-11-16.
//

#ifndef GPUPROGRAM_CUDACHECK_CUH
#define GPUPROGRAM_CUDACHECK_CUH

#include "../../infoLog/infoPrint.hpp"

#define CUDA_CHECK(call) \
do{                                     \
     const cudaError_t error_code = call; \
     if (error_code != cudaSuccess) {   \
        ERROR("CUDA Error:");        \
        _ERROR("    --File:       "); INFO(__FILE__);     \
        _ERROR("    --Line:       "); INFO(__LINE__);     \
        _ERROR("    --Error code: "); INFO(error_code);   \
        _ERROR("    --Error text: "); INFO(cudaGetErrorString(error_code));   \
        exit(1);           \
     }\
}while(0)

#endif //GPUPROGRAM_CUDACHECK_CUH
