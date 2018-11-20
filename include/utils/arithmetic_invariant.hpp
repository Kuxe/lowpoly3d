#ifndef ARITHMETIC_INVARIANT_HPP
#define ARITHMETIC_INVARIANT_HPP

#include <cassert>
#include <functional>

namespace lowpoly3d {

static constexpr bool ENABLE_THROW_ON_INVARIANT_BROKEN = true;

struct not_invariant_exception final : std::runtime_error {
    not_invariant_exception() : std::runtime_error("Invariant violation") { }
};

template<typename arithmetic_type>
struct in_unit_interval {
    static_assert(std::is_arithmetic<arithmetic_type>::value,
        "arithmetic_type must satisfy std::is_arithmetic");

    static bool check(arithmetic_type number) {
        return 0 <= number && number <= 1;
    }
};

/* arithmetic_invariant is a wrapper around an arithmetic type
 * that runtime-checks if the invariant_unary_checker returns true on
 * construction or assignment. If it returns false, an exception is thrown.
 * This is useful for a declarative style of programming where
 * compile-time checks are not possible (for instance, checking that some
 * user-supplied floating-point number is within [0, 1]).
 * 
 * Use arithmetic_invariant if you are tired of writing lots of if-checks
 * or assertions that checks some condition */
template<typename arithmetic_type, typename invariant_unary_checker>
struct arithmetic_invariant : invariant_unary_checker {
    static_assert(std::is_arithmetic<arithmetic_type>::value,
        "arithmetic_type must satisfy std::is_arithmetic");

    arithmetic_type number;

    arithmetic_invariant(arithmetic_type number) : number(number) {
        if(ENABLE_THROW_ON_INVARIANT_BROKEN) {
            if(!invariant_unary_checker::check(number)) throw not_invariant_exception();
        }
    }

    arithmetic_invariant(const arithmetic_invariant<arithmetic_type, invariant_unary_checker>& number)
        : arithmetic_invariant<arithmetic_type, invariant_unary_checker>(number.number) { }

    template<typename other_arithmetic_type>
    arithmetic_invariant operator=(other_arithmetic_type other) {
        static_assert(std::is_arithmetic<other_arithmetic_type>::value,
        "Can only assign arithmetic types to arithmetic_invariants");
        if(ENABLE_THROW_ON_INVARIANT_BROKEN) {
            if(!invariant_unary_checker::check(other)) throw not_invariant_exception();
        }
        std::swap(number, other.number);
        return *this;
    }

    arithmetic_type operator-() const {
        return -number;
    }

    /**************************/
    /** Comparison operators **/
    /**************************/
	template<typename other_arithmetic_type>
    bool operator<(arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number < other.number;
    }

    template<typename other_arithmetic_type>
    bool operator<=(arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number <= other.number;
    }

    template<typename other_arithmetic_type>
    bool operator>(arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number > other.number;
    }

    template<typename other_arithmetic_type>
    bool operator>=(arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number >= other.number;
    }

    template<typename other_arithmetic_type>
    bool operator==(arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number == other.number;
    }

    template<typename other_arithmetic_type>
    bool operator!=(arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) const {
        return number != other.number;
    }

    template<typename other_arithmetic_type>
    bool operator<(other_arithmetic_type other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number < other;
    }

    template<typename other_arithmetic_type>
    bool operator<=(other_arithmetic_type other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number <= other;
    }

    template<typename other_arithmetic_type>
    bool operator>(other_arithmetic_type other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number > other;
    }

    template<typename other_arithmetic_type>
    bool operator>=(other_arithmetic_type other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number >= other;
    }

    template<typename other_arithmetic_type>
    bool operator==(other_arithmetic_type other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number == other;
    }

    template<typename other_arithmetic_type>
    bool operator!=(other_arithmetic_type other) const {
		static_assert(std::is_arithmetic<other_arithmetic_type>::value, "other_arithmetic_type must be arithmetic");
        return number != other;
    }
};

// Addition
template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator+(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) {
	return number.number + other.number;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator+(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const other_arithmetic_type other) {
    return number.number + other;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
other_arithmetic_type operator+(const other_arithmetic_type other, const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number) {
    return number.number + other;
}

// Subtraction
template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator-(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) {
	return number.number - other.number;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator-(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const other_arithmetic_type other) {
    return number.number - other;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
other_arithmetic_type operator-(const other_arithmetic_type other, const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number) {
    return other - number.number;
}

// Multiplication
template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator*(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) {
	return number.number * other.number;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator*(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const other_arithmetic_type other) {
    return number.number * other;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
other_arithmetic_type operator*(const other_arithmetic_type other, const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number) {
    return number.number * other;
}

// Division
template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator/(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const arithmetic_invariant<other_arithmetic_type, invariant_unary_checker> other) {
	return arithmetic_type(1) / number.number * other;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
arithmetic_type operator/(const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number, const other_arithmetic_type other) {
    return number.number / other;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
other_arithmetic_type operator/(const other_arithmetic_type other, const arithmetic_invariant<arithmetic_type, invariant_unary_checker> number) {
    return number.number / other;
}

template<typename T, typename U, typename V>
bool operator<(U other, T number) {
	return number >= other;
}

template<typename T, typename U, typename V>
bool operator<=(U other, T number) {
	return number > other;
}

template<typename T, typename U, typename V>
bool operator>(U other, T number) {
	return number <= other;
}

template<typename T, typename U, typename V>
bool operator>=(U other, T number) {
	return other < number;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
bool operator==(
	other_arithmetic_type other,
	arithmetic_invariant<arithmetic_type, invariant_unary_checker> number) {
	return number == other;
}

template<typename arithmetic_type, typename other_arithmetic_type, typename invariant_unary_checker>
bool operator!=(
	other_arithmetic_type other,
	arithmetic_invariant<arithmetic_type, invariant_unary_checker> number) {
	return number != other;
}

template<typename arithmetic_type>
struct true_invariant {
    static bool check(arithmetic_type) {
        return true;
    }
};

template<typename arithmetic_type>
struct false_invariant {
    static bool check(arithmetic_type) {
        return false;
    }
};

template<typename arithmetic_type>
using arithmetic_true_invariant = arithmetic_invariant<arithmetic_type, true_invariant<arithmetic_type>>;
template<typename arithmetic_type>
using arithmetic_false_invariant = arithmetic_invariant<arithmetic_type, false_invariant<arithmetic_type>>;

/* A class that guarantees that numbers lie in [0, 1].
 * This class incurs performance overhead since it accomplishes
 * said guarantee by if-checks */
template<typename floating_point_type>
using LerpParameter = arithmetic_invariant<
    floating_point_type,
    in_unit_interval<floating_point_type>>;

} // End of namespace lowpoly3d

#endif // ARITHMETIC_INVARIANT_HPP