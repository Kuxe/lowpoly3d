#ifndef ILOWPOLYINPUT_HPP
#define ILOWPOLYINPUT_HPP

/** Interface whose realizations can be passed to renderer. These methods will
	be called whenever such an event is received within the renderer. It is then up
	to the realization to decide what to do with these events.
	(=lowpoly3d only forwards events by glfw to whatever application which uses lowpoly3d). **/
struct ILowpolyInput {
	virtual ~ILowpolyInput() { }
	virtual void onError() { }
	virtual void onFramebufferResize(int width, int height) { }
	virtual void onKey(int key, int scancode, int action, int mods) { }
	virtual void onMouseEnter(double xpos, double ypos) { }
	virtual void onMouseExit(double xpos, double ypos) { }
	virtual void onMouse(double xpos, double ypos) { }
};

#endif //ILOWPOLYINPUT_HPP