add_library(openmp_pixel "")

target_sources(openmp_pixel
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pixelMP.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/pixelMP.h
        )

target_include_directories(openmp_pixel
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/src/Pixel
        )

find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    target_link_libraries(openmp_pixel PRIVATE OpenMP::OpenMP_CXX)
endif ()
