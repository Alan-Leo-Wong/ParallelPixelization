//
// Created by lei on 22-10-21.
//

#include <iostream>
#include "pixelMP.h"

void PixelMP::push_resource_openmp() {
    Eigen::MatrixXd centers = Pixel::get_center();
    int rows = centers.rows();
//    std::cout << "像素个数: " << centers.rows() << std::endl;
    int pixel_num = rows / threads_num;
    int index = 0;
    do {
        GPIXEL gp;
        for (int i = 0; i < pixel_num; ++i) {
            if (index + i < rows) {
                // 保存index+i为了用于在recasting_get_drawPixel函数中draw_pixel_mesh的正确推入待绘制的像素
                gp.emplace_back(std::make_pair(centers.row(index + i), index + i));
//                std::cout << i << std::endl;
            } else {
                break;
            }
        }
        index += pixel_num;
        q.push(gp);
//        std::cout << "push的gp.size: " << gp.size() << std::endl;
    } while (index < rows);
}

void PixelMP::do_something_openmp() {
//    std::cout << "thread " << omp_get_thread_num() << " entry" << std::endl;
    while (true) {
        bool is_empty = true;
        omp_set_lock(&mutex_mp);
        GPIXEL gp;
        if (!q.empty()) {
            gp = q.front(); // 需要互斥地获取资源
            q.pop();
//        std::cout << "q.size: " << q.size() << std::endl;
            is_empty = false;
        }
        omp_unset_lock(&mutex_mp);
        if (is_empty) {
//            std::cout << "thread " << omp_get_thread_num() << " wait resource...." << std::endl;
            break;
        }
        this->recasting_get_drawPixel(gp);
//    std::cout << "thread " << omp_get_thread_num() << " work done!" << std::endl;
    }
}

void PixelMP::parallel_computing_openmp() {
    push_resource_openmp();

#pragma omp parallel num_threads(threads_num)
    {
        do_something_openmp();
    }
}
