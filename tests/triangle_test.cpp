#include <catch2/catch_all.hpp>

#include "geometric_primitives/intersects.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/triangle.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "utils/glm/glmprint.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp> // glm::sphericalRand

#include <sstream>
#include <string_view>

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

SCENARIO("Triangle circumcenter") {
	auto test = [](std::string_view desc, Triangle const& given, Point const& expected) {
		std::stringstream givenss;
		givenss << desc << " " << given;
		GIVEN(givenss.str()) {
			WHEN("Computing its circumenter") {
				Point actual = circumcenter(given);
				std::stringstream ss;
				ss << "The circumcenter lies in " << glm::to_string(expected);
				THEN(ss.str()) {
					INFO("expected=" << glm::to_string(expected) << ", actual=" << glm::to_string(actual));
					REQUIRE(almostEqual(actual, expected, 1e-6f));
				}
			}
		}
	};

	test("The point-degenerate triangle", {{0,0,0},{0,0,0},{0,0,0}}, {0,0,0});
	test("The triangle", {{1,0,0},{0,1,0},{-1,0,0}}, {0,0,0});
	test("The triangle", {{1,2,3},{2,3,1},{3,1,2}}, {2,2,2});

	// Pythagorean triplet {{3,0,0},{0,0,0},{0,-4,0}}
	// distance from {3,0,0} to {0,-4,0} is 5
	// distance from {0,0,0} to {0,-4,0} is 4
	// so distance from {0,1,0} to {0,-4,0} is 5
	// Note that distance from {-3,0,0} to {0,-4,0} is also 5
	// So distance from {0,-4,0} to all triangle vertices is 5,
	// hence it is the circumcenter since a property of a circumcenter is that it
	// is the point of the circle that intersect all vertices of a triangle
	// Hence {3,0,0},{-3,0,0},{0,1,0} has circumcenter in {0,-4,0} (with radius 5)
	test("The pythagorean triplet triangle", {{3,0,0},{0,1,0},{-3,0,0}}, {0,-4,0});
}

SCENARIO("Interior points of triangles (3D)") {
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

	GIVEN("A degenerate triangle {(-1, 0, 0), (0, 0, 0), (1, 0, 0)} and a point (2, 0.1, 0)") {
		Triangle const triangle {
				{-1.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f}
			};

		Point const point {2.0f, 0.1f, 0.0f};

		WHEN("Checking if the point is contained by the degenerate triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly not contained by the degenerate triangle") {
				REQUIRE_FALSE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(-1, 0, 0), (0, 0, 0), (1, 0, 0)} and a point (2, 0, 0)") {
		Triangle const triangle {
				{-1.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f}
			};

		Point const point {2.0f, 0.0f, 0.0f};

		WHEN("Checking if the point is contained by the degenerate triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly not contained by the degenerate triangle") {
				INFO("This test is expected to (wrongly) pass for naive contains method that check if points lies on the three closed half-planes.");
				REQUIRE_FALSE(isContained);
			}
		}
	}
}

SCENARIO("Interior points of triangle (2D)") {
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
				INFO("If this test fails but the 3D version didn't fail, then something is probably wrong with 2D specializations");
				REQUIRE(isContained);
			}
		}
	}
	
	GIVEN("A degenerate triangle {(0, 2), (0, 3), (0, 3)} and a point (0, 1)") {
		auto const triangle = TTriangle<float, 2> {
				{0.0f, 2.0f},
				{0.0f, 3.0f},
				{0.0f, 3.0f}
			};

		auto const point = Point2f {0.0f, 1.0f};

		WHEN("Checking if the point is contained by the degenerate triangle") {
			bool const isContained = triangle.contains(point);

			THEN("The point is reportedly not contained by the degenerate triangle") {
				REQUIRE_FALSE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, -1/sqrt(2)), (0,0), (0, -1/sqrt(2))} and the zeropoint, which is contained by the degenerate triangle") {
		auto const triangle = Triangle2{{0.0f, -1.0f/sqrtf(2.0f)}, {0.0f, 0.0f}, {0.0f, -1.0f/sqrtf(2.0f)}};
		auto const point = Point2{0.0f, 0.0f};

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, -1/sqrt(2)), (0,0), (0, -1/sqrt(2))} and the point (0, -1/sqrt(2)), which is contained by the degenerate triangle") {
		auto const triangleVertex = Point2{0.0f, -1.0f/sqrtf(2.0f)};
		auto const triangle = Triangle2{triangleVertex, {0.0f, 0.0f}, triangleVertex};
		auto const point = triangleVertex;

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0, -1/sqrt(2)), (0,0), (0, -1/sqrt(2))} and the point (0, -1/sqrt(2)), which is contained by the degenerate triangle") {
		auto const triangleVertex = Point2{0.0f, -1.0f/sqrtf(2.0f)};
		auto const triangle = Triangle2{triangleVertex, {0.0f, 0.0f}, triangleVertex};
		auto const point = triangleVertex;

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A degenerate triangle {(0,0), (0,0), (0,1)} and the zeropoint") {
		auto const triangle = Triangle2 {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}};
		auto const point = Point2 {0.0f, 0.0f};

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A triangle {(0,0), (0,0), (0,0)} that has degenerated into a point") {
		auto const triangle = Triangle2 {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
		auto const point = Point2 {0.0f, 0.0f};

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A triangle {(0,-1), (0,0), (0,0)} that has degenerated into a line segment and a point (0,0)") {
		auto const triangle = Triangle2 {{0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
		auto const point = Point2 {0.0f, 0.0f};

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A triangle {(0,0), (0,0), (0,-1)} that has degenerated into a line segment and a point (0,0)") {
		auto const triangle = Triangle2 {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -1.0f}};
		auto const point = Point2 {0.0f, 0.0f};

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
				REQUIRE(isContained);
			}
		}
	}

	GIVEN("A triangle {(0,0), (0,0), (0,-1)} that has degenerated into a line segment and a point (0,0)") {
		auto const triangle = Triangle2 {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}};
		auto const point = Point2 {0.0f, 0.0f};

		WHEN("Checking if the zeropoint is contained by the degenerate triangle") {
			auto const isContained = triangle.contains(point);

			THEN("The zeropoint is reportedly contained by the degenerate triangle") {
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

	GIVEN("Two triangles where an edge of the first triangle intersects the surface of the other triangle ") {
		// These two triangles intersects at (2, 1, 0)
		Triangle
			t1({0,0,0},{4,0,0},{4,4,0}),
			t2({2,1,3},{2,1,-3},{2,5,1});

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

	GIVEN("A triangle {(0,0,0),(1,0,0),(0,1,0)} and another non-intersecting triangle {(5,0,0),(6,0,0),(5,1,0)}") {
		Triangle const
			t1{{0,0,0},{1,0,0},{0,1,0}},
			t2{{5,0,0},{6,0,0},{5,1,0}};

		WHEN("Checking if they intersect") {
			bool const intersecting = intersects(t1, t2);
			THEN("They are reported as non-intersecting") {
				REQUIRE_FALSE(intersecting);
			}
		}
	}
}

}
