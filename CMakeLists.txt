cmake_minimum_required(VERSION 3.20)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

add_library(glad STATIC
        extern/src/glad.c
)

target_include_directories(glad PUBLIC extern/include)

add_executable(learnOpenGL
        main.cpp
		extern/include/libs/stb_image.cpp
        shader/shader.h
)

target_include_directories(learnOpenGL PRIVATE extern/include)

add_custom_command(
        TARGET learnOpenGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader $<TARGET_FILE_DIR:learnOpenGL>/shader
		${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:learnOpenGL>/assets
)

target_link_libraries(learnOpenGL PRIVATE
        glad
        glfw
)

if (APPLE)
    target_link_libraries(learnOpenGL PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()
