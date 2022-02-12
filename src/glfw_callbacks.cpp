#include "glfw_callbacks.hpp"

#include "events.hpp"
#include "subber.hpp"

#include <GLFW/glfw3.h>

namespace lowpoly3d
{
	ILowpolyInput* GLFWCallbacks::handler = nullptr;
	
	void GLFWCallbacks::set_input_handler(ILowpolyInput* iHandler)
	{
		handler = iHandler;
	}

	void GLFWCallbacks::error_callback(int error, const char* description) {
		handler->onError();
	}

	void GLFWCallbacks::framebuffer_size_callback(GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		handler->onFramebufferResize(w, h);
		subber::publish<OnResize>({w, h});
	}

	void GLFWCallbacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
		handler->onKey(key, scancode, action, mods);

		/** GLFW_KEY_R might be used for something else in client-application, so
			whenever user presses 'R' an (unintentional) live-reload of shaders
			happen. This is not fine for release-build but its not to worrysome in a debug-build **/
		#ifdef DEBUG
		if(key == GLFW_KEY_R) {
			subber::publish<rPress>({});
		}
		#endif //DEBUG
	}

	void GLFWCallbacks::cursor_enter_callback(GLFWwindow* window, int focused) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if(focused) {
			handler->onMouseEnter(x, y);
		} else {
			handler->onMouseExit(x, y);
		}
	}

	void GLFWCallbacks::mouse_callback(GLFWwindow* window, double x, double y) {
		handler->onMouse(x, y);
	}
} // End of namespace lowpoly3d