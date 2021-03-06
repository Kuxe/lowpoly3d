cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

include(ExternalProject)

# Only set UPDATE_DISCONNECTED 1 if cmake supports it
if(CMAKE_VERSION VERSION_LESS 3.2)
	set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
	set(UPDATE_DISCONNECTED_IF_AVAILABLE UPDATE_DISCONNECTED 1)
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

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.34.tar.gz"
    SHA1 "70287b1ffa810ee4e952052a9adff9b4856d0d54"
)

SET(CONTRIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib")
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_BASE "${CONTRIB_DIR}")

# Using hunter for glm instead of ExternalProject_Add since ExternalProject_Add
# gives a bad taste in my mouth
#ExternalProject_Add(glm
#	GIT_REPOSITORY https://github.com/g-truc/glm.git
#	${UPDATE_DISCONNECTED_IF_AVAILABLE}
#	BUILD_IN_SOURCE 1
#	CONFIGURE_COMMAND ""
#	BUILD_COMMAND ""
#	INSTALL_COMMAND ""
#)

ExternalProject_Add(glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	BUILD_IN_SOURCE 1
	CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
	INSTALL_COMMAND ""
)


ExternalProject_Add(glbinding_project
	GIT_REPOSITORY https://github.com/cginternals/glbinding.git
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	BUILD_IN_SOURCE 1
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DOPTION_BUILD_TESTS=OFF -DOPTION_BUILD_EXAMPLES=OFF -DOPTION_BUILD_TOOLS=OFF
	INSTALL_COMMAND ""
)

ExternalProject_Add(subber
	GIT_REPOSITORY https://github.com/Kuxe/subber.git
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	BUILD_IN_SOURCE 1
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DOPTION_BUILD_TESTS=OFF
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

ExternalProject_Add(queue
	GIT_REPOSITORY https://gist.github.com/6bdd5b748b5f11b303a5cccbb8c8a731.git
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

ExternalProject_Add(node
	GIT_REPOSITORY https://gist.github.com/Kuxe/82d69f65b6567d516963160099eccd92.git
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

ExternalProject_Add(lsys_project
	GIT_REPOSITORY https://github.com/Kuxe/lsys.git
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	BUILD_IN_SOURCE 1
)

add_dependencies(glbinding_project glfw)

#Run "make conf" to download dependencies
#This must be done atleast once before running "make example" or "make dev"
add_custom_target(configure
    DEPENDS glm glfw glbinding_project subber queue node lsys_project
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

link_directories(${CONTRIB_DIR}/Source/glfw/src)
link_directories(${CONTRIB_DIR}/Source/glbinding_project)
link_directories(${CONTRIB_DIR}/Source/lsys_project)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ------------------------- SETUP lowpoly3d PROJECT -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
project(liblowpoly3d VERSION 0.0.1 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ----------------------------- Hunter packages ------------------------------ #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
hunter_add_package(glm)
find_package(glm REQUIRED)

hunter_add_package(Catch)
find_package(Catch2 REQUIRED)

set(LOWPOLY3D_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CONTRIB_DIR}/Source/glfw/include
	${CONTRIB_DIR}/Source/glm
	${CONTRIB_DIR}/Source/glbinding_project/source/glbinding/include
	${CONTRIB_DIR}/Source/subber
	${CONTRIB_DIR}/Source/queue
	${CONTRIB_DIR}/Source/node
	${CONTRIB_DIR}/Source/lsys_project
)

# Thanks to https://stackoverflow.com/a/34317588
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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #Create a compilation database for use with clang static analyzer

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# -------------------------- SETUP lowpoly3d TARGET -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
#Run "make lowpoly3d" to build library target
add_library(lowpoly3d SHARED
	
	include/args.hxx
	include/bezier.hpp
	include/bounding_volume_hierarchy.hpp		src/bounding_volume_hierarchy.cpp
	include/celestialbody.hpp
	include/camera.hpp							src/camera.cpp
	include/events.hpp
	include/debugrenderer.hpp					src/debugrenderer.cpp
	include/depthframebuffer.hpp				src/depthframebuffer.cpp
	include/framebuffer.hpp						src/framebuffer.cpp
	include/gamedata.hpp
	include/ilowpolyinput.hpp
	include/keyaction.hpp						src/keyaction.cpp
	include/keymanager.hpp						src/keymanager.cpp
	include/lowpoly3d.hpp
	include/model.hpp							src/model.cpp
	include/modeldefs.hpp
	include/modeluniformdata.hpp
	include/mvpuniformdata.hpp
	include/perlin.hpp							src/perlin.cpp
	include/renderer.hpp 						src/renderer.cpp
	include/renderdata.hpp 						src/renderdata.cpp
	include/scene.hpp
	include/shaderprogram.hpp					src/shaderprogram.cpp
	include/uniformbuffer.hpp					src/uniformbuffer.cpp
	include/glframe.hpp							src/glframe.cpp
	include/worlduniformdata.hpp

	include/generators/circlegenerator.hpp		src/generators/circlegenerator.cpp
	include/generators/cubegenerator.hpp		src/generators/cubegenerator.cpp
	include/generators/cylindergenerator.hpp	src/generators/cylindergenerator.cpp
	include/generators/modelgenerator.hpp
	include/generators/planegenerator.hpp		src/generators/planegenerator.cpp
	include/generators/spheregenerator.hpp		src/generators/spheregenerator.cpp
	include/generators/terraingenerator.hpp		src/generators/terraingenerator.cpp
	include/generators/treegenerator.hpp

	include/geometric_primitives/intersections.hpp
	include/geometric_primitives/line.hpp
	include/geometric_primitives/linesegment.hpp
	include/geometric_primitives/plane.hpp
	include/geometric_primitives/point.hpp
	include/geometric_primitives/sphere.hpp		src/geometric_primitives/sphere.cpp
	include/geometric_primitives/triangle.hpp

	include/utils/arithmetic_invariant.hpp
	include/utils/glmutils.hpp
	include/utils/lerp.hpp
	include/utils/misc.hpp
	include/utils/not_implemented_exception.hpp	src/utils/not_implemented_exception.cpp
	include/utils/strong_type.hpp
)

target_compile_options(lowpoly3d 
	PRIVATE
		-Wall
		"$<$<CONFIG:RELEASE>:-O3>"
		"$<$<CONFIG:DEBUG>:-DDEBUG>")
target_compile_features(lowpoly3d PUBLIC cxx_std_14)

#Thanks to https://stackoverflow.com/questions/13429656/cmake-how-to-copy-contents-of-a-directory-into-build-directory-after-make/13429998
add_custom_command(TARGET lowpoly3d POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CONTRIB_DIR}/Source/lsys_project/install/bin $<TARGET_FILE_DIR:lowpoly3d>)

# Symlink compile_commands.json from build-directory to CMakelists-directory.
# Reason: clangd recursively looks in parent directories
# for compile_commands.json, but build-directory is not necessarily a parent
# directory of source files, whereas CMakeLists-directory is.
add_custom_command(TARGET lowpoly3d POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)

# Setup headers
target_include_directories(lowpoly3d
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${LOWPOLY3D_INCLUDE_DIRS}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include/
)

if(WIN32)
	target_link_libraries(lowpoly3d PRIVATE glbinding glfw3 opengl32)
	target_link_libraries(lowpoly3d PUBLIC lsys Catch2::Catch)
elseif(UNIX AND NOT APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads)
    target_link_libraries(lowpoly3d PUBLIC glbinding glfw3 dl GL X11 Xxf86vm Xrandr Threads::Threads Xi Xinerama Xcursor lsys Catch2::Catch)
elseif(APPLE)
	find_package(OpenGL REQUIRED)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREVIDEO CoreVideo)
	target_link_libraries(lowpoly3d PRIVATE glbinding glfw3 dl ${OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO})
	target_link_libraries(lowpoly3d PUBLIC lsys Catch2::Catch)
endif(WIN32)

add_dependencies(lowpoly3d configure)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP example TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
#Run "make example" to build the example
add_executable(example src/example.cpp)
target_include_directories(example
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_link_libraries(example PUBLIC lowpoly3d)
set_target_properties(example PROPERTIES OUTPUT_NAME example)


# All targets trigger the library target build. Library target output shared libraries into "libs" directory
# dev target depends on library target
# example target depends on library target
# "make configure" should run the configure target
# "make build" should build library
# "make dev" should build dev and library
# "make example" should build example and library
# "make install" should output headers + library into install directory

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- Testing using Catch ---------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_executable(lowpoly3d_test
	${TEST_DIR}/catch_tests_main.cpp
	${TEST_DIR}/bounding_volume_hierarchy_test.cpp
	${TEST_DIR}/sphere_test.cpp
	${TEST_DIR}/intersections_test.cpp
	${TEST_DIR}/lerp_test.cpp
	${TEST_DIR}/arithmetic_invariant_test.cpp
)
enable_testing()
target_include_directories(lowpoly3d_test PRIVATE ${LOWPOLY3D_INCLUDE_DIRS})
target_link_libraries(lowpoly3d_test PUBLIC lowpoly3d Catch2::Catch)
target_compile_features(lowpoly3d_test PRIVATE cxx_std_14)

include(CTest)
include("cmake/Catch.cmake")
include("cmake/ParseAndAddCatchTests.cmake")
catch_discover_tests(lowpoly3d_test)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP install TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
install(TARGETS lowpoly3d DESTINATION lib)
install(DIRECTORY include DESTINATION .)
install(DIRECTORY contrib/Source/glfw/include/ DESTINATION include)
install(DIRECTORY contrib/Source/glm/glm DESTINATION include)
install(DIRECTORY contrib/Source/glbinding_project/source/glbinding/include/glbinding DESTINATION include)
install(FILES contrib/Source/subber/subber.hpp DESTINATION include)
install(DIRECTORY shaders DESTINATION .)

# Create zip on windows but TGZ on linux and apple
set(CPACK_PACKAGE_NAME lowpoly3d)
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
