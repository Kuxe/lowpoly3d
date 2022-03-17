add_executable(${PROJECT_NAME}_intersection_visualizations src/intersection_visualizations.cpp)
target_link_libraries(${PROJECT_NAME}_intersection_visualizations PRIVATE ${PROJECT_NAME} glm::glm glfw glbinding::glbinding lsys range-v3)
set_target_properties(${PROJECT_NAME}_intersection_visualizations PROPERTIES
	OUTPUT_NAME intersection_visualizations
	INSTALL_RPATH ${EXECUTABLE_INSTALL_RPATH}
)

install(TARGETS ${PROJECT_NAME}_intersection_visualizations RUNTIME)
