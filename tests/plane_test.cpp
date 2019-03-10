#include <catch.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/epsilon.hpp> //glm::epsilonEqual
#include <glm/gtx/string_cast.hpp> //glm::vec3 into std::cout

#include "geometric_primitives/plane.hpp"

namespace Catch {
    template<>
    struct StringMaker<glm::vec3> {
        static std::string convert(glm::vec3 const& value ) {
            return glm::to_string(value);
        }
    };
}

SCENARIO("Constructing planes") {
	using namespace lowpoly3d;

	GIVEN("The zero point and the unit vector (0, 0, 1)") {
		Point const point {0.0f, 0.0f, 0.0f};
		Point const normal {0.0f, 0.0f, 1.0f};

		WHEN("Constructing a plane with the point and vector as arguments") {
			Plane const plane {point, normal};

			THEN("The plane point did not change") {
				INFO("plane.getPoint()=" << glm::to_string(plane.getPoint()));
				REQUIRE(plane.getPoint() == point);
			}

			THEN("The plane normal did not change") {
				INFO("plane.getNormal()=" << glm::to_string(plane.getNormal()));
				REQUIRE(plane.getNormal() == normal);
			}

			THEN("The d part of the plane is reportedly 0") {
				REQUIRE(plane.getD() == 0.0f);
			}
		}
	}
}

SCENARIO("Projecting points onto plane") {
	using namespace lowpoly3d;

	float eps = std::numeric_limits<float>::epsilon();
	glm::vec3 zeropoint {0.0f, 0.0f, 0.0f};

	GIVEN("The XY-plane and the point (0, 0, 1)") {
		Plane const plane {zeropoint, Point{0.0f, 0.0f, 1.0f}};
		Point const point {0.0f, 0.0f, 1.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.project(point);
			THEN("The zero point is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, zeropoint, eps)));
			}
		}
	}
}

SCENARIO("Projecting points onto local plane") {
	using namespace lowpoly3d;

	float eps = std::numeric_limits<float>::epsilon();
	glm::vec3 zeropoint3d {0.0f, 0.0f, 0.0f};
	glm::vec2 zeropoint2d {0.0f, 0.0f};

	GIVEN("The XZ-plane and the point (0, 1, 0)") {
		Plane const plane {zeropoint3d, Point{0.0f, 1.0f, 0.0f}};
		Point const point {0.0f, 1.0f, 0.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.projectIntoLocal(point);
			THEN("The zero point is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, zeropoint2d, eps)));
			}
		}
	}

	GIVEN("The XZ-plane and the point (0, 2, 0)") {
		Plane const plane {zeropoint3d, Point{0.0f, 1.0f, 0.0f}};
		Point const point {0.0f, 1.0f, 0.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.projectIntoLocal(point);
			THEN("The zero point is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, zeropoint2d, eps)));
			}
		}
	}

	GIVEN("The XZ-plane and the point (1, 1, 1)") {
		Plane const plane {zeropoint3d, Point{0.0f, 1.0f, 0.0f}};
		Point const point {1.0f, 1.0f, 1.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.projectIntoLocal(point);
			THEN("The point (1, 1) is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, TPoint<float, 2>(1.0f, 1.0f), eps)));
			}
		}
	}

	GIVEN("The XZ-plane and the point (1, 0, 1)") {
		Plane const plane {zeropoint3d, Point{0.0f, 1.0f, 0.0f}};
		Point const point {1.0f, 0.0f, 1.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.projectIntoLocal(point);
			THEN("The point (1, 1) is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, TPoint<float, 2>(1.0f, 1.0f), eps)));
			}
		}
	}

	GIVEN("The XZ-plane and the point (1, -1, 1)") {
		Plane const plane {zeropoint3d, Point{0.0f, 1.0f, 0.0f}};
		Point const point {1.0f, 0.0f, 1.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.projectIntoLocal(point);
			THEN("The point (1, 1) is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, TPoint<float, 2>(1.0f, 1.0f), eps)));
			}
		}
	}

	GIVEN("The XZ-plane and the point (1, 1, 2)") {
		Plane const plane {zeropoint3d, Point{0.0f, 1.0f, 0.0f}};
		Point const point {1.0f, 1.0f, 2.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.projectIntoLocal(point);
			THEN("The point (1, 2) is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, TPoint<float, 2>(2.0f, 1.0f), eps)));
			}
		}
	}

	GIVEN("The XZ-plane and the point (1, 1, 2)") {
		Plane const plane {zeropoint3d, Point{0.0f, 1.0f, 0.0f}};
		Point const point {1.0f, 1.0f, 2.0f};

		WHEN("Projecting the point onto the plane") {
			auto const projectedPoint = plane.projectIntoLocal(point);
			THEN("The point (1, 2) is obtained") {
				INFO("projectedPoint=" << glm::to_string(projectedPoint));
				REQUIRE(glm::all(glm::epsilonEqual(projectedPoint, TPoint<float, 2>(2.0f, 1.0f), eps)));
			}
		}
	}
}