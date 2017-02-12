#ifndef MISC_HPP
#define MISC_HPP

#include <functional>

/** misc.hpp contains miscellaneous reusable functions inspired from functional programming **/

namespace lowpoly3d {

/** misc.hpp contains miscellaneous reusable functions inspired from functional programming **/

/** zip is a function which given two containers, iterates through the pairs given by
		(a1, b1), (a2, b2), ..., (an, bn)
	and replaces the elements of a with
		f(a1, b1), f(a2, b2), ..., f(an, bn)
	It is required that a and b have the same cardinality **/
//FIXME: std::function signature should have same type as type of element in container. decltype(A::value_type) won't do the trick...
template<typename A, typename B>
void inplace_zip(A& a, const B& b, const std::function<float(float, float)> f) {
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
A zip(const A& a, const B& b, const std::function<A(float, float)> f) {
	A c(a);
	inplace_zip(c, b, f);
	return c;
}

}


#endif //MISC_HPP