#ifndef MISC_HPP
#define MISC_HPP

#include <functional>
#include <cmath>
#include <vector>

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

template<typename first_container_type, typename second_container_type>
std::vector<std::pair<
        typename first_container_type::value_type,
        typename second_container_type::value_type>>
cross(const first_container_type& c1, const second_container_type& c2) {
    using ret_type = std::vector<std::pair<
        typename first_container_type::value_type,
        typename second_container_type::value_type>>;
    ret_type ret;
    ret.reserve(c1.size() * c2.size());
    for(const auto& elem1 : c1) {
        for(const auto& elem2 : c2) {
            ret.emplace_back(elem1, elem2);
        }
    }
    return ret;
}

} // End of namespace lowpoly3d


#endif //MISC_HPP