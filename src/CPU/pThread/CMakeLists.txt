add_library(pthread_pixel "")

target_sources(pthread_pixel
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pixelThread.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/pixelThread.h
        )

target_include_directories(pthread_pixel
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/src/Pixel
        )
