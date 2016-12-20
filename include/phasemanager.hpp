#ifndef PHASEMANAGER_HPP
#define PHASEMANAGER_HPP

#include "phase.hpp"

class PhaseManager {
private:
	std::unordered_set<Phase> phases;
	std::unordered_set<Phase> activePhases;
	std::unordered_set<Phase> inactivePhases;

public:
	void executePhases() {
		for(const Phase& phase : activePhases) {
			phase.executeCommands();
		}
	}

};

#endif //PHASEMANAGER_HPP