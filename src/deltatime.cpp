#include "deltatime.hpp"
#include <iostream>

namespace DeltaTime {
	float dt = 0.016f;
	std::chrono::high_resolution_clock::time_point deltaTimeStart;

	void start() {
		using namespace std::chrono;
		deltaTimeStart = high_resolution_clock::now();
	}

	void stop() {
		using namespace std::chrono;
		dt = duration_cast<duration<float, std::milli>>(high_resolution_clock::now() - deltaTimeStart).count() / 1000.0f;
	}
}





