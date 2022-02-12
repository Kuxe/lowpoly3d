#ifndef GLFW_CALLBACKS_HPP
#define GLFW_CALLBACKS_HPP

#include "ilowpolyinput.hpp"

struct GLFWwindow;

namespace lowpoly3d
{
	struct ILowpolyInput;

	struct GLFWCallbacks {
		static ILowpolyInput* handler;
		static void set_input_handler(ILowpolyInput* iHandler);
		static void error_callback(int error, const char* description);
		static void framebuffer_size_callback(GLFWwindow* window, int w, int h);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_enter_callback(GLFWwindow* window, int focused);
		static void mouse_callback(GLFWwindow* window, double x, double y);
	};
	
} // End of namespace lowpoly3d

#endif // GLFW_CALLBACKS_HPP