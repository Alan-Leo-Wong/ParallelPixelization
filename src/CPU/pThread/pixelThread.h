//
// Created by lei on 22-10-19.
//

#ifndef GPUPROGRAM_PIXELTHREAD_H
#define GPUPROGRAM_PIXELTHREAD_H

#include <thread>
#include <queue>
#include <vector>
#include "pixelData.h"

class pixelThread : public Pixel {
protected:
    int threads_num;
    pthread_t *threads;
    static std::queue<GPIXEL> q;
    static pthread_mutex_t mutex;
public:
    pixelThread(double size, PointData *pointData, const int &num = 16) : Pixel(size, pointData), threads_num(num) {
        threads = new pthread_t[num];
        pthread_mutex_init(&mutex, nullptr);
    }

    // 使用pthread执行并行计算
    void parallel_computing();

    void create_threads();

    // push资源
    void push_resource();

    void join_threads();

    static void *do_something(void *args);

    ~pixelThread() {
        delete threads;
        pthread_mutex_destroy(&mutex);
    }
};

#endif //GPUPROGRAM_PIXELTHREAD_H
