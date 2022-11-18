//
// Created by lei on 22-10-15.
//

#ifndef GPUPROGRAM_MYVIEWER_H
#define GPUPROGRAM_MYVIEWER_H

#include "igl/opengl/glfw/Viewer.h"

#include <utility>
#include "igl/opengl/glfw/imgui/ImGuiPlugin.h"
#include "igl/opengl/glfw/imgui/ImGuiMenu.h"
#include "../Point/PointData.h"
#include "../Pixel/pixelData.h"

namespace igl {
    namespace opengl {
        namespace glfw {

            /*namespace draw {
                float f;

                bool draw_menu() {
                    if (ImGui::CollapsingHeader("Group1", ImGuiTreeNodeFlags_DefaultOpen)) {
                        ImGui::SliderFloat("pixel size", &f, 0.1f, 10.0f, "%.1f");
                    }
                }
            }*/

            class MyViewer : public Viewer {
            private:
                imgui::ImGuiPlugin plugin;
                imgui::ImGuiMenu menu;
                Pixel *px;
                Eigen::MatrixXd pixel_center;
            public:
                MyViewer(Pixel *p) : px(p) {
                    /*this->plugins.emplace_back(&plugin);
                    menu.callback_draw_viewer_menu = draw::draw_menu;
                    plugin.widgets.emplace_back(&menu);*/
                };

                void set_data(const std::vector<Eigen::RowVector3d> &color_vec);

                // 绘制point和bounding-box
                void draw_point_and_bb(const std::vector<float> &point_size);

                void draw_cube(int data_index, const Eigen::RowVector3d& edge_color);

                // 绘制像素网格
                void draw_pixel(const Eigen::MatrixXd &color);
            };
        }
    }
}

#endif //GPUPROGRAM_MYVIEWER_H
