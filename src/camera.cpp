#include "camera.hpp"
#include "deltatime.hpp"
#include "glm/ext.hpp"

Camera::Camera(const glm::mat4& origin) : 
	side(1.0, 0.0, 0.0, 0.0), up(0.0, 1.0, 0.0, 0.0), forward(0.0, 0.0, 1.0, 0.0), eye(0.0, 0.707/2.0, 2.0, 1.0), mouseX(0.0), mouseY(0.0) {

}

Camera::~Camera() {

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

const glm::mat4 Camera::get() const {
	return glm::inverse(m()) * glm::translate(glm::mat4(), glm::vec3(eye)) * glm::rotate(glm::mat4(), yaw, glm::vec3(side)) * glm::translate(glm::mat4(), -glm::vec3(eye));
}

glm::mat4 Camera::m() const {
	return glm::mat4(side, up, forward, eye);
}

void Camera::notify(const wPress& event) {
	dolly(-3.0f);
}

void Camera::notify(const aPress& event) {
	truck(-3.0f);
}

void Camera::notify(const sPress& event) {
	dolly(+3.0f);
}

void Camera::notify(const dPress& event) {
	truck(+3.0f);
}

void Camera::notify(const qPress& event) {
	pedestal(-3.0f);
}

void Camera::notify(const ePress& event) {
	pedestal(+3.0f);
}

void Camera::notify(const MouseEvent& event) {
	if(focused) {
		pan((event.x - mouseX) * DeltaTime::dt * 10.0f);
		tilt((event.y - mouseY) * DeltaTime::dt * 6.0f);
		mouseX = event.x;
		mouseY = event.y;
	}
}

void Camera::notify(const CursorEnterWindow& event) {
	mouseX = event.x;
	mouseY = event.y;
	focused = true;
}

void Camera::notify(const CursorExitWindow& event) {
	focused = false;
}
