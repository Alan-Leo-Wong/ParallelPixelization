#include <iostream>
#include <vector>
#include <iomanip>
//#include <igl/opengl/glfw/Viewer.h>
#include "pixelData.h"
//#include "MyViewer.h"
#include "pixelThread.h"

#ifdef USE_OPENMP

#include "pixelMP.h"

#endif

#include "infoLog/log.hpp"
#include "utils/common/timer.hpp"
#include "utils/common/cmdLineParser.hpp"

const char *default_file = "..//res//bigpolygon.wkt";

void set_viewConfig(Eigen::MatrixXd &color,
                    std::vector<Eigen::RowVector3d> &color_vec,
                    std::vector<float> &bb_point_size) {
    color.resize(4, 3);
    color << 0.5, 0.5, 0,
            0.5, 0.5, 0,
            0.5, 0.5, 0,
            0.5, 0.5, 0;

    color_vec.emplace_back(Eigen::RowVector3d(1, 0, 0));
    color_vec.emplace_back(Eigen::RowVector3d(0, 1, 0));
    color_vec.emplace_back(Eigen::RowVector3d(0, 0, 1));

    bb_point_size.emplace_back(2.5);
    bb_point_size.emplace_back(1e-9);
}

cmdLineParameter<char *> In("in");
cmdLineParameter<double> pixelSize("ps");
cmdLineParameter<int> threadsNum("num");
cmdLineReadable *params[] = {
        &In, &threadsNum, &pixelSize
};

int main(int argc, char *argv[]) {
    if (argc > 1) cmdLineParse(argc - 1, &argv[1], params);
    const char *filename = In.set ? In.value : default_file;
    LOG("file = ", std::string(filename), INFO);
    PointData pointData = PointData(filename);
    const double pixel_size = pixelSize.set ? pixelSize.value : 10;
    LOG("pixel_size = ", std::to_string(pixel_size), INFO);
    const int threads_num = threadsNum.set ? threadsNum.value : 16;
    LOG("threads_num = ", std::to_string(threads_num), INFO);

#ifdef USE_OPENMP
    /*openmp*/
    PixelMP pixelMp = PixelMP(pixel_size, &pointData, threads_num);

    Timer timer("--------OpenMP test--------");
    pixelMp.parallel_computing_openmp();
    timer.log();

//    igl::opengl::glfw::MyViewer myViewer = igl::opengl::glfw::MyViewer(&pixelMp);
#else
    /*pthread*/
    MyThread myThread = MyThread(pixel_size, &PointData, threads_num);

    Timer timer("--------pThread test--------");
    myThread.parallel_computing();
    timer.log();

    auto draw_pixel_mesh = myThread.get_draw_pixel_mesh();
    for (const auto &d: draw_pixel_mesh)
        std::cout << d << std::endl;
//    igl::opengl::glfw::MyViewer myViewer = igl::opengl::glfw::MyViewer(&myThread);
#endif

    /*Eigen::MatrixXd infoLog;
    std::vector<Eigen::RowVector3d> color_vec;
    std::vector<float> bb_point_size;
    set_viewConfig(infoLog, color_vec, bb_point_size);
    myViewer.set_data(color_vec);
//    myViewer.draw_point_and_bb(bb_point_size);
    myViewer.draw_pixel(infoLog);
    myViewer.launch();*/

    return EXIT_SUCCESS;
}