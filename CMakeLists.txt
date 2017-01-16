cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

include(ExternalProject)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# -------------------- DOWNLOAD DEPENDICIES AUTOMAGICALLY -------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
SET(CONTRIB_DIR "${CMAKE_SOURCE_DIR}/contrib")
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_BASE "${CONTRIB_DIR}")

ExternalProject_Add(glm_project
	GIT_REPOSITORY https://github.com/g-truc/glm
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

ExternalProject_Add(glfw_project
	GIT_REPOSITORY https://github.com/glfw/glfw
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
	INSTALL_COMMAND ""
)

ExternalProject_Add(glbinding_project
	GIT_REPOSITORY https://github.com/cginternals/glbinding
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DOPTION_BUILD_TESTS=OFF
	INSTALL_COMMAND ""
)

ExternalProject_Add(subber_project
	GIT_REPOSITORY https://github.com/Kuxe/subber
	UPDATE_DISCONNECTED 1
	BUILD_IN_SOURCE 1
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DOPTION_BUILD_TESTS=OFF
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_dependencies(glbinding_project glfw_project)

link_directories(${CONTRIB_DIR}/Source/glfw_project/src)
link_directories(${CONTRIB_DIR}/Source/glbinding_project)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ------------------------- SETUP lowpoly3d PROJECT -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
project(lowpoly3d)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CONTRIB_DIR}/Source/glfw_project/include
	${CONTRIB_DIR}/Source/glfw_project/deps
	${CONTRIB_DIR}/Source/glm_project
	${CONTRIB_DIR}/Source/glbinding_project/source/glbinding/include
	${CONTRIB_DIR}/Source/subber_project
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++14")

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ------------------------- SETUP playground TARGET -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
add_executable(lowpoly3d-playground 
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

add_dependencies(lowpoly3d-playground  glm_project)
add_dependencies(lowpoly3d-playground  glfw_project)
add_dependencies(lowpoly3d-playground  glbinding_project)
add_dependencies(lowpoly3d-playground  subber_project)

if (WIN32)
	target_link_libraries(lowpoly3d-playground glbinding glfw3 opengl32)
endif (WIN32)

if (UNIX)
    target_link_libraries(lowpoly3d-playground glbinding glfw3 dl GL X11 Xxf86vm Xrandr pthread Xi Xinerama Xcursor)
endif (UNIX)

#Run "make playground" to build the playground
ADD_CUSTOM_TARGET(playground
    DEPENDS lowpoly3d-playground
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP example TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #

add_executable(lowpoly3d-example 
	src/renderer.cpp
	src/renderdata.cpp
	src/model.cpp
	src/shaderprogram.cpp
	src/camera.cpp
	src/deltatime.cpp
	src/terraingenerator.cpp
	src/cubegenerator.cpp
	src/spheregenerator.cpp
	src/example.cpp
	src/framebuffer.cpp
	src/depthframebuffer.cpp
	src/uniformbuffer.cpp
)

if (WIN32)
	target_link_libraries(lowpoly3d-example glbinding glfw3 opengl32)
endif (WIN32)

if (UNIX)
    target_link_libraries(lowpoly3d-example glbinding glfw3 dl GL X11 Xxf86vm Xrandr pthread Xi Xinerama Xcursor)
endif (UNIX)

#Run "make example" to build the example
ADD_CUSTOM_TARGET(example
    DEPENDS lowpoly3d-example
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
