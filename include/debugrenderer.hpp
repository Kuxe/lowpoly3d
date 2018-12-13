#ifndef DEBUGRENDERER_HPP

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

// Forward declarations

namespace lowpoly3d {
	class Framebuffer;
	class ShaderProgram;
}

namespace lowpoly3d {

/** Class encapsualting graphical debugging functionality.
	This class is assumed to be a member of a parent renderer.
	Whenever the "render" method is called, the debug renderer draws
	the debug primitives (whose coordinates are in world-space) onto
	the binded framebuffer. **/
class DebugRenderer {
	gl::GLuint vbo, vao;
	bool inited = false;
public:

	/** Initializes the debug renderer - send lines, circles etc to GPU memory **/
	bool init();

	//Render debug primitives. Returns true on success, otherwise false.
	bool render(const glm::mat4& vp, const Framebuffer& fb, const ShaderProgram& sp);
};

} //End of namespace lowpoly3d

#endif //DEBUGRENDERER_HPP