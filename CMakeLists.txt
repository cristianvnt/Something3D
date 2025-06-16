cmake_minimum_required(VERSION 3.28)
project(Something3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automate submodules git init
execute_process(
	COMMAND git submodule update --init --recursive
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	RESULT_VARIABLE GIT_SUBMOD_RESULT
)
if(NOT GIT_SUBMOD_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to init git submodules. Try running manually: git submodule update --init --recursive")
endif()

# GLFW
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw)

# GLAD 
add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/libs/glad/include)

# GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/libs/glm)

# Third party
add_library(third_party_h INTERFACE)
target_include_directories(third_party_h INTERFACE ${CMAKE_SOURCE_DIR}/libs/third_party)

find_package(OpenGL REQUIRED)

# Get sources and create executable
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
	glfw 
	glad 
	glm 
	third_party_h
	OpenGL::GL
)

# On linux
if (UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread)
endif()

# Runtime resources
file(COPY
	${CMAKE_SOURCE_DIR}/resources
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)