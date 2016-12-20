#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "events.hpp"
#include "subber.hpp"

/** Camera can perform standard camera movements such as pan or truck.
	The standard camera movements are applied onto a matrix m which
	can be read via the get()-method. Intended use is as view-matrix
	in a model-view-projection matrix.

	Relies on the DeltaTime class.
**/

class Camera : Subber<wPress>, Subber<aPress>, Subber<sPress>, Subber<dPress>, Subber<qPress>, Subber<ePress>, Subber<MouseEvent>, Subber<CursorEnterWindow>, Subber<CursorExitWindow> {
private:
	glm::vec4 side, up, forward, eye;
	float yaw, pitch;
	double mouseX, mouseY;
	bool focused = false;
	glm::mat4 m() const;
public:
	Camera(const glm::mat4& origin = glm::mat4());
	~Camera();
	void tilt(const float f);
	void pan(const float f);
	void zoom(const float f);
	void pedestal(const float f);
	void dolly(const float f);
	void truck(const float f);
	const glm::mat4 get() const;

	void notify(const wPress& event);
	void notify(const aPress& event);
	void notify(const sPress& event);
	void notify(const dPress& event);
	void notify(const qPress& event);
	void notify(const ePress& event);
	void notify(const MouseEvent& event);
	void notify(const CursorEnterWindow& event);
	void notify(const CursorExitWindow& event);

};

#endif //CAMERA_HPP