#ifndef KEYACTION_HPP
#define KEYACTION_HPP

#include <functional>

namespace lowpoly3d {

/* A keyaction binds functionality to certain keyevents.
 * The available keyevents are:
 * 
 * 1. OnPress
 * 2. OnRelease
 * 3. OnHold
 * 
 * Idiomatic usage is to construct a KeyAction with three lambdas that determine
 * behaviour on each type of key event.
*/

class KeyAction {
public:
	using function_type = std::function<void(void)>;

	KeyAction(int keycode);

	int getKeycode() const;

	void executeOnPress();
	void executeOnRelease();
	void executeOnHold();

	const function_type& getOnPressFunction() const;
	const function_type& getOnReleaseFunction() const;
	const function_type& getOnHoldFunction() const;
	
	KeyAction& setOnPressFunction(const function_type&);
	KeyAction& setOnReleaseFunction(const function_type&);
	KeyAction& setOnHoldFunction(const function_type&);

private:
	std::function<void(void)>
		onPress = [](){},
		onRelease = [](){},
		onHold = [](){};
	int mKeycode;
};

} // End of namespace lowpoly3d

#endif // KEYACTION_HPP