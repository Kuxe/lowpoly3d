#include "keymanager.hpp"

namespace lowpoly3d {

void KeyManager::bind(const KeyAction& action) {
	keybindings.emplace(action.getKeycode(), action);
}

void KeyManager::unbind(int key) {
	keybindings.erase(key);
}

void KeyManager::execute() {
	for(auto key : pressedKeys) {
		keybindings.at(key).executeOnPress();
	}
	pressedKeys.clear();

	for(auto key : releasedKeys) {
		keybindings.at(key).executeOnRelease();
	}
	releasedKeys.clear();

	for(auto key : heldKeys) {
		keybindings.at(key).executeOnHold();
	}
}

void KeyManager::pressed(int key) {
	// Only insert it to pressedKeys and heldKeys if it is a binded key
	if(keybindings.find(key) != keybindings.end()) {
		pressedKeys.insert(key);
		heldKeys.insert(key);
	}
}

void KeyManager::released(int key) {
	// Only insert it and remove it from releasedKeys and heldKeys, respectively, if it is a binded key
	if(keybindings.find(key) != keybindings.end()) {
		releasedKeys.insert(key);
		heldKeys.erase(key);
	}
}

KeyAction& KeyManager::operator[](int key) {
	auto[it, emplaced] = keybindings.try_emplace(key, KeyAction(key));
	auto&[dummy, value] = *it;
	return value;
}

} // End of namespace lowpoly3d
