//
// Created by lei on 22-10-21.
//

#ifndef GPUPROGRAM_PIXELMP_H
#define GPUPROGRAM_PIXELMP_H

#include <queue>
#include <vector>
#include <omp.h>
#include "pixelData.h"

class PixelMP : public Pixel {
protected:
    int threads_num;
    std::queue<GPIXEL> q;
    omp_lock_t mutex_mp;
public:
    PixelMP(double size, PointData *pointData, const int &num = 16) : Pixel(size, pointData), threads_num(num) {
        omp_init_lock(&mutex_mp);
    }

    // 使用openmp执行并行计算
    void parallel_computing_openmp();

    // push资源
    void push_resource_openmp();

    void do_something_openmp();

    ~PixelMP() {
        omp_destroy_lock(&mutex_mp);
    }
};


#endif //GPUPROGRAM_PIXELMP_H
