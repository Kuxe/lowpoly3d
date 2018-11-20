#ifndef LERP_HPP
#define LERP_HPP

#include "utils/arithmetic_invariant.hpp"

namespace lowpoly3d {

template<typename value_type, typename parameter_type>
value_type lerp(const value_type& a, const value_type& b, LerpParameter<parameter_type> t) {
	return (parameter_type(1) - t) * a + t*b;
}

template<typename value_type, typename parameter_type>
value_type lerp(const value_type& a, const value_type& b, parameter_type t) {
	return lerp(a, b, LerpParameter<parameter_type>(t));
}

template<typename value_type, typename parameter_type>
value_type bilerp(
	const value_type& topleft,
	const value_type& topright,
	const value_type& bottomleft,
	const value_type& bottomright,
	LerpParameter<parameter_type> u, LerpParameter<parameter_type> v) {

	return lerp(lerp(topleft, topright, u),	lerp(bottomleft, bottomright, u), v);
}

template<typename value_type, typename parameter_type>
value_type bilerp(
	const value_type& topleft,
	const value_type& topright,
	const value_type& bottomleft,
	const value_type& bottomright,
	parameter_type u, parameter_type v) {

	return bilerp(
		topleft, topright, bottomleft, bottomright,
		LerpParameter<parameter_type>(u), LerpParameter<parameter_type>(v));
}

// Returns t with value = lerp(start, end, t). If value != lerp(start, end, t),
// for all t, then returned t will be either smaller than 0 or greater than 1.
template<typename floating_point_type>
bool getLerpT(
    floating_point_type start,
    floating_point_type end,
    floating_point_type value) {
    static_assert(std::is_floating_point<floating_point_type>::value, "floating_point_type must be a floating point type...");

    assert(end != start);
    return (value - start) / (end - start);
}

} // End of namespace lowpoly3d

#endif // LERP_HPP