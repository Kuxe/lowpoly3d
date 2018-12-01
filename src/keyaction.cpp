#include "keyaction.hpp"

namespace lowpoly3d {

void KeyAction::executeOnPress() {
	onPress();
}

void KeyAction::executeOnRelease() {
	onRelease();
}

void KeyAction::executeOnHold() {
	onHold();
}

const KeyAction::function_type& KeyAction::getOnPressFunction() const {
	return onPress;
}

const KeyAction::function_type& KeyAction::getOnReleaseFunction() const {
	return onRelease;
}

const KeyAction::function_type& KeyAction::getOnHoldFunction() const {
	return onHold;
}

KeyAction& KeyAction::setOnPressFunction(const function_type& f) {
	onPress = f;
	return *this;
}

KeyAction& KeyAction::setOnReleaseFunction(const function_type& f) {
	onRelease = f;
	return *this;
}

KeyAction& KeyAction::setOnHoldFunction(const function_type& f) {
	onHold = f;
	return *this;
}

} // End of namespace lowpoly3d