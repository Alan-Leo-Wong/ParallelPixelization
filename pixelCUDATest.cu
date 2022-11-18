//
// Created by lei on 22-11-15.
//

#include <unistd.h>
#include "PointData.h"
#include "pixelData.h"
#include "utils/CUDA/cudaCheck.cuh"
#include "utils/common/timer.hpp"
#include "infoLog/infoPrint.hpp"
#include "utils/common/cmdLineParser.hpp"
#include "infoLog/log.hpp"
//#include "src/Visualization/MyViewer.h"

__device__ double d_end_w_coordinate;

__device__ double vector_product(const double *vectorA,
                                 const double *vectorB) {
    /*计算 x_1 * y_2 - x_2 * y_1*/
    return vectorA[0] * vectorB[1] - vectorB[0] * vectorA[1];
}

__device__ bool check_intersect(const double *A,
                                const double *B,
                                const double *C,
                                const double *D) {
    double AC[2] = {C[0] - A[0], C[1] - A[1]};
    double AD[2] = {D[0] - A[0], D[1] - A[1]};
    double BC[2] = {C[0] - B[0], C[1] - B[1]};
    double BD[2] = {D[0] - B[0], D[1] - B[1]};

    return (vector_product(AC, AD) * vector_product(BC, BD) <= 1e-15) &&
           (vector_product(AC, BC) * vector_product(AD, BD) <= 1e-15);
}

struct Point {
    double x, y;
};

struct Edge {
    int from, to;
};

__global__ void recasting_get_drawPixel(const Point *d_points_data,
                                        const Point *d_centers_data,
                                        const Edge *d_edges_index_data,
                                        int *d_draw_pixel_mesh,
                                        const int pixel_num, const int point_num) {

    size_t i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < pixel_num) {
        double x = d_centers_data[i].x;
        double y = d_centers_data[i].y;

        // 使用过像素中心点且平行于x轴的射线作为判断
        double line_point1[2] = {x, y};
        double line_point2[2] = {d_end_w_coordinate, y};

        int count = 0; // 与边的相交次数

        for (int j = 0; j < point_num; ++j) {
            // 边的两个端点
            double edge_point1[2] = {d_points_data[d_edges_index_data[j].from].x, // x1
                                     d_points_data[d_edges_index_data[j].from].y}; // y1
            double edge_point2[2] = {d_points_data[d_edges_index_data[j].to].x, // x2
                                     d_points_data[d_edges_index_data[j].to].y}; // y2

            if (check_intersect(line_point1, line_point2, edge_point1, edge_point2)) count++;
        }

        if (count & 1) atomicExch(&d_draw_pixel_mesh[i], 1);
    }
}

const char *default_file = "..//res//bigpolygon.wkt";

cmdLineParameter<char *> In("in");
cmdLineParameter<double> pixelSize("ps");
cmdLineParameter<int> threadsNum("num");
cmdLineReadable *params[] = {
        &In, &threadsNum, &pixelSize
};

int main(int argc, char **argv) {

    if (argc > 1) cmdLineParse(argc - 1, &argv[1], params);
    const char *filename = In.set ? In.value : default_file;
    LOG("file = ", std::string(filename), INFO);
    const double pixel_size = pixelSize.set ? pixelSize.value : 0.05;
    LOG("pixel_size = ", std::to_string(pixel_size), INFO);

    // prepare data on host
    PointData pointData = PointData(filename);
    Pixel pixel = Pixel(pixel_size, &pointData);
    std::vector<Eigen::MatrixXd> pixel_mesh = pixel.get_pixel_mesh();

    Eigen::MatrixXd points_data = pointData.get_points_data();
    int point_num = points_data.rows();
    Point *h_points = (Point *) malloc(sizeof(Point) * point_num);
    for (int i = 0; i < points_data.rows(); ++i) {
        h_points[i].x = points_data.row(i)[0];
        h_points[i].y = points_data.row(i)[1];
    }

    Eigen::MatrixXi edges_index_data = pointData.get_edges_index_data(); // 所有边的两个端点的索引
    Edge *h_edges_index = (Edge *) malloc(sizeof(Edge) * edges_index_data.rows());
    for (int i = 0; i < edges_index_data.rows(); ++i) {
        h_edges_index[i].from = edges_index_data.row(i)[0];
        h_edges_index[i].to = edges_index_data.row(i)[1];
    }

    double end_w_coordinate = pixel.get_end_w_coordinate();

    Eigen::MatrixXd centers_data = pixel.get_center();
    int pixel_num = centers_data.rows();
    LOG("pixel num = ", std::to_string(pixel_num), INFO);
    Point *h_centers = (Point *) malloc(sizeof(Point) * pixel_num);
    for (int i = 0; i < centers_data.rows(); ++i) {
        h_centers[i].x = centers_data.row(i)[0];
        h_centers[i].y = centers_data.row(i)[1];
    }

    int *h_draw_pixel_mesh = (int *) malloc(sizeof(int) * pixel_num); // 待绘制的像素网格

    // initialize device data
    Point *d_points;
    Point *d_centers;
    Edge *d_edges_index;
    int *d_draw_pixel_mesh;
    CUDA_CHECK(cudaMalloc((void **) &d_points, sizeof(Point) * point_num));
    CUDA_CHECK(cudaMalloc((void **) &d_centers, sizeof(Point) * pixel_num));
    CUDA_CHECK(cudaMalloc((void **) &d_edges_index, sizeof(Edge) * edges_index_data.rows()));
    CUDA_CHECK(cudaMalloc((void **) &d_draw_pixel_mesh, sizeof(int) * pixel_num));

    // copy data from host to device
    CUDA_CHECK(cudaMemcpy(d_points, h_points, sizeof(Point) * point_num, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_centers, h_centers, sizeof(Point) * pixel_num, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_edges_index, h_edges_index, sizeof(Edge) * edges_index_data.rows(),
                          cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpyToSymbol(d_end_w_coordinate, &end_w_coordinate, sizeof(double)));

    //
    const int block_size = 1024;
    LOG("block_size = ", std::to_string(block_size), INFO);
    const int block_i = (pixel_num + block_size - 1) / block_size;
    dim3 blockNum(block_i);
    dim3 threadsPerBlock(block_size);

    Timer timer("--------CUDA test--------");
    recasting_get_drawPixel<<<blockNum, threadsPerBlock>>>(d_points, d_centers, d_edges_index, d_draw_pixel_mesh,
                                                           pixel_num, point_num);
    cudaDeviceSynchronize();
    timer.log();

    // copy result from device to host
    CUDA_CHECK(cudaMemcpy(h_draw_pixel_mesh, d_draw_pixel_mesh, sizeof(int) * pixel_num, cudaMemcpyDeviceToHost));

    // draw_pixel
    std::vector<Eigen::MatrixXd> draw_pixel_mesh;
    for (int i = 0; i < pixel_num; ++i) {
        if (h_draw_pixel_mesh[i]) {
            draw_pixel_mesh.emplace_back(pixel_mesh[i]);
//            std::cout << pixel_mesh[i] << std::endl;
        }
    }
    LOG("draw pixel nums = ", std::to_string(draw_pixel_mesh.size()), INFO);
    pixel.set_draw_pixel_mesh(draw_pixel_mesh);

    // free pointers on host
    free(h_points);
    free(h_centers);
    free(h_edges_index);
    free(h_draw_pixel_mesh);

    // free pointers on device
    CUDA_CHECK(cudaFree(d_points));
    CUDA_CHECK(cudaFree(d_edges_index));
    CUDA_CHECK(cudaFree(d_centers));
    CUDA_CHECK(cudaFree(d_draw_pixel_mesh));

    /*igl::opengl::glfw::MyViewer myViewer = igl::opengl::glfw::MyViewer(&pixel);
    Eigen::MatrixXd infoLog(4, 3);
    infoLog << 0.5, 0.5, 0,
            0.5, 0.5, 0,
            0.5, 0.5, 0,
            0.5, 0.5, 0;

    std::vector<Eigen::RowVector3d> color_vec;
    color_vec.emplace_back(Eigen::RowVector3d(1, 0, 0));
    color_vec.emplace_back(Eigen::RowVector3d(0, 1, 0));
    color_vec.emplace_back(Eigen::RowVector3d(0, 0, 1));
    std::vector<float> bb_point_size;
    bb_point_size.emplace_back(2.5);
    bb_point_size.emplace_back(1e-9);

    myViewer.set_data(color_vec);
//    myViewer.draw_point_and_bb(bb_point_size);
    myViewer.draw_pixel(infoLog);
    myViewer.launch();*/

    return EXIT_SUCCESS;
}