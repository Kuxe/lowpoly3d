#ifndef APT_ASSERT_HPP
#define APT_ASSERT_HPP

#include "utils/almost_eq.hpp"

#include <iostream>
#include <source_location>

#ifdef NDEBUG
#define apt_assert(condition)                 ((void)0)
#define APT_ASSERT_LESS(a, b)                 ((void)0)
#define APT_ASSERT_LEQ(a, b)                  ((void)0)
#define APT_ASSERT_EQ(a, b)                   ((void)0)
#define APT_ASSERT_ALMOST_EQ(a, b, tolerance) ((void)0)
#define APT_ASSERT_GREATER(a, b)              ((void)0)
#define APT_ASSERT_GEQ(a, b)                  ((void)0)
#define APT_ASSERT_NEQ(a, b)                  ((void)0)
#else

static constexpr auto& apt_assert_default_out = std::cout;
using apt_assert_default_out_type = decltype(apt_assert_default_out);

namespace apt_assert_internal
{

template<
    typename T,
    typename U,
    typename Name,
    typename BinaryOpName,
    typename BinaryOp,
    typename Out = apt_assert_default_out_type
>
void apt_assert(
    Name const& nt, T const& t,
    Name const& nu, U const& u,
    BinaryOpName op_name,
    BinaryOp op,
    std::source_location const& sl,
    Out& out = apt_assert_default_out)
{
    if(!op(t, u))
    {
        out.precision(17);
        out << sl.file_name() << ":" << sl.line() << ": ";
        out << "in function `" << sl.function_name() <<"': ";
        out << "apt_assertion: `" << nt << "=" << t << " " << op_name << " " << nu << "=" << u << "' failed.\n";
        std::abort();
    }
}

#define CREATE_APT_ASSERT(function_name, op)\
template<typename T, typename U, typename Out = apt_assert_default_out_type>\
void function_name (char const* nt, T const& t, char const* nu, U const& u, std::source_location const& sl, Out& out = apt_assert_default_out)\
{\
    apt_assert(nt, t, nu, u, #op, [](T const& rhs, U const& lhs)\
    {\
        return rhs op lhs;\
    },\
    sl, out);\
}

CREATE_APT_ASSERT(apt_assert_less, <)
CREATE_APT_ASSERT(apt_assert_leq, <=)
CREATE_APT_ASSERT(apt_assert_eq, ==)

template<typename T, typename Out = apt_assert_default_out_type>
void apt_assert_almost_eq(char const* nt, T const& t, char const* nu, T const& u, T const& tolerance, std::source_location const& sl, Out& out = apt_assert_default_out)
{
	apt_assert(nt, t, nu, u, "almost_eq", [&tolerance](T const& x, T const& y) {
		return almost_eq(x, y, tolerance);
		}, sl, out);
}

CREATE_APT_ASSERT(apt_assert_greater, >)
CREATE_APT_ASSERT(apt_assert_geq, >=)
CREATE_APT_ASSERT(apt_assert_neq, !=)

} // End of namespace apt_assert_internal

#define APT_ASSERT_LESS(a, b)                 apt_assert_internal::apt_assert_less(#a, a, #b, b, std::source_location::current())
#define APT_ASSERT_LEQ(a, b)                  apt_assert_internal::apt_assert_leq(#a, a, #b, b, std::source_location::current())
#define APT_ASSERT_EQ(a, b)                   apt_assert_internal::apt_assert_eq(#a, a, #b, b, std::source_location::current())
#define APT_ASSERT_ALMOST_EQ(a, b, tolerance) apt_assert_internal::apt_assert_almost_eq(#a, a, #b, b, tolerance, std::source_location::current())
#define APT_ASSERT_GREATER(a, b)              apt_assert_internal::apt_assert_greater(#a, a, #b, std::source_location::current())
#define APT_ASSERT_GEQ(a, b)                  apt_assert_internal::apt_assert_geq(#a, a, #b, b, std::source_location::current())
#define APT_ASSERT_NEQ(a, b)                  apt_assert_internal::apt_assert_neq(#a, a, #b, b, std::source_location::current())

#endif // NDEBUG
#endif // APT_ASSERT_HPP