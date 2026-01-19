cmake_minimum_required(VERSION 3.20)
project(OpenGLProject)

# 1. Set C++ Standard
set(CMAKE_CXX_STANDARD 17)

# 2. Find GLFW (Homebrew path)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(glfw3 REQUIRED)

# 3. Tell CMake where to look for your include folders (GLAD and KHR)
include_directories(extern/include)

# 4. Define the executable
# Note: Removed the duplicate main.cpp
add_executable(learnOpenGL
        main.cpp
        extern/src/glad.c
)

# 5. Link libraries
target_link_libraries(learnOpenGL PRIVATE glfw)

# 6. Mac-specific: Link required system frameworks
if(APPLE)
    target_link_libraries(learnOpenGL PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()