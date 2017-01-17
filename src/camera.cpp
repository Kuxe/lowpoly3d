#include "camera.hpp"
#include "deltatime.hpp"
#include "glm/ext.hpp"

Camera::Camera(const glm::mat4& origin) : 
	side(1.0, 0.0, 0.0, 0.0), up(0.0, 1.0, 0.0, 0.0), forward(0.0, 0.0, 1.0, 0.0), eye(0.0, 0.707/2.0, 2.0, 1.0) {

}

void Camera::tilt(const float f) {
	yaw += f * DeltaTime::dt;
	yaw = yaw > 3.14f/2.0f ? 3.14f/2.0f : yaw;
	yaw = yaw < -3.14f/2.0f ? -3.14f/2.0f : yaw;
}

void Camera::pan(const float f) {
	const glm::mat4 rotAroundY = glm::translate(glm::mat4(), glm::vec3(eye)) * glm::rotate(glm::mat4(), f * DeltaTime::dt, -glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::mat4(), glm::vec3(-eye));
	forward = rotAroundY * forward;
	side = rotAroundY * side;
}

void Camera::zoom(const float f) {
	
}

void Camera::pedestal(const float f) {
	eye += glm::vec4(0.0, 1.0, 0.0, 0.0) * f * DeltaTime::dt;
}

void Camera::dolly(const float f) {
	eye += glm::rotate(glm::mat4(), -yaw, glm::vec3(side)) * forward * f * DeltaTime::dt;;
}

void Camera::truck(const float f) {
	eye += side * f * DeltaTime::dt;
}

void Camera::look(const glm::vec2& mouse) {
	pan((mouse.x-lastMouseCoord.x) * DeltaTime::dt * 10.0f);
	tilt((mouse.y-lastMouseCoord.y) * DeltaTime::dt * 6.0f);
	lastMouseCoord = mouse;
}

const glm::mat4 Camera::get() const {
	return glm::inverse(m()) * glm::translate(glm::mat4(), glm::vec3(eye)) * glm::rotate(glm::mat4(), yaw, glm::vec3(side)) * glm::translate(glm::mat4(), -glm::vec3(eye));
}

glm::mat4 Camera::m() const {
	return glm::mat4(side, up, forward, eye);
}
