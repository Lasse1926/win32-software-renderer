cmake_minimum_required(VERSION 3.23)

project(win32-software-renderer)

add_executable(win32-software-renderer)

target_sources(win32-software-renderer
  PRIVATE
    main.c
    src/camera.c
    src/scene.c
    src/transform.c
    src/vector_utils.c
    src/geometry_utils.c
    src/math_utils.c
    src/quaternion.c
    src/model_loader.c
    src/file_utils.c
)

target_include_directories(win32-software-renderer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)
