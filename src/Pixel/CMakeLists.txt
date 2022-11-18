add_library(pixel_data "")

target_sources(pixel_data
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pixelData.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/pixelData.h
        )

target_include_directories(pixel_data
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/src/Point
        )
