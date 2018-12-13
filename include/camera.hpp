#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

namespace lowpoly3d {

/** Camera can perform standard camera movements such as pan or truck.
	The standard camera movements are applied onto a matrix m which
	can be read via the get()-method. Intended use is as view-matrix
	in a model-view-projection matrix.

	Relies on the DeltaTime class.
**/

class Camera {
private:
	glm::dvec3 side, up, forward;
	double yaw, pitch;
	glm::dvec2 lastMouseCoord = {0.0, 0.0};
	glm::dmat4 m() const;
public:
	glm::dvec3 eye;
	Camera(const glm::dmat4& origin = glm::translate(glm::scale(glm::dmat4(1.0), glm::dvec3(1.0, 1.0, -1.0)), glm::dvec3(0.0, 5.0, 0.0)));
	void tilt(const double f);
	void pan(const double f);
	void zoom(const double f);
	void pedestal(const double f);
	void dolly(const double f);
	void truck(const double f);
	void look(const glm::dvec2& mouse, double dt);

	/** Gets the transform matrix of the camera.
		Not to be confused with the view-matrix! **/
	const glm::dmat4 transform() const;

	/** Gets the view matrix which takes objects from
		model-space to view-space**/
	const glm::dmat4 view() const;
};

}

#endif //CAMERA_HPP