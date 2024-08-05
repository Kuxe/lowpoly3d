#include <cassert>

#include "geometric_primitives/point.hpp"
#include "geometric_primitives/sphere.hpp"
#include "geometric_primitives/triangle.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp" // glm::distance2

#include "minimum_bounding_sphere.hpp"

#include "utils/glm/glmprint.hpp"
#include "utils/apt_assert.hpp"

#include <glm/detail/qualifier.hpp>

namespace lowpoly3d
{

// Returns a minimum bounding sphere over a triangle defined by vertices a, b and c
template<typename fpt, std::size_t dim>
TSphere<fpt, dim> mbs(TTriangle<fpt, dim> const& t) {

	auto const& a = t[0];
	auto const& b = t[1];
	auto const& c = t[2];
	

	// Check that input points are not NaN
	APT_ASSERT_EQ(a, a);
	APT_ASSERT_EQ(b, b);
	APT_ASSERT_EQ(c, c);

	/* Check if MBS is midpoint of longest side, if so, return MBS at the midpoint */
	fpt const abside2 = glm::distance2(a, b);
	fpt const acside2 = glm::distance2(a, c);
	fpt const bcside2 = glm::distance2(b, c);

	using point_type = TPoint<fpt, dim>;
	using pair_point_type = std::pair<point_type, point_type>;
	pair_point_type farthestApartPoints;
	point_type otherPoint;
	fpt longestSide;

	if(abside2 > acside2) {
		if(abside2 > bcside2) {
			farthestApartPoints = pair_point_type{a, b};
			otherPoint = c;
			longestSide = abside2;
		} else {
			farthestApartPoints = pair_point_type{b, c};
			otherPoint = a;
			longestSide = bcside2;
		}
	} else {
		if(acside2 > bcside2) {
			farthestApartPoints = pair_point_type{a, c};
			otherPoint = b;
			longestSide = acside2;
		} else {
			farthestApartPoints = pair_point_type{b, c};
			otherPoint = a;
			longestSide = bcside2;
		}
	}

	{
		auto const midpoint = fpt(0.5)*(farthestApartPoints.first + farthestApartPoints.second);
		auto const radius = fpt(0.5)*std::sqrt(longestSide);
		TSphere<fpt, dim> ret(midpoint, radius);
		if(ret.contains(otherPoint)) {
			APT_ASSERT_LEQ(glm::distance(ret.p, a), ret.r + fpt(1e-4));
			APT_ASSERT_LEQ(glm::distance(ret.p, b), ret.r + fpt(1e-4));
			APT_ASSERT_LEQ(glm::distance(ret.p, c), ret.r + fpt(1e-4));
			return ret;
		}
	}

	{
		/* Let t=(p1,p2,3), then the center and radius of its MBS is given by:
		* 
		* (1)	||p1-c|| = r
		* (2)	||p2-c|| = r
		* (3)	||p3-c|| = r
		* 
		* Then:
		* =>	||p1-c|| = ||p2-c||, all solutions for c give a plane 
		* =>	||p1-c|| = ||p3-c||, all solutions for c give another plane
		* =>	||p2-c|| = ||p3-c||, all solutions for c give the last plane
		* 
		* So there exist exactly one c that satisfy (1-3), namely the intersection
		* of all 3 planes, which yields a point (if the three planes have different normals) */ 

		auto const cc = circumcenter(TTriangle<fpt, dim>(a, b, c));
		auto const r = glm::length(a - cc);

		// Check that (1), (2) and (3) hold
		APT_ASSERT_ALMOST_EQ(glm::length(a - cc), r, fpt(1e-4));
		APT_ASSERT_ALMOST_EQ(glm::length(b - cc), r, fpt(1e-4));
		APT_ASSERT_ALMOST_EQ(glm::length(c - cc), r, fpt(1e-4));

		// Check that circumcenter has no NaN components and that radius is not NaN
		APT_ASSERT_EQ(cc.x, cc.x);
		APT_ASSERT_EQ(cc.y, cc.y);
		APT_ASSERT_EQ(cc.z, cc.z);
		APT_ASSERT_EQ(r, r);

		TSphere<fpt, dim> ret {cc, r};
		assert(ret.contains(t));
		return ret;
	}
}

// Finds a minimum bounding sphere that encloses two spheres a and b
template<typename fpt, std::size_t dim>
TSphere<fpt, dim> mbs(
	const TSphere<fpt, dim>& a,
	const TSphere<fpt, dim>& b) {
	
	// Check that the radii of a and b are not NaN
	APT_ASSERT_EQ(a.r, a.r);
	APT_ASSERT_EQ(b.r, b.r);

	using vec_type = typename TSphere<fpt, dim>::vec_type;

	const vec_type diff = b.p - a.p;
	const fpt d = ::glm::length(diff);
	APT_ASSERT_EQ(d, d);

	// FIXME: d can be zero, causing a divide by zero
	// If one sphere is enclosed by the other sphere, return the bigger sphere
	const fpt radiidiff = std::abs(a.r - b.r);
	if (d <= radiidiff) {
		// One sphere is fully enclosed in the other, return the biggest sphere
		return a.r > b.r ? a : b; 
	} else {
		const vec_type unitdiff = diff / d;
		const vec_type e2 = b.p + unitdiff * b.r;
		const vec_type e1 = a.p - unitdiff * a.r;
		const vec_type midpoint = fpt(0.5)*(e1 + e2);

		const fpt enclosing_radius = fpt(0.5) * (d + a.r + b.r); // Since glm::distance(e1, e2) / 2.0f = (d + a.r + b.r) / 2.0f

		// Sanity checks
		{
			APT_ASSERT_GEQ(enclosing_radius + std::numeric_limits<fpt>::epsilon(), a.r);
			APT_ASSERT_GEQ(enclosing_radius + std::numeric_limits<fpt>::epsilon(), b.r);
			{
				[[maybe_unused]] auto const ok = TSphere<fpt, dim>(midpoint, enclosing_radius).encloses(a);
				assert(ok);
			}
			{
				[[maybe_unused]] auto const ok = TSphere<fpt, dim>(midpoint, enclosing_radius).encloses(b);
				assert(ok);
			}
		}
		return {midpoint, enclosing_radius};
	}
}

template TSphere< float, 3> mbs(TTriangle< float, 3> const&);
template TSphere<double, 3> mbs(TTriangle<double, 3> const&);

template TSphere< float, 3> mbs(TSphere< float, 3> const&, TSphere< float, 3> const&);
template TSphere<double, 3> mbs(TSphere<double, 3> const&, TSphere<double, 3> const&);

} // End of namespace lowpoly3d