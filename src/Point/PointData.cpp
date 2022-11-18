//
// Created by lei on 22-10-15.
//

#include "PointData.h"
#include <vector>
#include <iostream>
#include <fstream>

bool PointData::readFile(char *&file_data, int &file_length) {
    std::ifstream in(filename);
    in.seekg(-3, std::ifstream::end); // remove "))"
    file_length = static_cast<int>(in.tellg()) - 9;
    if (file_length > 0) {
        file_data = new char[file_length];
        in.seekg(9, std::ifstream::beg); // remove "POLYGON(("
        in.read(file_data, file_length);
        in.close();
        return true;
    }
    in.close();
    return false;
}

void PointData::fill_points_data() {
    char *file_data;
    int file_length;
    if (!readFile(file_data, file_length)) {
        std::cerr << "Failed to open \"" << filename << "\" ! Please check your filename." << std::endl;
        exit(1);
    }

    std::stringstream ss_all(file_data);
    std::string temp;

    std::vector<std::pair<double, double> > vec;
    while (std::getline(ss_all, temp, ',')) {
        std::stringstream ss_temp(temp);
        double t1, t2;
        ss_temp >> t1 >> t2;
        vec.emplace_back(std::make_pair(t1, t2));
    }

    points_data.resize(static_cast<int>(vec.size()), 3);

    int row_num = 0;
    for (auto &p: vec) {
        /*std::cout << std::fixed << std::setprecision(6) << p.first << " " << p.second << std::endl;*/
        points_data.row(row_num) = Eigen::RowVector3d(p.first, p.second, 0);
        row_num++;
    }
}

void PointData::fill_edges_index_data() {
    edges_index_data.resize(points_data.rows(), 2);
    int rows = points_data.rows();
    for (int i = 0; i < rows; ++i) {
        edges_index_data(i, 0) = i;
        edges_index_data(i, 1) = (i + 1) % rows;
    }
}

void PointData::get_boundingbox() {
    Eigen::Vector3d m = points_data.colwise().minCoeff();
    Eigen::Vector3d M = points_data.colwise().maxCoeff();
    bb_limit = Eigen::Vector4d(m(0), M(0), m(1), M(1));

    bb_point.resize(4, 3);
    bb_point << m(0), M(1), 0,
            m(0), m(1), 0,
            M(0), m(1), 0,
            M(0), M(1), 0;
}

