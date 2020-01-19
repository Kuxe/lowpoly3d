#include "fps_camera.hpp"

#include "camera.hpp"
#include "keymanager.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace lowpoly3d
{

FPSCamera::FPSCamera(KeyManager& keyManager, duration_s& dt)
	: mCamera(std::make_unique<Camera>())
	, mKeyManagerRef(keyManager)
	, mCameraSpeed(3.0)
{
	/** Map key events to functions **/
	auto &keymanager = mKeyManagerRef.get();
	keymanager[GLFW_KEY_W].setOnHoldFunction([this, &dt]() { mCamera->dolly(+mCameraSpeed * dt.count()); });
	keymanager[GLFW_KEY_S].setOnHoldFunction([this, &dt]() { mCamera->dolly(-mCameraSpeed * dt.count()); });
	keymanager[GLFW_KEY_A].setOnHoldFunction([this, &dt]() { mCamera->truck(-mCameraSpeed * dt.count()); });
	keymanager[GLFW_KEY_D].setOnHoldFunction([this, &dt]() { mCamera->truck(+mCameraSpeed * dt.count()); });
	keymanager[GLFW_KEY_Q].setOnHoldFunction([this, &dt]() { mCamera->pedestal(-mCameraSpeed * dt.count()); });
	keymanager[GLFW_KEY_E].setOnHoldFunction([this, &dt]() { mCamera->pedestal(+mCameraSpeed * dt.count()); });
	keymanager[GLFW_KEY_LEFT_SHIFT]
		.setOnPressFunction([this]() { mCameraSpeed = 10.0f; })
		.setOnReleaseFunction([this]() { mCameraSpeed = 3.0f; });
}

void FPSCamera::look(double xpos, double ypos)
{
	const double radsPerPixel = std::acos(-1) / 2000.0;
	mCamera->look({xpos, ypos}, mCameraSpeed * radsPerPixel);
}

glm::dmat4 const FPSCamera::transform() const
{
	return mCamera->transform();
}

glm::dmat4 const FPSCamera::view() const
{
	return mCamera->view();
}

} // End of namespace lowpoly3d