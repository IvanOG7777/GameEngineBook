cmake_minimum_required(VERSION 3.20)
project(FireWorksTest)

set(CMAKE_CXX_STANDARD 14)

# Repo root: .../GameEngineBook
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# --- Your executable ---
add_executable(FireWorksTest
        main.cpp
        Source/firework.cpp
        Source/objects.cpp
        Source/windowFunctions.cpp

        # Your shared engine files
        ${ROOT_DIR}/gameLib/Particle.cpp

        ${ROOT_DIR}/Libraries/src/glad.c
)

# --- Include folders (headers) ---
target_include_directories(FireWorksTest PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}          # FireWorksTest/
        ${ROOT_DIR}/gameLib                # Particle.h, core.h, etc.

        ${ROOT_DIR}/Libraries/include      # glad/..., GLFW/...
)

# --- Library folder (contains glfw3.lib or libglfw3.a) ---
target_link_directories(FireWorksTest PRIVATE
        ${ROOT_DIR}/Libraries/lib
)

# --- Link libs ---
# Windows OpenGL is opengl32
target_link_libraries(FireWorksTest PRIVATE
        glfw3
        opengl32
)
