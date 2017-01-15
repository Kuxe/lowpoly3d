cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

include(ExternalProject)

ExternalProject_Add(glm_project
	GIT_REPOSITORY https://github.com/g-truc/glm
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	PREFIX ${CMAKE_SOURCE_DIR}/contrib
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

ExternalProject_Add(glfw_project
	GIT_REPOSITORY https://github.com/glfw/glfw
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	PREFIX ${CMAKE_SOURCE_DIR}/contrib
	CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
	INSTALL_COMMAND ""
)

ExternalProject_Add(glbinding_project
	GIT_REPOSITORY https://github.com/cginternals/glbinding
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	PREFIX ${CMAKE_SOURCE_DIR}/contrib
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DOPTION_BUILD_TESTS=OFF
	INSTALL_COMMAND ""
)

ExternalProject_Add(subber_project
	GIT_REPOSITORY https://github.com/Kuxe/subber
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	PREFIX ${CMAKE_SOURCE_DIR}/contrib
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DOPTION_BUILD_TESTS=OFF
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_dependencies(glbinding_project glfw_project)

link_directories(${CMAKE_SOURCE_DIR}/contrib/src/glfw_project/src)
link_directories(${CMAKE_SOURCE_DIR}/contrib/src/glbinding_project)

project(lowpoly3d)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/contrib/src/glfw_project/include
	${CMAKE_SOURCE_DIR}/contrib/src/glfw_project/deps
	${CMAKE_SOURCE_DIR}/contrib/src/glm_project
	${CMAKE_SOURCE_DIR}/contrib/src/glbinding_project/source/glbinding/include
	${CMAKE_SOURCE_DIR}/contrib/src/subber_project
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++14")
add_executable(lowpoly3d_example 
	src/main.cpp
	src/renderer.cpp
	src/renderdata.cpp
	src/model.cpp
	src/shaderprogram.cpp
	src/camera.cpp
	src/deltatime.cpp
	src/terraingenerator.cpp
	src/cubegenerator.cpp
	src/spheregenerator.cpp
	src/dummysimulation.cpp
	src/planegenerator.cpp
	src/circlegenerator.cpp
	src/framebuffer.cpp
	src/depthframebuffer.cpp
	src/uniformbuffer.cpp
)

add_dependencies(lowpoly3d_example glm_project)
add_dependencies(lowpoly3d_example glfw_project)
add_dependencies(lowpoly3d_example glbinding_project)
add_dependencies(lowpoly3d_example subber_project)

if (WIN32)
	target_link_libraries(lowpoly3d_example glbinding glfw3 opengl32)
endif (WIN32)

if (UNIX)
    target_link_libraries(lowpoly3d_example glbinding glfw3 dl GL X11 Xxf86vm Xrandr pthread Xi Xinerama Xcursor)
endif (UNIX)
