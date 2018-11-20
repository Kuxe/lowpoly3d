#ifndef MISC_HPP
#define MISC_HPP

#include <functional>
#include <cmath>

/** misc.hpp contains miscellaneous reusable functions **/

namespace lowpoly3d {

/** zip is a function which given two containers, iterates through the pairs given by
		(a1, b1), (a2, b2), ..., (an, bn)
	and replaces the elements of a with
		f(a1, b1), f(a2, b2), ..., f(an, bn)
	It is required that a and b have the same cardinality **/
template<typename A, typename B>
void inplace_zip(A& a, const B& b, const std::function<typename A::value_type(typename A::value_type, typename B::value_type)> f) {
	typename B::const_iterator bit = b.begin(); 
	for(auto& ait : a) {
		ait = f(ait, *(bit++));
	}
}

/** zip is a function which given two containers, iterates through the pairs given by
		(a1, b1), (a2, b2), ..., (an, bn)
	and produces a new container of type Container1 containing the elements
		f(a1, b1), f(a2, b2), ..., f(an, bn)
	It is required that a and b have the same cardinality **/
template<typename A, typename B>
A zip(const A& a, const B& b, const std::function<A(typename A::value_type, typename B::value_type)> f) {
	A c(a);
	inplace_zip(c, b, f);
	return c;
}

} // End of namespace lowpoly3d


#endif //MISC_HPP