cmake_minimum_required(VERSION 3.28)
project(Something3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLAD 
add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

# GLFW
add_subdirectory(libs/glfw)

# Req
find_package(OpenGL REQUIRED)

# Exec
file(GLOB SOURCES "src/*.cpp")
add_executable(Something3D ${SOURCES})

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Something3D PRIVATE glad glfw ${OPENGL_LIBRARIES})

# On linux
if (UNIX)
	target_link_libraries(Something3D PRIVATE dl pthread)
endif()