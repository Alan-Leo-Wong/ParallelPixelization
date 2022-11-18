add_library(visual "")

target_sources(visual
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/myViewer.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/myViewer.h
        )

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Libigl
include(libigl)

# enable the target igl::glfw
igl_include(glfw)
igl_include(imgui)

target_link_libraries(visual PUBLIC igl::glfw igl::imgui)

target_include_directories(visual
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/src/Point
        ${CMAKE_SOURCE_DIR}/src/Pixel
        )
