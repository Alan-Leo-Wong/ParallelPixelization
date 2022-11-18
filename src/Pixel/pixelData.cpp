//
// Created by lei on 22-10-15.
//

#include "pixelData.h"

void Pixel::fill_center() {
    std::vector<double> center_w; // 保存x维度的所有像素网格的中心点坐标
    std::vector<double> center_h; // 保存y维度的所有像素网格的中心点坐标

    double pixel_coordinate = start_w_coordinate;
    while (pixel_coordinate <= end_w_coordinate) {
        double temp = std::fmin(pixel_size, end_w_coordinate - pixel_coordinate) / 2.0;
        center_w.emplace_back(pixel_coordinate + temp);
        pixel_coordinate += pixel_size;
    }

    pixel_coordinate = start_h_coordinate;
    while (pixel_coordinate <= end_h_coordinate) {
        double temp = std::fmin(pixel_size, end_h_coordinate - pixel_coordinate) / 2.0;
        center_h.emplace_back(pixel_coordinate + temp);
        pixel_coordinate += pixel_size;
    }

    center.resize(center_w.size() * center_h.size(), 3);
    long index = 0;
    for (auto cw: center_w) {
        for (auto ch: center_h) {
            center.row(index) = Eigen::RowVector3d(cw, ch, 0);
            ++index;
        }
    }
}

void Pixel::fill_pixel_mesh() {
    for (int i = 0; i < center.rows(); i++) {
        // 像素中心点的坐标
        double x = center(i, 0);
        double y = center(i, 1);

        double x_t = std::fmin(pixel_size / 2.0, end_w_coordinate - x);
        double y_t = std::fmin(pixel_size / 2.0, end_h_coordinate - y);

        // 像素网格的左上方，左下角，右下角和右上方
        Eigen::Vector3d pixel_lu_coordinate, pixel_lb_coordinate, pixel_rb_coordinate, pixel_ru_coordinate;

        pixel_lu_coordinate = Eigen::Vector3d(x - x_t, y + y_t, 0);
        pixel_lb_coordinate = Eigen::Vector3d(x - x_t, y - y_t, 0);
        pixel_rb_coordinate = Eigen::Vector3d(x + x_t, y - y_t, 0);
        pixel_ru_coordinate = Eigen::Vector3d(x + x_t, y + y_t, 0);

        Eigen::MatrixXd pm(4, 3);
        pm.row(0) = pixel_lb_coordinate;
        pm.row(1) = pixel_rb_coordinate;
        pm.row(2) = pixel_ru_coordinate;
        pm.row(3) = pixel_lu_coordinate;

        pixel_mesh.emplace_back(pm);
    }
}

double vector_product(const Eigen::Vector2d &vectorA, const Eigen::Vector2d &vectorB) {
    /*计算 x_1 * y_2 - x_2 * y_1*/
    return vectorA(0) * vectorB(1) - vectorB(0) * vectorA(1);
}

// 判断两条(二维，虽然传的是三维但是z轴都为0)线段是否相交
// AB代表一条线段，CD代表一条线段
bool Pixel::check_intersect(const Eigen::Vector2d &A,
                            const Eigen::Vector2d &B,
                            const Eigen::Vector2d &C,
                            const Eigen::Vector2d &D) {
    /*std::cout << "A: " << A << std::endl
              << "B: " << B << std::endl
              << "C: " << C << std::endl
              << "D: " << D << std::endl;
    std::cout << "------------------------------\n";*/
    Eigen::Vector2d AC = C - A;
    Eigen::Vector2d AD = D - A;
    Eigen::Vector2d BC = C - B;
    Eigen::Vector2d BD = D - B;

    return (vector_product(AC, AD) * vector_product(BC, BD) <= 1e-9) &&
           (vector_product(AC, BC) * vector_product(AD, BD) <= 1e-9);
}

void Pixel::recasting_get_drawPixel() {
    Eigen::MatrixXd points_data = pd->get_points_data();
    Eigen::MatrixXi edges_index_data = pd->get_edges_index_data();

//    std::cout << "center.rows: " << center.rows() << std::endl;
    for (int i = 0; i < center.rows(); ++i) {
        double x = center(i, 0);
        double y = center(i, 1);
        // 使用过像素中心点且平行于x轴的射线作为判断
        // 当然这条射线可以转化为像素中心点与bounding-box的右边界点构成的线段
//        Eigen::Vector3d line = Eigen::Vector3d(end_w_coordinate - x, 0, 0);
        Eigen::Vector2d line_point1 = Eigen::Vector2d(x, y);
        Eigen::Vector2d line_point2 = Eigen::Vector2d(end_w_coordinate, y);

        int count = 0; // 与边的相交次数
        for (int j = 0; j < edges_index_data.rows(); ++j) {
            // 边的两条端点
            Eigen::Vector2d edge_point1 = points_data.row(edges_index_data(j, 0)).block<1, 2>(0, 0);
            Eigen::Vector2d edge_point2 = points_data.row(edges_index_data(j, 1)).block<1, 2>(0, 0);
            if (check_intersect(line_point1, line_point2, edge_point1, edge_point2)) count++;
        }
//        std::cout << "count: " << count << std::endl;
        if (count & 1) {
            draw_pixel_mesh.emplace_back(pixel_mesh[i]);
        }
    }
}

void Pixel::recasting_get_drawPixel(const GPIXEL &gp) {
    Eigen::MatrixXd points_data = pd->get_points_data();
    Eigen::MatrixXi edges_index_data = pd->get_edges_index_data();

    for (int i = 0; i < gp.size(); ++i) {
        double x = gp[i].first(0);
        double y = gp[i].first(1);
        // 使用过像素中心点且平行于x轴的射线作为判断
        // 当然这条射线可以转化为像素中心点与bounding-box的右边界点构成的线段
        Eigen::Vector2d line_point1 = Eigen::Vector2d(x, y);
        Eigen::Vector2d line_point2 = Eigen::Vector2d(end_w_coordinate, y);

        int count = 0; // 与边的相交次数
        for (int j = 0; j < edges_index_data.rows(); ++j) {
            // 边的两条端点
            Eigen::Vector2d edge_point1 = points_data.row(edges_index_data(j, 0)).block<1, 2>(0, 0);
            Eigen::Vector2d edge_point2 = points_data.row(edges_index_data(j, 1)).block<1, 2>(0, 0);
            if (check_intersect(line_point1, line_point2, edge_point1, edge_point2)) count++;
        }
//        std::cout << "count: " << count << std::endl;
        if (count & 1) {
            draw_pixel_mesh.emplace_back(pixel_mesh[gp[i].second]);
        }
    }
}

int Pixel::mid_search(const std::vector<double> &query_vector, const double &query_value) {
    int vec_size = static_cast<int>(query_vector.size());
    int left = 0, right = vec_size - 1;
    int result = right;

    while (left <= right) {
        int mid = (left + right) / 2;
        if (query_vector[mid] >= query_value) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    if (query_value <= query_vector[result]) return result;
    return -1;
}
