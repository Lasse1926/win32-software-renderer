cmake_minimum_required(VERSION 3.23)

project(c_window)

add_executable(c_window)

target_sources(c_window
  PRIVATE
    main.c
    src/camera.c
    src/scene.c
    src/transform.c
    src/vector_utils.c
    src/geometry_utils.c
    src/math_utils.c
)

target_include_directories(c_window
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)
