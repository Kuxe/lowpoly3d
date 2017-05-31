#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace lowpoly3d {

/** Camera can perform standard camera movements such as pan or truck.
	The standard camera movements are applied onto a matrix m which
	can be read via the get()-method. Intended use is as view-matrix
	in a model-view-projection matrix.

	Relies on the DeltaTime class.
**/

class Camera {
private:
	glm::vec3 side, up, forward;
	float yaw, pitch;
	glm::vec2 lastMouseCoord = {0.0f, 0.0f};
	glm::mat4 m() const;
public:
	glm::vec3 eye;
	Camera(const glm::mat4& origin = glm::mat4());
	void tilt(const float f);
	void pan(const float f);
	void zoom(const float f);
	void pedestal(const float f);
	void dolly(const float f);
	void truck(const float f);
	void look(const glm::vec2& mouse, float dt);

	/** Gets the transform matrix of the camera.
		Not to be confused with the view-matrix! **/
	const glm::mat4 transform() const;

	/** Gets the view matrix which takes objects from
		model-space to view-space**/
	const glm::mat4 view() const;
};

}

#endif //CAMERA_HPP