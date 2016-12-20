#ifndef PHASE_HPP
#define PHASE_HPP

#include <unordered_set>

class PhaseBase {

};

/** A phase can be active or inactive. While active, it's list of actions is iterated
	and each action is executed. **/
template<typename T>
class Phase {
private:
	std::unordered_set<std::function<void(void)> f> commands;

public:
	/** add command (function with void(void) signature) to this phase and return id of command **/
	void add(const std::function<void(void)>& f) {
		commands.insert(f);
	}

	void executeCommands() {
		for(const auto f& : commands) {
			f();
		}
	}


	/** Statics **/
private:
	static std::unordered_set<Phase<
public:
	static handlePhases() {

	}

};

#endif //PHASE_HPP