#ifndef KEYMANAGER_HPP
#define KEYMANAGER_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "keyaction.hpp"

namespace lowpoly3d {

class KeyAction;

/* Executes added keyactions */
class KeyManager {
public:
	// Binds a key to a keyaction
	void bind(KeyAction const& action);
	// Unbinds a previously binded key
	void unbind(int key);
	// Executes key actions
	void execute();
	// Tell keymanager that a key is pressed
	void pressed(int key);
	// Tell keymanager that a key is released
	void released(int key);
	// Return a new or existing KeyAction associated with 'key'
	KeyAction& operator[](int key);
private:
	std::unordered_set<int> heldKeys, pressedKeys, releasedKeys;
	std::unordered_map<int, KeyAction> keybindings;
};



} // End of namespace lowpoly3d

#endif // KEYMANAGER_HPP