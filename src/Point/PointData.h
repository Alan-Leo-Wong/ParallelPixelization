//
// Created by lei on 22-10-15.
//

#ifndef GPUPROGRAM_POINTDATA_H
#define GPUPROGRAM_POINTDATA_H

#include <Eigen/Dense>
#include <utility>

class PointData {
private:
    Eigen::MatrixXd points_data;
    Eigen::MatrixXi edges_index_data;
    Eigen::Vector4d bb_limit;
    Eigen::MatrixXd bb_point;
    const char* filename;
public:
    PointData(const char* file) : filename(file) {
        fill_points_data();
        fill_edges_index_data();
        get_boundingbox();
    }

    Eigen::MatrixXd get_points_data() const { return points_data; }

    Eigen::MatrixXi get_edges_index_data() const { return edges_index_data; }

    Eigen::Vector4d get_bb_limit() const { return bb_limit; }

    Eigen::MatrixXd get_bb_point() const { return bb_point; }

    bool readFile(char *&file_data, int &file_length);

    void fill_points_data();

    void fill_edges_index_data(); // 保存所有边两端点索引的数据

    void get_boundingbox();
};


#endif //GPUPROGRAM_POINTDATA_H
