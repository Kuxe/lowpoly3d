#include "camera.hpp"
#include <glm/ext.hpp>

namespace lowpoly3d {

Camera::Camera(const glm::mat4& origin) : 
	side(1.0, 0.0, 0.0, 0.0), up(0.0, 1.0, 0.0, 0.0), forward(0.0, 0.0, 1.0, 0.0), eye(0.0, 0.707/2.0, 2.0, 1.0) {

}

void Camera::tilt(const float f) {
	yaw += f;
	yaw = yaw > 3.14f/2.0f ? 3.14f/2.0f : yaw;
	yaw = yaw < -3.14f/2.0f ? -3.14f/2.0f : yaw;
}

void Camera::pan(const float f) {
	const glm::mat4 rotAroundY = glm::translate(glm::mat4(), glm::vec3(eye)) * glm::rotate(glm::mat4(), f, -glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::mat4(), glm::vec3(-eye));
	forward = rotAroundY * forward;
	side = rotAroundY * side;
}

void Camera::zoom(const float f) {
	
}

void Camera::pedestal(const float f) {
	eye += glm::vec4(0.0, 1.0, 0.0, 0.0) * f;
}

void Camera::dolly(const float f) {
	eye += glm::rotate(glm::mat4(), -yaw, glm::vec3(side)) * forward * f;
}

void Camera::truck(const float f) {
	eye += side * f;
}

void Camera::look(const glm::vec2& mouse, float dt) {
	pan((mouse.x-lastMouseCoord.x) * dt);
	tilt((mouse.y-lastMouseCoord.y) * dt * .6f);
	lastMouseCoord = mouse;
}

const glm::mat4 Camera::get() const {
	return glm::inverse(m()) * glm::translate(glm::mat4(), glm::vec3(eye)) * glm::rotate(glm::mat4(), yaw, glm::vec3(side)) * glm::translate(glm::mat4(), -glm::vec3(eye));
}

glm::mat4 Camera::m() const {
	return glm::mat4(side, up, forward, eye);
}

}
