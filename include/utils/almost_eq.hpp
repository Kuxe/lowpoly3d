#ifndef ALMOST_EQ
#define ALMOST_EQ

#include <algorithm>
#include <numeric>

// Compares two floating point numbers with a relative tolerance
// Source: https://www.reidatcheson.com/floating%20point/comparison/2019/03/20/floating-point-comparison.html
template<typename T>
bool almost_eq(T x, T y, T tolerance)
{
	auto const smallest = std::min(std::abs(x), std::abs(y));
	return (smallest == T(0) && std::abs(x-y) < tolerance)
		|| std::abs(x-y) / std::max(std::numeric_limits<T>::min(), smallest) < tolerance;
}

#endif // ALMOST_EQ