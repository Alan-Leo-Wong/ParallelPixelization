//
// Created by lei on 22-10-19.
//

#include "pixelThread.h"

std::queue<GPIXEL> pixelThread::q;
pthread_mutex_t pixelThread::mutex;

void pixelThread::create_threads() {
    for (int i = 0; i < threads_num; ++i) {
        pthread_create(&threads[i], nullptr, do_something, this);
    }
}

void *pixelThread::do_something(void *args) {
    auto *p = (Pixel *) args;
    while (true) {
        bool is_empty = true;
        pthread_mutex_lock(&mutex);
        GPIXEL gp;
        if (!q.empty()) {
            gp = q.front(); // 需要互斥地获取资源
            q.pop();
            is_empty = false;
        }
        pthread_mutex_unlock(&mutex);
        if(is_empty) break;
        p->recasting_get_drawPixel(gp);
    }
    return nullptr;
}

void pixelThread::push_resource() {
    Eigen::MatrixXd centers = Pixel::get_center();
    int rows = centers.rows();
//    std::cout << "像素个数: " << centers.rows() << std::endl;
    int pixel_num = rows / threads_num;
    int index = 0;
    do {
//        pthread_mutex_lock(&mutex);
        GPIXEL gp;
        for (int i = 0; i < pixel_num; ++i) {
            if (index + i < rows) {
                // 保存index+i为了用于在recasting_get_drawPixel函数中draw_pixel_mesh的正确推入待绘制的像素
                gp.emplace_back(std::make_pair(centers.row(index + i), index + i));
            } else {
                break;
            }
        }
        index += pixel_num;
        q.push(gp);
    } while (index < rows);
}

void pixelThread::join_threads() {
    for (int i = 0; i < threads_num; ++i) {
        void *status;
        pthread_join(threads[i], &status);
    }
}

void pixelThread::parallel_computing() {
    push_resource(); // 主线程用于push资源
    create_threads();
    join_threads();
}
