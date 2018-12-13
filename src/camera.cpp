#include "camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace lowpoly3d {

Camera::Camera(const glm::dmat4& origin) :
	side(glm::column(origin, 0)), up(glm::column(origin, 1)), forward(glm::column(origin, 2)), yaw(0.0), pitch(0.0), eye(glm::column(origin, 3)) {

}

void Camera::tilt(const double f) {
	const double pi = std::acos(-1);
	const double pihalf = pi / 2.0;
	pitch += f;
	pitch = pitch > pihalf ? pihalf : pitch;
	pitch = pitch < -pihalf ? -pihalf : pitch;
}

void Camera::pan(const double f) {
	yaw += f;
}

void Camera::zoom(const double f) {
	
}

void Camera::pedestal(const double f) {
	eye += up * f;
}

void Camera::dolly(const double f) {
	eye += glm::rotate(forward, -yaw, up) * f;
}

void Camera::truck(const double f) {
	eye += glm::rotate(side, -yaw, up) * f;
}

void Camera::look(const glm::dvec2& mouse, double dt) {
	pan(mouse.x * dt - lastMouseCoord.x * dt);
	tilt(mouse.y * dt * .6 - lastMouseCoord.y * dt * .6);
	lastMouseCoord = mouse;
}

const glm::dmat4 Camera::transform() const {
	return glm::rotate(glm::rotate(m(), yaw, up), pitch, side);
}

const glm::dmat4 Camera::view() const {
	return glm::inverse(glm::scale(transform(), glm::dvec3(1.0, 1.0, -1.0)));
}

glm::dmat4 Camera::m() const {
	return glm::mat4(glm::dvec4(side, 0.0), glm::dvec4(up, 0.0), glm::dvec4(forward, 0.0), glm::dvec4(eye, 1.0));
}

}
