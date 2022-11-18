add_library(point_data "")

target_sources(point_data
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/PointData.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/PointData.h
        )

target_include_directories(point_data
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

find_package(Eigen3 REQUIRED)
if (TARGET Eigen3::Eigen)
    target_link_libraries(point_data PUBLIC Eigen3::Eigen)
endif ()
