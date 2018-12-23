#ifndef BEZIER_HPP
#define BEZIER_HPP

#include <vector>
#include <array>
#include <cmath>
#include <numeric>
#include <functional> // std::invoke
#include <type_traits> // std::is_same

/* Bezier curve functions. Usage of the global bezier function should be enough for most use cases.
 * Addition and scalar multiplication has to be defined on T if FuseFunctor (= more efficient lerp) is used.
 * Indeed, implementing all operations used in BinaryOperation is sufficient for type T **/
template<typename T>
constexpr T lerp(const T& v1, const T& v2, double t) {
    return v1*t + (1.0 - t)*v2;
}

// A more efficient lerp whenever one has obj = lerp(obj, other). Instead, call fuse(obj, other).
template<typename T>
constexpr T& fuse(T& v1, const T& v2, double t) {
    return v1 = v1*t + (1.0 - t) * v2;
}

template<typename T>
struct FuseFunctor {
    constexpr T& operator()(T& v1, const T& v2, double t) const {
        return fuse(v1, v2, t);
    }
};

// A functor that outputs the point at time t on the bezier curve defined from the points
template<typename T, std::size_t N, std::size_t iter, typename BinaryInterpolator>
struct BezierFunctor {
    constexpr T operator()(std::array<T, N>& points, const double& t) const {
        static_assert(N>=2, "Bezier can only work with arrays of size two or greater");
        for(std::size_t i = 0; i < iter-1; i++) {
            points[i] = BinaryInterpolator()(points[i], points[i+1], t);
        }
        return BezierFunctor<T, N, iter-1, BinaryInterpolator>()(points, t);
    }
};

// Base-case
template<typename T, std::size_t N, typename BinaryInterpolator>
struct BezierFunctor<T, N, 1, BinaryInterpolator> {
    constexpr T operator()(const std::array<T, N>& points, const double&) const {
        return points.front();
    }
};

// Returns bezier of time t, possibly using a custom interpolator (instead of standard lerp)
template<typename T, std::size_t N, typename BinaryInterpolator = FuseFunctor<T>>
constexpr T bezier(std::array<T, N>& points, const double& t) {
    static_assert(
        std::is_invocable<BinaryInterpolator, T&, T, double>::value,
        "Expected BinaryInterpolator with signature \"double operator()(T&, const T&, double)\""
    );
    return BezierFunctor<T, N, N, BinaryInterpolator>()(points, t);
}

/* Some simple vector functionality to demonstrate beizer functionality (and generality) */
template<typename T>
struct TVec2 {
    T x = T(), y = T();
    using value_type = T;

    constexpr TVec2<T> operator-() const {
        return {-x, -y};
    }

    constexpr double length() const {
        return std::sqrt(x*x + y*y);
    }
};

template<typename T>
constexpr TVec2<T> operator*(const TVec2<T>& v, double s) {
    return {v.x * s, v.y * s};
    }

template<typename T> TVec2<T>
constexpr operator*(double s, const TVec2<T>& v) {
    return v*s;
}

template<typename T>
constexpr TVec2<T> operator+(const TVec2<T>& v1, const TVec2<T>& v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

template<typename T>
constexpr TVec2<T> operator-(const TVec2<T>& v1, const TVec2<T>& v2) {
    return v1 + (-v2);
}

template<typename T>
constexpr double dot(const TVec2<T>& v1, const TVec2<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

template<typename A, typename T>
A& operator<<(A& a, const TVec2<T>& v) {
    a << "(" << v.x << ", " << v.y << ")";
    return a;
}

using Vec2 = TVec2<double>;
using vec2 = Vec2;

#endif // BEZIER_HPP
