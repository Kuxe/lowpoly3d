#include <catch.hpp>
#include <iostream>

#include "geometric_primitives/intersections.hpp"
#include "geometric_primitives/point.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/epsilon.hpp> //glm::epsilonEqual
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp> //glm::vec3 into std::cout

namespace Catch {
    template<>
    struct StringMaker<glm::vec3> {
        static std::string convert(glm::vec3 const& value ) {
            return glm::to_string(value);
        }
    };
}

SCENARIO("Intersection tests") {
	using namespace lowpoly3d;

	const glm::vec3 zerovec = {0.0f, 0.0f, 0.0f};
	const glm::vec3 xaxis = {1.0f, 0.0f, 0.0f};
	const glm::vec3 yaxis = {0.0f, 1.0f, 0.0};
	const glm::vec3 zaxis = {0.0f, 0.0f, 1.0f};
	const glm::vec3 nxaxis = -xaxis;
	const glm::vec3 nyaxis = -yaxis;
	const glm::vec3 nzaxis = -zaxis;
	const Plane xyplane = {zerovec, {0.0f, 0.0f, 1.0f}};
	const Plane xzplane = {zerovec, {0.0f, 1.0f, 0.0f}};
	const Plane yzplane = {zerovec, {1.0f, 0.0f, 0.0f}};

	static constexpr auto eps = std::numeric_limits<float>::epsilon();

	const auto printvec = [](const glm::vec3& vec) {
		std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
	};

	GIVEN("The XY-plane and XZ-plane") {
		WHEN("Computing the their line of intersection") {
			const auto line = intersection(xyplane, xzplane);
			THEN("The line direction is the negative xaxis") {
				REQUIRE(glm::all(glm::epsilonEqual(line.getDirection(), nxaxis, eps)));
			}
			THEN("The line only intersects the xaxis") {
				REQUIRE(line.getPoint().y == 0.0f);
				REQUIRE(line.getPoint().z == 0.0f);
			}
		}
	}

	GIVEN("The XY-plane and YZ-plane") {
		WHEN("Computing the their line of intersection") {
			const auto line = intersection(xyplane, yzplane);
			THEN("The line direction is the positive yaxis") {
				REQUIRE(glm::all(glm::epsilonEqual(line.getDirection(), yaxis, eps)));
			}
			THEN("The line only intersects the yaxis") {
				REQUIRE(line.getPoint().x == 0.0f);
				REQUIRE(line.getPoint().z == 0.0f);
			}
		}
	}

	GIVEN("The XZ-plane and YZ-plane") {
		WHEN("Computing the their line of intersection") {
			const auto line = intersection(xzplane, yzplane);
			THEN("The line direction is the negative zaxis") {
				REQUIRE(glm::all(glm::epsilonEqual(line.getDirection(), nzaxis, eps)));
			}
			THEN("The line only intersects the xaxis") {
				REQUIRE(line.getPoint().x == 0.0f);
				REQUIRE(line.getPoint().y == 0.0f);
			}
		}
	}

	GIVEN("A bulk of randomly generated pairs of parallell planes") {

		for(std::size_t i = 0; i < 100; i++) {
			const auto point = glm::sphericalRand(1.0f);
			const auto normal = glm::sphericalRand(1.0f);
			const auto pointScaled1 = glm::linearRand(0.0f, 1.0f) * point;
			const auto pointScaled2 = glm::linearRand(0.0f, 1.0f) * point;

			const Plane plane1 = {pointScaled1, normal};
			const Plane plane2 = {pointScaled2, normal};

			WHEN("Computing the line of intersection of a randomly generated plane-pair") {
				const auto line = intersection(plane1, plane2);
				THEN("The reported line of intersection has the zero-vector as direction (as indicated by intesection(plane,plane) documentation)") {
					REQUIRE(glm::all(glm::epsilonEqual(line.getDirection(), zerovec, eps)));
				}
			}
		}
	}

	GIVEN("Line through y-axis and xz-plane") {
		const Line lineThroughY {yaxis, zerovec};
		
		WHEN("Computing the point of intersection") {
			const Point intersectionPoint = intersection(lineThroughY, xzplane);

			THEN("Zero-point is returned") {
				REQUIRE(glm::all(glm::epsilonEqual(intersectionPoint, zerovec, eps)));
			}
		}
	}

	GIVEN("Line through x-axis and xz-plane") {
		const Line lineThroughX {zerovec, xaxis};
		
		WHEN("Computing the point of intersection") {
			const Point intersectionPoint = intersection(lineThroughX, xzplane);

			THEN("NaN-point is returned") {
				REQUIRE(intersectionPoint == glm::vec3(NAN, NAN, NAN));
			}
		}
	}

	GIVEN("A bulk of random (line,plane)-pairs") {

		for(std::size_t i = 0; i < 100; i++) {
			const Line line {glm::sphericalRand(1.0f), glm::ballRand(1.0f)};
			const Plane plane {glm::ballRand(1.0f), glm::sphericalRand(1.0f)};

			THEN("Ordering of pair members as arguments does not matter") {
				const Point linePlaneIntersection = intersection(line, plane);
				const Point planeLineIntersection = intersection(plane, line);
				REQUIRE(linePlaneIntersection == planeLineIntersection);
			}
		}
	}

	GIVEN("A bulk of random (plane,plane)-pairs") {

		for(std::size_t i = 0; i < 100; i++) {
			const Plane plane1 {glm::ballRand(1.0f), glm::sphericalRand(1.0f)};
			const Plane plane2 {glm::ballRand(1.0f), glm::sphericalRand(1.0f)};

			THEN("Ordering of pair members as arguments does not matter") {
				const Line plane1plane2 = intersection(plane1, plane2);
				const Line plane2plane1 = intersection(plane2, plane1);
				REQUIRE(almostEqual<float, 3>(plane1plane2, plane2plane1));
			}
		}
	}

	GIVEN("XY, XZ and YZ planes") {
		THEN("Their common intersection is the zero-point") {
			const Point intersectionPoint = intersection(xyplane, xzplane, yzplane);
			const Point zeropoint = Point(0.0f, 0.0f, 0.0f);
			const bool areEqual = almostEqual<float, 3>(intersectionPoint, zeropoint);
			REQUIRE(areEqual);
		}
	}

	GIVEN("A bulk of random plane-triplets") {

		for(std::size_t i = 0; i < 100; i++) {
			const Plane
				p1 {glm::ballRand(1.0f), glm::sphericalRand(1.0f)},
				p2 {glm::ballRand(1.0f), glm::sphericalRand(1.0f)},
				p3 {glm::ballRand(1.0f), glm::sphericalRand(1.0f)};

			WHEN("Computing common points for each triplet") {

				const Point
					common123 = intersection(p1, p2, p3),
					common132 = intersection(p1, p3, p2),
					common213 = intersection(p2, p1, p3),
					common231 = intersection(p2, p3, p1),
					common312 = intersection(p3, p1, p2),
					common321 = intersection(p3, p2, p1);

				THEN("Ordering of triplet members as arguments does not matter") {
					REQUIRE(common123 == common132);
					REQUIRE(common132 == common213);
					REQUIRE(common213 == common231);
					REQUIRE(common231 == common312);
					REQUIRE(common312 == common321);
					REQUIRE(common321 == common123);
				}
			}
		}
	}

	// TODO: More difficult plane-plane intersections with non-trivial results that cannot be degenerate cases
}