#include <catch.hpp>

#include "geometric_primitives/intersections.hpp"
#include "geometric_primitives/triangle.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace lowpoly3d {

SCENARIO("Triangle misc methods") {
	GIVEN("The triangle ({0,0,0},{3,0,0},{0,4,0})") {
		Triangle triangle({0,0,0},{3,0,0},{0,4,0});
		WHEN("Computing its area") {
			auto const area = triangle.area();
			THEN("The area is 3*4/2=6") {
				INFO("triangle=" << triangle);
				REQUIRE(area == 6.0);
			}
		}
	}

	GIVEN("The triangle ({0,0,0},{0,-8,0},{2,-3,0})") {
		Triangle triangle({0,0,0},{0,-8,0},{2,-3,0});
		WHEN("Computing its area") {
			auto const area = triangle.area();
			THEN("The area is 3*2/2 + 5*2/2 = 8") {
				INFO("triangle=" << triangle);
				REQUIRE(area == 8.0);
			}
		}
	}
}

SCENARIO("Triangle intersection") {

	auto intersectsCommutativityCheck = [](Triangle const& t1, Triangle const t2) {
		REQUIRE(intersects(t1, t2) == intersects(t2, t1));
	};

	GIVEN("Two non-intersecting triangles") {
		Triangle
			t1({0,0,0},{1,0,0},{1,1,0}),
			t2({2,0,0},{3,0,0},{3,1,0});

		WHEN("Checking if they intersect") {
			bool intersecting = intersects(t1, t2);
			THEN("They are reported as non-intersecting") {
				REQUIRE_FALSE(intersecting);
			}
		}

		intersectsCommutativityCheck(t1, t2);
	}

	GIVEN("Two triangles that touch at a point") {
		Triangle
			t1({0,0,0},{1,0,0},{1,1,0}),
			t2({1,0,0},{2,0,0},{2,1,0});

		WHEN("Checking if they intersect") {
			bool intersecting = intersects(t1, t2);
			THEN("They are reported as non-intersecting") {
				REQUIRE_FALSE(intersecting);
			}
		}

		intersectsCommutativityCheck(t1, t2);
	}

	GIVEN("Two triangles that touch along an edge") {
		Triangle
			t1({0,0,0},{1,0,0},{1,1,0}),
			t2({1,0,0},{2,0,0},{1,1,0});

		WHEN("Checking if they intersect") {
			bool intersecting = intersects(t1, t2);
			THEN("They are reported as non-intersecting") {
				REQUIRE_FALSE(intersecting);
			}
		}

		intersectsCommutativityCheck(t1, t2);
	}

	GIVEN("Two identical triangles but different translations along z-axis") {
		Triangle
			t1({0,0,0},{1,0,0},{1,1,0}),
			t2({0,0,1},{1,0,1},{1,1,1});

		WHEN("Checking if they intersect") {
			bool intersecting = intersects(t1, t2);
			THEN("They are reported as non-intersecting") {
				REQUIRE_FALSE(intersecting);
			}
		}

		intersectsCommutativityCheck(t1, t2);
	}

	GIVEN("Two triangles that have intersecting points and intersecting edges") {
		Triangle
			t1({0,0,0},{1,0,0},{1,1,0}),
			t2({0,0,0},{1,0,0},{0,1,0});

		WHEN("Checking if they intersect") {
			bool intersecting = intersects(t1, t2);
			THEN("They are reported as intersecting") {
				REQUIRE(intersecting);
			}
		}

		intersectsCommutativityCheck(t1, t2);
	}

	GIVEN("Two triangles where an egde of the first triangle intersects the surface of the other triangle ") {
		// These two triangles intersects at (2, 1, 0)
		Triangle
			t1({0,0,0},{4,0,0},{4,4,0}),
			t2({2,1,-3},{2,1,-3},{2,5,1});

		WHEN("Checking if they intersect") {
			bool intersecting = intersects(t1, t2);
			THEN("They are reported as intersecting") {
				REQUIRE(intersecting);
			}
		}

		intersectsCommutativityCheck(t1, t2);
	}

	GIVEN("A triangle that is completely contained within another larger") {
		Triangle
			large({0,0,0},{4,0,0},{4,4,0}),
			small({2,1,0},{3,1,0},{3,2,0});

		WHEN("Checking if they intersect") {
			bool intersecting = intersects(large, small);
			THEN("They are reported as intersecting") {
				REQUIRE(intersecting);
			}
		}

		intersectsCommutativityCheck(large, small);
	}
}

}
