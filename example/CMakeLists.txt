
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# --------------------------- SETUP example TARGET --------------------------- #
# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
add_executable(${PROJECT_NAME}_example src/example.cpp)
target_link_libraries(${PROJECT_NAME}_example PRIVATE ${PROJECT_NAME} glm::glm glfw glbinding::glbinding lsys)
set_target_properties(${PROJECT_NAME}_example PROPERTIES OUTPUT_NAME example)

install(TARGETS ${PROJECT_NAME}_example RUNTIME)