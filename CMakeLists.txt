cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(lowpoly3d
	VERSION 0.0.2
	LANGUAGES CXX
	HOMEPAGE_URL https://github.com/Kuxe/lowpoly3d
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(FetchContent)
include(buildcache)

configure_file(build.hpp.in build.hpp @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# [WIN32] All projects should output dlls to the binary directory.
# This is because .exe-files look for .dlls in its own directory,
# so if .exe-files should be runnable from the binary directory
# then all its dependencies must exist in the binary directory.
if(WIN32)
	# Windows does not have concept of RUNPATHs.
	# So build all dlls in same directory.
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------- GENERATE .gitignore in build directory--------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# -------------------- DOWNLOAD DEPENDICIES AUTOMAGICALLY -------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #

SET(CONTRIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib")

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.0.0-preview4
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
	glbinding
	GIT_REPOSITORY https://github.com/cginternals/glbinding.git
	GIT_TAG 70e76f9
)
set(OPTION_BUILD_TOOLS OFF CACHE INTERNAL "")
set(OPTION_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(glbinding)
if(WIN32)
	set_target_properties(glbinding PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	set_target_properties(glbinding-aux PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
endif()

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.6
)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
	lsys
	GIT_REPOSITORY https://github.com/Kuxe/lsys.git
	GIT_TAG 201438c
)
set(lsys_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(lsys)

file(
	DOWNLOAD https://raw.githubusercontent.com/Kuxe/subber/f0565d1dea10633ed04b5a1df1164f5f5915a40b/subber.hpp
	${CONTRIB_DIR}/subber.hpp
)

file(
	DOWNLOAD https://gist.githubusercontent.com/Kuxe/6bdd5b748b5f11b303a5cccbb8c8a731/raw/fd2865201f77a2dac29a010d2cdc5f79ffc4aff4/queue.hpp
	${CONTRIB_DIR}/queue.hpp
)

file(
	DOWNLOAD https://gist.githubusercontent.com/Kuxe/6bdd5b748b5f11b303a5cccbb8c8a731/raw/fd2865201f77a2dac29a010d2cdc5f79ffc4aff4/semaphore.hpp
	${CONTRIB_DIR}/semaphore.hpp
)

file(
	DOWNLOAD https://gist.githubusercontent.com/Kuxe/82d69f65b6567d516963160099eccd92/raw/d9387964d78691144c1b4ca201e2f11ceb72af3a/node.hpp
	${CONTRIB_DIR}/node.hpp
)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ------------------------- SETUP lowpoly3d PROJECT -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# Thanks to https://stackoverflow.com/a/34317588
if(UNIX)
	find_program(CCACHE_EXECUTABLE ccache)
	mark_as_advanced(CCACHE_EXECUTABLE)
	if(CCACHE_EXECUTABLE)
		foreach(LANG C CXX)
			if(NOT DEFINED CMAKE_${LANG}_COMPILER_LAUNCHER AND NOT CMAKE_${LANG}_COMPILER MATCHES ".*/ccache")
				message(STATUS "Enabling ccache for ${LANG}")
				set(CMAKE_${LANG}_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE} CACHE STRING "")
			endif()
		endforeach()
	endif()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #Create a compilation database for use with clang static analyzer

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# -------------------------- SETUP lowpoly3d TARGET -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
add_library(${PROJECT_NAME} SHARED
	include/args.hxx
	include/bezier.hpp
	include/bounding_volume_hierarchy.hpp src/bounding_volume_hierarchy.cpp
	include/celestialbody.hpp
	include/camera.hpp src/camera.cpp
	include/draw_geometric_primitives.hpp src/draw_geometric_primitives.cpp
	include/events.hpp
	include/debugrenderer.hpp src/debugrenderer.cpp
	include/depthframebuffer.hpp src/depthframebuffer.cpp
	include/fps_camera.hpp src/fps_camera.cpp
	include/framebuffer.hpp src/framebuffer.cpp
	include/gamedata.hpp
	include/ilowpolyinput.hpp
	include/keyaction.hpp src/keyaction.cpp
	include/keymanager.hpp src/keymanager.cpp
	include/lowpoly3d.hpp
	include/model.hpp src/model.cpp
	include/modeldefs.hpp
	include/modeluniformdata.hpp
	include/mvpuniformdata.hpp
	include/perlin.hpp src/perlin.cpp
	include/renderer.hpp src/renderer.cpp
	include/renderdata.hpp src/renderdata.cpp
	include/scene.hpp src/scene.cpp
	include/shaderprogram.hpp src/shaderprogram.cpp
	include/uniformbuffer.hpp src/uniformbuffer.cpp
	include/glframe.hpp src/glframe.cpp
	include/worlduniformdata.hpp
	include/minimum_bounding_sphere.hpp src/minimum_bounding_sphere.cpp

	include/generators/circlegenerator.hpp src/generators/circlegenerator.cpp
	include/generators/cubegenerator.hpp src/generators/cubegenerator.cpp
	include/generators/cylindergenerator.hpp src/generators/cylindergenerator.cpp
	include/generators/modelgenerator.hpp
	include/generators/planegenerator.hpp src/generators/planegenerator.cpp
	include/generators/spheregenerator.hpp src/generators/spheregenerator.cpp
	include/generators/terraingenerator.hpp src/generators/terraingenerator.cpp
	include/generators/treegenerator.hpp

	include/geometric_primitives/cylinder.hpp
	include/geometric_primitives/direction.hpp
	include/geometric_primitives/intersections.hpp src/geometric_primitives/intersections.cpp
	include/geometric_primitives/intersects.hpp src/geometric_primitives/intersects.cpp
	include/geometric_primitives/line.hpp
	include/geometric_primitives/linesegment.hpp
	include/geometric_primitives/plane.hpp
	include/geometric_primitives/point.hpp
	include/geometric_primitives/rectangle.hpp
	include/geometric_primitives/sphere.hpp src/geometric_primitives/sphere.cpp
	include/geometric_primitives/triangle.hpp src/geometric_primitives/triangle.cpp

	include/utils/apt_assert.hpp
	include/utils/arithmetic_invariant.hpp
	include/utils/glm/are_parallel.hpp
	include/utils/glm/glmprint.hpp
	include/utils/glm/glmutils.hpp
	include/utils/glm/vector_projection.hpp
	include/utils/lerp.hpp
	include/utils/misc.hpp
	include/utils/no_such_triangle_exception.hpp src/utils/no_such_triangle_exception.cpp
	include/utils/not_implemented_exception.hpp src/utils/not_implemented_exception.cpp
	include/utils/solve.hpp
	include/utils/strong_type.hpp
	include/utils/throw_if.hpp src/utils/throw_if.cpp
)

target_compile_options(${PROJECT_NAME} 
	PRIVATE
		-Wall
		"$<$<CONFIG:RELEASE>:-O3>"
		"$<$<CONFIG:DEBUG>:-DDEBUG>")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Symlink compile_commands.json from build-directory to CMakelists-directory.
# Reason: clangd recursively looks in parent directories
# for compile_commands.json, but build-directory is not necessarily a parent
# directory of source files, whereas CMakeLists-directory is.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)

# Setup headers
set(${PROJECT_NAME}_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/include

	#Temporary fix until Ive figured out if my build is broke or if its glbinding that did something bad
	${CONTRIB_DIR}/Source/glbinding_project/3rdparty/KHR/include
	${CONTRIB_DIR}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${${PROJECT_NAME}_INCLUDE_DIRS}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include/
)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
	target_link_libraries(${PROJECT_NAME} PUBLIC lsys glm::glm glfw glbinding::glbinding)
elseif(UNIX AND NOT APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads)
	target_link_libraries(${PROJECT_NAME} PUBLIC glfw dl GL X11 Xxf86vm Xrandr Threads::Threads Xi Xinerama Xcursor lsys glbinding::glbinding)
elseif(APPLE)
	find_package(OpenGL REQUIRED)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREVIDEO CoreVideo)
	target_link_libraries(${PROJECT_NAME} PRIVATE glfw dl ${OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO} glbinding::glbinding)
	target_link_libraries(${PROJECT_NAME} PUBLIC lsys)
endif(WIN32)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP example TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
add_executable(${PROJECT_NAME}_example src/example.cpp)
target_include_directories(${PROJECT_NAME}_example
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_link_libraries(${PROJECT_NAME}_example PUBLIC ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_example PROPERTIES OUTPUT_NAME example)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ----------------- SETUP intersection_visualizations TARGET ----------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
add_executable(${PROJECT_NAME}_intersection_visualizations src/intersection_visualizations.cpp)
target_include_directories(${PROJECT_NAME}_intersection_visualizations
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_link_libraries(${PROJECT_NAME}_intersection_visualizations PUBLIC ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_intersection_visualizations PROPERTIES OUTPUT_NAME intersection_visualizations)

add_subdirectory(tests)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP install TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY include DESTINATION include)
install(DIRECTORY shaders DESTINATION .)

# Create zip on windows but TGZ on linux and apple
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_PACKAGE_CONTACT joakimthoren93@gmail.com)
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 1)
if(WIN32)
	set(CPACK_GENERATOR ZIP)
elseif(UNIX)
	set(CPACK_GENERATOR TGZ)
endif(WIN32)

include(CPack)
