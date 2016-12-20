#ifndef DELTATIME_HPP
#define DELTATIME_HPP

#include <chrono>

/** DeltaTime measures time elapsed since start() and end() via the delta()-method
	Any measured dt is universal - intended use is to have a uniform dt for any class.
**/
namespace DeltaTime {
	extern float dt;
	extern std::chrono::high_resolution_clock::time_point deltaTimeStart;
	void start();
	void stop();
};

#endif //DELTATIME_HPP