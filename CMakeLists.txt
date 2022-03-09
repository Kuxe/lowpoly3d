cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(lowpoly3d
	VERSION 0.0.2
	LANGUAGES CXX
	HOMEPAGE_URL https://github.com/Kuxe/lowpoly3d
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(buildcache)
include(target_disable_warnings)
include(CheckCXXCompilerFlag)
include(FetchContent)
include(GNUInstallDirs)

# Check if compiler supports colored diagnostics, if so, use it.
check_cxx_compiler_flag(-fcolor-diagnostics is_supported_fcolor-diagnostics)
if(${is_supported_fcolor-diagnostics})
	add_compile_options("-fcolor-diagnostics")
endif()
check_cxx_compiler_flag(-fdiagnostics-color=always is_supported_fdiagnostics-color)
if(${is_supported_fdiagnostics-color})
	add_compile_options("-fdiagnostics-color=always")
endif()

configure_file(build.hpp.in build.hpp @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Variables
set(runtime_build_dir "${CMAKE_BINARY_DIR}/bin")
set(shaders_dir "${CMAKE_SOURCE_DIR}/shaders")

# Global options
option(BUILD_SHARED_LIBS "Build shared libraries." ON)
option(${PROJECT_NAME}_BUILD_EXAMPLES "Build examples." ON)
option(${PROJECT_NAME}_BUILD_TESTS "Build tests." ON)
option(${PROJECT_NAME}_BUILD_INTERSECTION_VISUALIZATIONS "Build intersection-visualizations." ON)

# RPATH
set(EXECUTABLE_INSTALL_RPATH $ORIGIN/../lib)
set(LIBRARY_INSTALL_RPATH $ORIGIN)

# Install RPATH is defaulted to the library install rpath.
# Executable targets are expected to set their own install rpath property.
set(CMAKE_INSTALL_RPATH ${LIBRARY_INSTALL_RPATH})
set(CMAKE_BUILD_WITH_INSTALL_RPATH False)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH False)
set(CMAKE_SKIP_BUILD_RPATH False)
set(CMAKE_BUILD_RPATH_USE_ORIGIN True)

# [WIN32] All projects should output dlls to the same directory.
# This is because .exe-files look for .dlls in its own directory,
# so if .exe-files should be runnable then all its dependencies
# must exist in its directory.
# Let that directory be ${CMAKE_BINARY_DIR}/bin, so that shaders
# live at "../shaders" as expected.
if(WIN32)
	# Windows does not have concept of RUNPATHs.
	# So build all dlls in same directory.
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${runtime_build_dir})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${runtime_build_dir})
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

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(CONTRIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib")

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
target_disable_warnings(glm)

FetchContent_Declare(
	glbinding
	GIT_REPOSITORY https://github.com/cginternals/glbinding.git
	GIT_TAG 70e76f9
)
set(OPTION_BUILD_TOOLS OFF CACHE INTERNAL "")
set(OPTION_BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_GetProperties(glbinding)
if(NOT glbinding_POPULATED)
	FetchContent_Populate(glbinding)
	add_subdirectory(${glbinding_SOURCE_DIR} ${glbinding_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if(WIN32)
	set_target_properties(glbinding PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${runtime_build_dir}
		LIBRARY_OUTPUT_DIRECTORY ${runtime_build_dir}
	)
	set_target_properties(glbinding-aux PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${runtime_build_dir}
		LIBRARY_OUTPUT_DIRECTORY ${runtime_build_dir}
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
# TODO: Don't use SHARED, let users decide with BUILD_SHARED_LIBS
add_library(${PROJECT_NAME}
	include/args.hxx
	include/bezier.hpp
	include/binary_path.hpp src/binary_path.cpp
	include/bounding_volume_hierarchy.hpp src/bounding_volume_hierarchy.cpp
	include/celestialbody.hpp
	include/camera.hpp src/camera.cpp
	include/draw_geometric_primitives.hpp src/draw_geometric_primitives.cpp
	include/events.hpp
	include/debugrenderer.hpp src/debugrenderer.cpp
	include/depthframebuffer.hpp src/depthframebuffer.cpp
	include/drawfeature.hpp src/drawfeature.cpp
	include/fps_camera.hpp src/fps_camera.cpp
	include/framebuffer.hpp src/framebuffer.cpp
	include/gamedata.hpp
	include/glfw_callbacks.hpp src/glfw_callbacks.cpp
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
	include/shaderprogrambank.hpp src/shaderprogrambank.cpp
	include/shaderprogram.hpp src/shaderprogram.cpp
	include/uniformbuffer.hpp src/uniformbuffer.cpp
	include/glframe.hpp src/glframe.cpp
	include/worlduniformdata.hpp
	include/minimum_bounding_sphere.hpp src/minimum_bounding_sphere.cpp

	include/generators/conegenerator.hpp src/generators/conegenerator.cpp
	include/generators/circlegenerator.hpp src/generators/circlegenerator.cpp
	include/generators/cubegenerator.hpp src/generators/cubegenerator.cpp
	include/generators/cylindergenerator.hpp src/generators/cylindergenerator.cpp
	include/generators/modelgenerator.hpp
	include/generators/planegenerator.hpp src/generators/planegenerator.cpp
	include/generators/spheregenerator.hpp src/generators/spheregenerator.cpp
	include/generators/terraingenerator.hpp src/generators/terraingenerator.cpp
	include/generators/treegenerator.hpp

	include/geometric_primitives/cone.hpp
	include/geometric_primitives/cylinder.hpp
	include/geometric_primitives/direction.hpp
	include/geometric_primitives/intersections.hpp src/geometric_primitives/intersections.cpp
	include/geometric_primitives/intersects.hpp src/geometric_primitives/intersects.cpp
	include/geometric_primitives/line.hpp
	include/geometric_primitives/linesegment.hpp
	include/geometric_primitives/parallelogram.hpp
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

# GNU compile commands
set(gcc_compile_options_common -Wall -Werror)
set(gcc_compile_options_debug "")
set(gcc_compile_options_release "")
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:${gcc_compile_options_common}>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:DEBUG>>:${gcc_compile_options_debug}>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:RELEASE>>:${gcc_compile_options_release}>)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Symlink compile_commands.json from build-directory to CMakelists-directory.
# Reason: clangd recursively looks in parent directories
# for compile_commands.json, but build-directory is not necessarily a parent
# directory of source files, whereas CMakeLists-directory is.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${shaders_dir} ${CMAKE_BINARY_DIR}/shaders
)


# Setup headers
set(${PROJECT_NAME}_INCLUDE_DIRS
	include
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
	target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 lsys glm::glm glfw glbinding::glbinding)
elseif(UNIX AND NOT APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads)
	target_link_libraries(${PROJECT_NAME} PRIVATE glfw dl GL X11 Xxf86vm Xrandr Threads::Threads Xi Xinerama Xcursor lsys glbinding::glbinding)
elseif(APPLE)
	find_package(OpenGL REQUIRED)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREVIDEO CoreVideo)
	target_link_libraries(${PROJECT_NAME} PRIVATE lsys glfw dl ${OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO} glbinding::glbinding)
endif(WIN32)

if(${PROJECT_NAME}_BUILD_EXAMPLES)
	add_subdirectory(example)
endif()

if(${PROJECT_NAME}_BUILD_INTERSECTION_VISUALIZATIONS)
	add_subdirectory(intersection_visualizations)
endif()

if(${PROJECT_NAME}_BUILD_TESTS)
	add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP install TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
install(TARGETS ${PROJECT_NAME} RUNTIME)
install(TARGETS glbinding RUNTIME)
install(DIRECTORY include DESTINATION .)
install(DIRECTORY ${shaders_dir} DESTINATION .)

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
