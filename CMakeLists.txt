cmake_minimum_required(VERSION 3.20)
project(OpenGLProject)

# 1. Set C++ Standard
set(CMAKE_CXX_STANDARD 17)

# 2. Find GLFW (Homebrew path)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(glfw3 REQUIRED)

# 3. Add GLAD as a separate library
add_library(glad STATIC
        extern/src/glad.c
)
target_include_directories(glad PUBLIC extern/include)

# 4. Define the executable
add_executable(learnOpenGL
        main.cpp
        shader/shader.h
)

# 5. Include directories for your main executable
target_include_directories(learnOpenGL PRIVATE extern/include)

# 6. Copy shader files post-build
add_custom_command(
        TARGET learnOpenGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader $<TARGET_FILE_DIR:learnOpenGL>/shader
)

# 7. Link libraries
target_link_libraries(learnOpenGL PRIVATE
        glad
        glfw
)

# 8. Mac-specific: Link required system frameworks
if (APPLE)
    target_link_libraries(learnOpenGL PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()
