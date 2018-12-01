#include "keymanager.hpp"

namespace lowpoly3d {

KeyManager& KeyManager::bind(int key, const KeyAction& action) {
	keybindings.emplace(key, action);
	return *this;
}

KeyManager& KeyManager::unbind(int key) {
	keybindings.erase(key);
	return *this;
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

} // End of namespace lowpoly3d
