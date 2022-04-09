#ifndef ORIENTEDPLANE_HPP
#define ORIENTEDPLANE_HPP

#include <glm/gtx/string_cast.hpp> // glm::to_string

#include <range/v3/algorithm/transform.hpp>

namespace lowpoly3d {

template<typename floating_point_type, std::size_t dimension>
using TPoint = glm::vec<dimension, floating_point_type>;

template<typename floating_point_type, std::size_t dimension>
struct TPlane;

template<typename fpt, typename GeometricPrimitive>
struct ProjectLocal;

// An OrientedPlane is a plane with an orientation
// It is represented as a point P and a right-handed frame F instead of just a
// point P and a normal N. This enables an inverse projection function
// for points in 3D that are contained by the plane.
// OrientedPlanes can be used to solve problems in a lower dimension, follwowed
// by lifting the result back to a higher dimension.
// An OrientedPlane is more taxing on memory since it has to store a Point P
// with two base-vectors X and Y (Z is inferred from right-handedness), for a
// total of 3+3+3=9 floating points instead of 3+3=6 floating points.
template<typename fpt>
class TOrientedPlane
{
public:

using floating_point_type = fpt;
using point_type_3d = glm::vec<3, fpt>;
using point_type_2d = glm::vec<2, fpt>;

	TOrientedPlane(
		point_type_3d const& point,
		point_type_3d const& x,
		point_type_3d const& y) :
			point(point),
			x(x),
			y(y) { }
	TOrientedPlane(TOrientedPlane const& other) :
		TOrientedPlane(
			other.point,
			other.x,
			other.y) { }
	TOrientedPlane(TOrientedPlane&& other) :
		point(std::move(other.point)),
		x(std::move(other.x)),
		y(std::move(other.y)) { }
	virtual ~TOrientedPlane() { }

	TOrientedPlane& swap(TOrientedPlane plane) {
		swap(point, plane.point);
		swap(x, plane.x);
		swap(y, plane.y);
		return *this;
	}

	TOrientedPlane& operator=(TOrientedPlane plane) {
		return swap(plane);
	}

	const point_type_3d& getPoint() const { return point; }
	point_type_3d getNormal() const { return getZ(); }

	/* Returns the parameter d typically found in the plane equation, 
	 * i.e ax + by + cz + d = 0 */
	floating_point_type getD() const {
		return -glm::dot(getNormal(), getPoint());
	}

	// Returns true if point is above the plane, otherwise false
	bool above(const point_type_3d& point) const {
		return glm::dot(getNormal(), point - getPoint()) > 0.0f;
	}

	// Returns true if point is below the plane, otherwise false
	bool below(const point_type_3d& point) const {
		return glm::dot(getNormal(), point - getPoint()) < 0.0f;
	}

	// Returns true if point lies on the plane, otherwise false
	bool contains(const point_type_3d& point) const {
		// It should really be + since we want to check the difference between negative d and dot-product.
		return std::abs(glm::dot(getNormal(), point) + getD()) <= floating_point_type{1e-6};
	}

	// TODO: Create concept for anything that has a range of vertices
	template<typename GeometricPrimitive>
	auto projectLocal(GeometricPrimitive const& gp) const
	{
		return ProjectLocal<fpt, GeometricPrimitive>()(*this, gp);
	}

	// Projects a point in plane's local space to world space
	point_type_3d projectLocalInverse(point_type_2d const& point) const
	{
		return base() * point_type_3d(point.x, point.y, floating_point_type(0)) + getPoint();
	}

	glm::vec<3, fpt> const& getX() const
	{
		return x;
	}

	glm::vec<3, fpt> const& getY() const
	{
		return y;
	}

	glm::vec<3, fpt> getZ() const
	{
		return glm::cross(getX(), getY());
	}

	glm::mat<3, 3, fpt> base() const
	{
		return glm::mat<3, 3, fpt>(getX(), getY(), getZ());
	}

	glm::mat<3, 3, fpt> inverse() const
	{
		// Inverse of orthonormal matrix is transpose
		return glm::transpose(base());
	}

private:
	point_type_3d point;
	glm::vec<3, fpt> x;
	glm::vec<3, fpt> y;
};

template<typename fpt, typename GeometricPrimitive>
struct ProjectLocal {
	auto operator()(TOrientedPlane<fpt> const& plane, GeometricPrimitive const& gp) const {
		return vertices(gp) | ranges::transform([&plane](auto const& p){ return plane.projectLocal(p); });
	}
};

template<typename fpt>
struct ProjectLocal<fpt, glm::vec<3, fpt>> {
	auto operator()(TOrientedPlane<fpt> const& plane, glm::vec<3, fpt> const& point) const {
		return glm::mat2x3(plane.inverse()) * (point-plane.getPoint());
	}
};

template<typename floating_point_type>
TPlane<floating_point_type, 3> stripOrientation(TOrientedPlane<floating_point_type> const& plane) {
	return TPlane<floating_point_type, 3>(plane.getPoint(), plane.getNormal());
}

template<typename floating_point_type>
std::ostream& operator<<(std::ostream& out, TOrientedPlane<floating_point_type> const& plane) {
	out
		<< "(p=" << glm::to_string(plane.getPoint()).c_str()
		<< ", x=" << glm::to_string(plane.getX()).c_str()
		<< ", y=" << glm::to_string(plane.getY()).c_str() << ")";
	return out;
}

using OrientedPlanef = TOrientedPlane<float>;
using OrientedPlaned = TOrientedPlane<double>;
using OrientedPlane = OrientedPlanef;

} // End of namespace lowpoly3d

#endif // ORIENTEDPLANE_HPP