#ifndef ILOWPOLYINPUT_HPP
#define ILOWPOLYINPUT_HPP

/** Interface whose realizations can be passed to renderer. These methods will
	be called whenever such an event is received within the renderer. It is then up
	to the realization to decide what to do with these events.
	(=lowpoly3d only forwards events by glfw to whatever application which uses lowpoly3d). **/
struct ILowpolyInput {
	virtual ~ILowpolyInput() { }
	virtual void onError() = 0;
	virtual void onFramebufferResize() = 0;
	virtual void onKeyPress() = 0;
	virtual void onKeyRelease() = 0;
	virtual void onMouseEnter() = 0;
	virtual void onMouseExit() = 0;
	virtual void onMouse() = 0; 
};

struct DummyLowpolyInput : ILowpolyInput {
	~DummyLowpolyInput() { }
	void onError() { }
	void onFramebufferResize() { }
	void onKeyPress() { }
	void onKeyRelease() { }
	void onMouseEnter() { }
	void onMouseExit() { }
	void onMouse() { }
};


#endif //ILOWPOLYINPUT_HPP