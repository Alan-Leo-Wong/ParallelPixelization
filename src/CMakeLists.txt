add_subdirectory(Point)
add_subdirectory(Pixel)
add_subdirectory(CPU)
#add_subdirectory(Visualization)
add_subdirectory(CUDA)

target_link_libraries(openmp_pixel PRIVATE point_data pixel_data)
target_link_libraries(pthread_pixel PRIVATE point_data pixel_data)
