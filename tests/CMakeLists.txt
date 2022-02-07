cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

add_executable(${PROJECT_NAME}_test
	bounding_volume_hierarchy_test.cpp
	solve_test.cpp
	sphere_test.cpp
	intersections_test.cpp
	lerp_test.cpp
	arithmetic_invariant_test.cpp
	triangle_test.cpp
	plane_test.cpp
)
enable_testing()
target_include_directories(${PROJECT_NAME}_test PRIVATE ${${PROJECT_NAME}_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain glm::glm)
target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_17)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME}_test)