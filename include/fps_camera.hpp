#ifndef FPS_CAMERA_HPP
#define FPS_CAMERA_HPP

#include <glm/mat4x4.hpp>

#include <chrono>
#include <memory>

// Forward declarations
namespace lowpoly3d
{
	class Camera;
	class KeyManager;
}

namespace lowpoly3d
{

/* FPSCamera is very similar to lowpoly3d::Camera,
 * but it is "prebinded" with keys */
class FPSCamera
{
public:

	using duration_s = std::chrono::duration<double, std::ratio<1, 1>>;

	/* Set FPS camera bindings on keyManager.
	 * This overwrites existing conflicting bindings */
	FPSCamera(KeyManager& keyManager, duration_s& dt);

	void look(double xpos, double ypos);

	/** Gets the transform matrix of the camera.
	Not to be confused with the view-matrix! **/
	[[nodiscard]] glm::dmat4 const transform() const;

	/** Gets the view matrix which takes objects from
		model-space to view-space**/
	[[nodiscard]] glm::dmat4 const view() const;

private:
	std::unique_ptr<Camera> mCamera;
	std::reference_wrapper<KeyManager> mKeyManagerRef;
	double mCameraSpeed;
	duration_s mDt;
};

};

#endif // FPS_CAMERA_HPP