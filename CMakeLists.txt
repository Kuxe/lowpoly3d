cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

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
SET(CONTRIB_DIR "${CMAKE_SOURCE_DIR}/contrib")
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_BASE "${CONTRIB_DIR}")

ExternalProject_Add(glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

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
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DOPTION_BUILD_TESTS=OFF
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

add_dependencies(glbinding_project glfw)

#Run "make conf" to download dependencies
#This must be done atleast once before running "make example" or "make dev"
add_custom_target(configure
    DEPENDS glm glfw glbinding_project subber queue
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

link_directories(${CONTRIB_DIR}/Source/glfw/src)
link_directories(${CONTRIB_DIR}/Source/glbinding_project)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# ------------------------- SETUP lowpoly3d PROJECT -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
project(lowpoly3d)

set(LOWPOLY3D_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/include
	${CONTRIB_DIR}/Source/glfw/include
	${CONTRIB_DIR}/Source/glm
	${CONTRIB_DIR}/Source/glbinding_project/source/glbinding/include
	${CONTRIB_DIR}/Source/subber
	${CONTRIB_DIR}/Source/queue
)
include_directories(${LOWPOLY3D_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O3 -Wall -std=c++14 -fomit-frame-pointer") #CPack needs this for some reason unknown
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -std=c++14 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3 -Wall -std=c++14 -fomit-frame-pointer")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# -------------------------- SETUP lowpoly3d TARGET -------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
#Run "make lowpoly3d" to build library target
add_library(lowpoly3d SHARED 
	src/renderer.cpp
	src/renderdata.cpp
	src/model.cpp
	src/shaderprogram.cpp
	src/camera.cpp
	src/terraingenerator.cpp
	src/cubegenerator.cpp
	src/spheregenerator.cpp
	src/planegenerator.cpp
	src/circlegenerator.cpp
	src/framebuffer.cpp
	src/depthframebuffer.cpp
	src/uniformbuffer.cpp
	src/perlin.cpp
)

if(WIN32)
	target_link_libraries(lowpoly3d glbinding glfw3 opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(lowpoly3d glbinding glfw3 dl GL X11 Xxf86vm Xrandr pthread Xi Xinerama Xcursor)
elseif(APPLE)
	find_package(OpenGL REQUIRED)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREVIDEO CoreVideo)
	target_link_libraries(lowpoly3d glbinding glfw3 dl ${OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO})
endif(WIN32)

add_dependencies(lowpoly3d configure)

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP example TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
#Run "make example" to build the example
add_executable(example src/example.cpp)
target_link_libraries(example lowpoly3d)
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