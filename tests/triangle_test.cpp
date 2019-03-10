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

SCENARIO("Interior points of triangles") {
	GIVEN("A triangle {(0, 0, 0), (3, 0, 0), (0, 3, 0)} and a point (1, 1, 0) that lie inside said triangle") {
		Triangle const triangle {
			{0.0f, 0.0f, 0.0f},
			{3.0f, 0.0f, 0.0f},
			{0.0f, 3.0f, 0.0f}
		};

		Point const point {1.0f, 1.0f, 0.0f};

		WHEN("Checking if the point is contained by the triangle") {
			bool const isContained = triangle.contains(point);
			
			THEN("The point is reportedly contained by the triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A triangle {(0, 0, 0), (3, 0, 0), (0, 3, 0)} and a point (2, 2, 0) that lie outside said triangle") {
		Triangle const triangle {
			{0.0f, 0.0f, 0.0f},
			{3.0f, 0.0f, 0.0f},
			{0.0f, 3.0f, 0.0f}
		};

		Point const point {2.0f, 2.0f, 0.0f};

		WHEN("Checking if the point is contained by the triangle") {
			bool const isContained = triangle.contains(point);
			
			THEN("The point is reportedly not contained by the triangle") {
				REQUIRE_FALSE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, 0, 0), (1, 1, 0), (-1, -1, 0) and a point within the degenerate triangle (0, 0, 0)") {
		Triangle const triangle {
				{0.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 0.0f},
				{-1.0f, -1.0f, 0.0f}
			};

		Point const point {0.0f, 0.0f, 0.0f};
		
		WHEN("Checking if the point is contained by the triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly contained by the triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, 0, 0), (1, 1, 0), (-1, -1, 0) and a point within the degenerate triangle (1, 1, 0)") {
		Triangle const triangle {
				{0.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 0.0f},
				{-1.0f, -1.0f, 0.0f}
			};

		Point const point {1.0f, 1.0f, 0.0f};
		
		WHEN("Checking if the point is contained by the triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly contained by the triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, 0, 0), (1, 1, 0), (-1, -1, 0) and a point within the degenerate triangle (-1, -1, 0)") {
		Triangle const triangle {
				{0.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 0.0f},
				{-1.0f, -1.0f, 0.0f}
			};

		Point const point {-1.0f, -1.0f, 0.0f};
		
		WHEN("Checking if the point is contained by the triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly contained by the triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, 0, 0), (0, 1/sqrt(2), 0), (0, sqrt(2), 0)} and a point on that triangle (0, 1/sqrt(2), 0)") {
		Triangle const triangle {
				{0.0f, 0.0f, 0.0f},
				{0.0f, 1.0f/sqrtf(2.0f), 0.0f},
				{0.0f, sqrtf(2.0f), 0.0f}
			};

		Point const point {0.0f, 1.0f/sqrtf(2.0f), 0.0f};

		WHEN("Checking if the point is contained by the triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly contained by the triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, 0), (0, 1/sqrt(2)), (0, sqrt(2))} and a point on that triangle (0, 1/sqrt(2))") {
		TTriangle<float, 2> const triangle {
				{0.0f, 0.0f},
				{0.0f, 1.0f/sqrtf(2.0f)},
				{0.0f, sqrtf(2.0f)}
			};

		TPoint<float, 2> const point {0.0f, 1.0f/sqrtf(2.0f)};

		WHEN("Checking if the point is contained by the triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly contained by the triangle") {
				INFO("If this test fails but the 3D version didnt fail, then something is probably wrong with 2D specializations");
				REQUIRE(isContained);
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
			THEN("They are reported as intersecting") {
				REQUIRE(intersecting);
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
			THEN("They are reported as intersecting") {
				REQUIRE(intersecting);
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
