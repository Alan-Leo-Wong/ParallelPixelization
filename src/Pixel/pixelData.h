//
// Created by lei on 22-10-15.
//

#ifndef GPUPROGRAM_PIXELDATA_H
#define GPUPROGRAM_PIXELDATA_H

#include <utility>
#include <vector>
#include <Eigen/Dense>
#include "../Point/PointData.h"

typedef std::vector<std::pair<Eigen::MatrixXd, int>> GPIXEL; // 一组pixel

class Pixel {
private:
    double pixel_size;
    // 对应于bounding-box的四个值
    double start_w_coordinate, end_w_coordinate;
    double start_h_coordinate, end_h_coordinate;
    Eigen::MatrixXd center; // 保存所有像素网格的中心点坐标
    std::vector<Eigen::MatrixXd> pixel_mesh; // 保存所有像素网格的四个顶点的坐标，每个元素都与中心点对应
    std::vector<Eigen::MatrixXd> draw_pixel_mesh; // 保存所有待绘制的像素网格
    PointData *pd;
public:
    Pixel(double size, PointData *pointData) : pixel_size(size), pd(pointData) {
        Eigen::Vector4d bb_limit = pd->get_bb_limit();
//        std::cout << bb_limit;
        start_w_coordinate = bb_limit(0);
        end_w_coordinate = bb_limit(1);
        start_h_coordinate = bb_limit(2);
        end_h_coordinate = bb_limit(3);
        fill_center();
        fill_pixel_mesh();
//        recasting_get_drawPixel();
    }

    double get_pixel_size() const { return pixel_size; }

    double get_start_w_coordinate() const { return start_w_coordinate; }

    double get_start_h_coordinate() const { return start_h_coordinate; }

    double get_end_w_coordinate() const { return end_w_coordinate; }

    double get_end_h_coordinate() const { return end_h_coordinate; }

    PointData *get_PointData() const { return pd; }

    Eigen::MatrixXd get_center() const { return center; }

    std::vector<Eigen::MatrixXd> get_pixel_mesh() const { return pixel_mesh; }

    std::vector<Eigen::MatrixXd> get_draw_pixel_mesh() const { return draw_pixel_mesh; }

    void set_draw_pixel_mesh(std::vector<Eigen::MatrixXd> d_pixel_mesh) { draw_pixel_mesh = std::move(d_pixel_mesh); }

    // 填充保存所有像素中心点的数组
    void fill_center();

    // 填充所有像素网格坐标的数组
    void fill_pixel_mesh();

    // 光线追踪判断每个像素点与数据中所有线段的位置，并记录待绘制的像素网格（在内部即绘制）
    void recasting_get_drawPixel();

    void recasting_get_drawPixel(const std::vector<std::pair<Eigen::MatrixXd, int>> &gp);

    // 从query_vector中找到第一个大于等于query_value的数的下标，用于确定某个点在哪个像素里面
    int mid_search(const std::vector<double> &query_vector, const double &query_value);

    ~Pixel() {
        pd = nullptr;
    }

    static bool
    check_intersect(const Eigen::Vector2d &A, const Eigen::Vector2d &B, const Eigen::Vector2d &C,
                    const Eigen::Vector2d &D);

};


#endif //GPUPROGRAM_PIXELDATA_H
