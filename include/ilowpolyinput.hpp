#ifndef ILOWPOLYINPUT_HPP
#define ILOWPOLYINPUT_HPP

/** Interface whose realizations can be passed to renderer. These methods will
	be called whenever such an event is received within the renderer. It is then up
	to the realization to decide what to do with these events.
	(=lowpoly3d only forwards events by glfw to whatever application which uses lowpoly3d). **/
struct ILowpolyInput {
	virtual ~ILowpolyInput() { }
	virtual void onError() = 0;
	virtual void onFramebufferResize(int width, int height) = 0;
	virtual void onKey(int key, int scancode, int action, int mods) = 0;
	virtual void onMouseEnter(double xpos, double ypos) = 0;
	virtual void onMouseExit(double xpos, double ypos) = 0;
	virtual void onMouse(double xpos, double ypos) = 0; 
};

struct DummyLowpolyInput : ILowpolyInput {
	~DummyLowpolyInput() { }
	void onError() { }
	void onFramebufferResize(int width, int height) { }
	void onKey(int key, int scancode, int action, int mods) { }
	void onMouseEnter(double xpos, double ypos) { }
	void onMouseExit(double xpos, double ypos) { }
	void onMouse(double xpos, double ypos) { }
};


#endif //ILOWPOLYINPUT_HPP