#include "camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace lowpoly3d {

Camera::Camera(const glm::mat4& origin) :
	side(glm::column(origin, 0)), up(glm::column(origin, 1)), forward(glm::column(origin, 2)), yaw(0.0f), pitch(0.0f), eye(glm::column(origin, 3)) {

}

void Camera::tilt(const float f) {
	pitch += f;
	pitch = pitch > 3.14f/2.0f ? 3.14f/2.0f : pitch;
	pitch = pitch < -3.14f/2.0f ? -3.14f/2.0f : pitch;
}

void Camera::pan(const float f) {
	yaw += f;
}

void Camera::zoom(const float f) {
	
}

void Camera::pedestal(const float f) {
	eye += up * f;
}

void Camera::dolly(const float f) {
	eye += glm::rotate(forward, -yaw, up) * f;
}

void Camera::truck(const float f) {
	eye += glm::rotate(side, -yaw, up) * f;
}

void Camera::look(const glm::vec2& mouse, float dt) {
	pan((mouse.x-lastMouseCoord.x) * dt);
	tilt((mouse.y-lastMouseCoord.y) * dt * .6f);
	lastMouseCoord = mouse;
}

const glm::mat4 Camera::transform() const {
	return glm::rotate(glm::rotate(m(), yaw, up), pitch, side);
}

const glm::mat4 Camera::view() const {
	return glm::inverse(glm::scale(transform(), glm::vec3(1.0f, 1.0f, -1.0f)));
}

glm::mat4 Camera::m() const {
	return glm::mat4(glm::vec4(side, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(eye, 1.0f));
}

}
