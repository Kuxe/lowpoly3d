#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

namespace lowpoly3d {

/** Camera can perform standard camera movements such as pan or truck.
	The standard camera movements are applied onto a matrix m which
	can be read via the get()-method. Intended use is as view-matrix
	in a model-view-projection matrix.

	Relies on the DeltaTime class.
**/

class Camera {
private:
	glm::vec4 side, up, forward, eye;
	float yaw;
	glm::vec2 lastMouseCoord = {0.0f, 0.0f};
	glm::mat4 m() const;
public:
	Camera(const glm::mat4& origin = glm::mat4());
	void tilt(const float f);
	void pan(const float f);
	void zoom(const float f);
	void pedestal(const float f);
	void dolly(const float f);
	void truck(const float f);
	void look(const glm::vec2& mouse, float dt);
	const glm::mat4 get() const;
};

}

#endif //CAMERA_HPP