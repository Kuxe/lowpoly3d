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

	GIVEN("A Plane((2,1,0),(-1/sqrt(2),-1/sqrt(2),0)) and a three points p1, p2, p3, with p2 being exactly above (2,1,0) in the direction of the plane-normal and the line segment (p1,p3) is perpendicular to the plane normal") {
		auto const point = Point(2.0f, 1.0f, 0.0f);
		float const cathetus = -1.0f/sqrtf(2.0f);
		auto const normal = Point(cathetus, cathetus, 0.0f);
		auto const plane = Plane(point, normal);
		Point const
			p1 = {0.0f,0.0f,0.0f},
			p2 = {1.0f,0.0f,0.0f},
			p3 = {1.0f,1.0f,0.0f};

		WHEN("Projecting all three points locally into the plane") {
			auto const
				localp1 = plane.projectIntoLocal(p1),
				localp2 = plane.projectIntoLocal(p2),
				localp3 = plane.projectIntoLocal(p3);

			THEN("The point p2 is projected into zeropoint") {
				REQUIRE(glm::all(glm::epsilonEqual(localp1, Point2f(0.0f, 0.0f), eps)));
			}

			THEN("The points p1, p3 are projected into the same point") {
				INFO("localp1=" << glm::to_string(localp1));
				INFO("localp2=" << glm::to_string(localp3));
				REQUIRE(glm::all(glm::epsilonEqual(localp1, localp3, eps)));
			}
		}
	}

	GIVEN("A plane((1,0,0),(1,0,0)) and three points p1=(0,0,0), p2=(1,0,0), p3=(1,1,0)") {
		auto const point = Point(1.0f, 0.0f, 0.0f);
		auto const normal = Point(1.0f, 0.0f, 0.0f);
		auto const plane = Plane(point, normal);
		Point const
			p1 = {0.0f,0.0f,0.0f},
			p2 = {1.0f,0.0f,0.0f},
			p3 = {1.0f,1.0f,0.0f};

		WHEN("Projecting all three points locally into the plane") {
			auto const
				localp1 = plane.projectIntoLocal(p1),
				localp2 = plane.projectIntoLocal(p2),
				localp3 = plane.projectIntoLocal(p3);

			THEN("The points p1 and p2 are projected into zeropoint") {
				REQUIRE(glm::all(glm::epsilonEqual(localp1, Point2f(0.0f, 0.0f), eps)));
				REQUIRE(glm::all(glm::epsilonEqual(localp2, Point2f(0.0f, 0.0f), eps)));
			}
		}
	}
}
