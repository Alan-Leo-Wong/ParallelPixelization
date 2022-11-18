//
// Created by lei on 22-10-15.
//

#include "myViewer.h"

namespace igl {
    namespace opengl {
        namespace glfw {
            void MyViewer::set_data(const std::vector<Eigen::RowVector3d> &color_vec) {
                Eigen::MatrixXd pointsData = px->get_PointData()->get_points_data();
                data_list[0].add_points(pointsData, color_vec[0]);
                this->core().align_camera_center(pointsData);

                append_mesh();
                Eigen::MatrixXd bb_point = px->get_PointData()->get_bb_point();
                data_list[1].add_points(bb_point, color_vec[1]);

                // 获取所有待绘制的像素网格顶点坐标(vector中存储的为4*3的像素网格顶点矩阵，分别对应像素网格的左上方，左下角，右下角和右上方)
                std::vector<Eigen::MatrixXd> pixel_mesh = px->get_draw_pixel_mesh();
                pixel_center.resize(pixel_mesh[0].rows(), pixel_mesh[0].cols());
                pixel_center.setZero();

//                std::cout << "data维度： " << pixel_mesh[0].rows() << " " << pixel_mesh[0].cols() << std::endl;

                Eigen::MatrixXi F;
                /*pm.row(0) = pixel_lb_coordinate;
                    pm.row(1) = pixel_rb_coordinate;
                    pm.row(2) = pixel_ru_coordinate;
                    pm.row(3) = pixel_lu_coordinate;*/
                F.resize(2, 3);
                F << 0, 1, 3,
                        1, 2, 3;

                for (int i = 0; i < pixel_mesh.size(); ++i) {
                    append_mesh();
                    data_list[2 + i].set_mesh(pixel_mesh[i], F);
                    pixel_center += pixel_mesh[i];
                }

                pixel_center /= static_cast<double>(pixel_mesh.size());
            }

            void MyViewer::draw_cube(int data_index, const Eigen::RowVector3d &edge_color) {
                Eigen::MatrixXi edge;
                edge.resize(4, 2);
                edge <<
                     0, 1,
                        0, 3,
                        1, 2,
                        2, 3;
                for (int i = 0; i < 4; ++i) {
                    data_list[data_index].add_edges(
                            data_list[data_index].points.block<1, 3>(edge(i, 0), 0),
                            data_list[data_index].points.block<1, 3>(edge(i, 1), 0),
                            edge_color
                    );
                }
            }

            void MyViewer::draw_point_and_bb(const std::vector<float> &point_size) {
                data_list[0].set_visible(true);
                data_list[1].set_visible(true);
                for (int i = 2; i < data_list.size(); i++)
                    data_list[i].set_visible(false);

                data_list[0].point_size = point_size[0];
                data_list[1].point_size = point_size[1];

                // 绘制bounding-box
                draw_cube(1, Eigen::RowVector3d(0.5, 0.5, 0.5));
            }

            void MyViewer::draw_pixel(const Eigen::MatrixXd &color) {
                data_list[0].set_visible(false);
                data_list[1].set_visible(false);

                this->core().camera_zoom = 0.01;
                for (int i = 2; i < data_list.size(); ++i) {
                    data_list[i].set_visible(true);
//                    std::cout << data_list[i].points << std::endl << "----\n";
                    data_list[i].set_colors(color);
                    data_list[i].show_lines = false;
                }
            }
        }
    }
}