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
	const Point zeropoint = {0.0f, 0.0f, 0.0f};
	const glm::vec3 xaxis = {1.0f, 0.0f, 0.0f};
	const glm::vec3 yaxis = {0.0f, 1.0f, 0.0};
	const glm::vec3 zaxis = {0.0f, 0.0f, 1.0f};
	const glm::vec3 nxaxis = -xaxis;
	const glm::vec3 nyaxis = -yaxis;
	const glm::vec3 nzaxis = -zaxis;
	const Plane xyplane = {zeropoint, {0.0f, 0.0f, 1.0f}};
	const Plane xzplane = {zeropoint, {0.0f, 1.0f, 0.0f}};
	const Plane yzplane = {zeropoint, {1.0f, 0.0f, 0.0f}};

	static constexpr auto eps = std::numeric_limits<float>::epsilon();

	const auto printvec = [](const glm::vec3& vec) {
		std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
	};

	/* Here be lambdas */
	auto const commuter = [](auto f) {
		return [f](auto const& arg1, auto const& arg2) {
			return f(arg2, arg1);
		};
	};

	auto getCommutativeTester = [commuter](auto f) {
		return [commuter, f](auto const& arg1, auto const& arg2, auto const& binary_prediate) {
			REQUIRE(binary_prediate(f(arg1, arg2), commuter(f)(arg1, arg2)));
		};
	};

	auto const commutativeTest = [getCommutativeTester](auto const& f, auto const& arg1, auto const& arg2, auto const& binary_prediate){
		getCommutativeTester(f)(arg1, arg2, binary_prediate);
	};

	auto intersectionCommutativeTest = [getCommutativeTester](auto const& arg1, auto const& arg2) {
		getCommutativeTester([](auto const& a, auto const& b) { return intersection(a, b); })
			(arg1, arg2, [](auto const& a, auto const&b) { return almostEqual<float, 3>(a, b); }
		);
	};

	GIVEN("The XY-plane, XZ-pplane and YZ-plane") {
		WHEN("Computing their common point with cramer-function") {
			THEN("The reported intersection point is the zero-point") {
				REQUIRE(glm::all(glm::epsilonEqual(cramer(xyplane, xzplane, yzplane), zerovec, eps)));
				REQUIRE(glm::all(glm::epsilonEqual(cramer(xyplane, yzplane, xzplane), zerovec, eps)));
				REQUIRE(glm::all(glm::epsilonEqual(cramer(xzplane, xyplane, yzplane), zerovec, eps)));
				REQUIRE(glm::all(glm::epsilonEqual(cramer(xzplane, yzplane, xyplane), zerovec, eps)));
				REQUIRE(glm::all(glm::epsilonEqual(cramer(yzplane, xyplane, xzplane), zerovec, eps)));
				REQUIRE(glm::all(glm::epsilonEqual(cramer(yzplane, xzplane, xyplane), zerovec, eps)));
			}
		}
	}

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
			THEN("The point of the line is reportedly on the line") {
				REQUIRE(line.contains(line.getPoint()));
			}
		}

		WHEN("Reordering arguments to intersection") {
			THEN("The two returned lines are equal (intersection should commute)") {
				intersectionCommutativeTest(xyplane, xzplane);
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
			THEN("The point of the line is reportedly on the line") {
				REQUIRE(line.contains(line.getPoint()));
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
			THEN("The point of the line is reportedly on the line") {
				REQUIRE(line.contains(line.getPoint()));
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
		const Line lineThroughY {zeropoint, yaxis};
		
		WHEN("Computing the point of intersection") {
			const Point intersectionPoint = intersection(lineThroughY, xzplane);

			THEN("Zero-point is returned") {
				REQUIRE(glm::all(glm::epsilonEqual(intersectionPoint, zeropoint, eps)));
			}
		}
	}

	GIVEN("Line through x-axis and xz-plane") {
		const Line lineThroughX {zeropoint, xaxis};
		
		WHEN("Computing the point of intersection") {
			const Point intersectionPoint = intersection(lineThroughX, xzplane);

			THEN("NaN-point is returned") {
				REQUIRE(intersectionPoint == Point(NAN, NAN, NAN));
			}
		}
	}

	GIVEN("The same line along X-axis with different defining points") {
		std::vector<Line> lines {
			Line{zeropoint, xaxis},
			{glm::vec3(0.1, 0.0, 0.0), xaxis},
			{glm::vec3(-0.1, 0.0, 0.0), xaxis},
			{glm::vec3(100000, 0.0, 0.0), xaxis},
			{glm::vec3(-100000, 0.0, 0.0), xaxis},
			{glm::vec3(-1337, 0.0, 0.0), xaxis},
			{glm::vec3(-1337, 0.0, 0.0), xaxis},
		};
		
		WHEN("Checking if the lines are equal") {
			THEN("All linesa are reported as almost equal") {
				REQUIRE(std::adjacent_find(lines.begin(), lines.end(), [](auto& lhs, auto& rhs) {
					return !almostEqual<float, 3>(lhs, rhs);
				}) == lines.end());
			}
		}
	}

	GIVEN("The same line along X-axis and another line pointing in the reverse direction") {
		std::vector<Line> lines {
			{glm::vec3(1.0, 1.0, 1.0), xaxis},
			{glm::vec3(-1.0, -1.0, -1.0), xaxis},
		};
		
		WHEN("Checking if the lines are equal") {
			THEN("All linesa are reported as almost equal") {
				REQUIRE(std::adjacent_find(lines.begin(), lines.end(), [](auto& lhs, auto& rhs) {
					return !almostEqual<float, 3>(lhs, rhs);
				}) == lines.end());
			}
		}
	}

	GIVEN("A bulk of random (line,plane)-pairs") {

		for(std::size_t i = 0; i < 100; i++) {
			const Line line {glm::sphericalRand(1.0f), glm::ballRand(1.0f)};
			const Plane plane {glm::ballRand(1.0f), glm::sphericalRand(1.0f)};

			THEN("Ordering of pair members as arguments does not matter") {
				intersectionCommutativeTest(line, plane);
			}
		}
	}

	auto testSystemOfLinearEquations = [](
		float x1, float y1, float z1, float b1,
		float x2, float y2, float z2, float b2,
		float x3, float y3, float z3, float b3,
		float s1, float s2, float s3
	) {
		const glm::vec3
			v1 {x1, y1, z1},
			v2 {x2, y2, z2},
			v3 {x3, y3, z3};

		glm::mat3x3 A;
		A = glm::row(A, 0, v1);
		A = glm::row(A, 1, v2);
		A = glm::row(A, 2, v3);
		glm::vec3 b = {b1, b2, b3};

		std::ostringstream oss;
		oss << "A system of linear equations";
		oss << "\nA=" << glm::to_string(A) << "\nb=" << glm::to_string(b);
		GIVEN(oss.str()) {
			WHEN("Computing a solution using Cramer's rule") {
				const glm::vec3 solution = cramer(A, b);
				const glm::vec3 expectedSolution = glm::vec3(s1, s2, s3);
				std::ostringstream solutionStr;
				solutionStr << "The solution should be (x,y,z)=" << glm::to_string(expectedSolution);
				THEN(solutionStr.str()) {
					REQUIRE(solution == expectedSolution);
				}
			}
		}
	};

	/* This system means:
		1x + 0y + 0z = 1
		0x + 1y + 0z = 1
		0x + 0y + 1z = 1
	 * which obviously has (x,y,z)=(1,1,1) as the solution */
	testSystemOfLinearEquations(
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		1, 1, 1
	);

	testSystemOfLinearEquations(
		-1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		-1, 1, 1
	);

	testSystemOfLinearEquations(
		1, 0, 0, 1,
		0, -1, 0, 1,
		0, 0, 1, 1,
		1, -1, 1
	);

	testSystemOfLinearEquations(
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, -1, 1,
		1, 1, -1
	);

	testSystemOfLinearEquations(
		1, 0, 0, 1,
		0, 1, 0, 1,
		1, 1, 1, 3,
		1, 1, 1
	);

	GIVEN("A bulk of random (plane,plane)-pairs") {

		for(std::size_t i = 0; i < 100; i++) {
			const Plane plane1 {glm::ballRand(1.0f), glm::sphericalRand(1.0f)};
			const Plane plane2 {glm::ballRand(1.0f), glm::sphericalRand(1.0f)};

			THEN("Intersection function is commutative") {
				const Line plane1plane2 = intersection(plane1, plane2);
				const Line plane2plane1 = intersection(plane2, plane1);
				INFO("plane1plane2=" << plane1plane2);
				INFO("plane2plane1=" << plane2plane1);
				INFO("If this test fails with only point being wrong, then cramer-function is probably broke");
				intersectionCommutativeTest(plane1, plane2);
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

	GIVEN("A bulk of random planes constructed given a point P and a normal") {
		const std::size_t N_SAMPLES = 100;
		for(std::size_t i = 0; i < N_SAMPLES; i++) {
			const Point p = glm::ballRand(1.0f);
			const Plane	plane {p, glm::sphericalRand(1.0f)};
			WHEN("Checking that P is contained within the plane") {
				const bool isContainedWithinPlane = plane.contains(p);
				THEN("P is reportedly contained within the plane") {
					REQUIRE(isContainedWithinPlane);
				}
			}
		}
	}

	GIVEN("A bulk of random plane-triplets constructed with with the same point P but random normals") {
		const std::size_t N_SAMPLES = 100;
		for(std::size_t i = 0; i < N_SAMPLES; i++) {
			const Point p = glm::ballRand(1.0f);
			const Plane
				p1 {p, glm::sphericalRand(1.0f)},
				p2 {p, glm::sphericalRand(1.0f)},
				p3 {p, glm::sphericalRand(1.0f)};
			WHEN("Computing common points for each triplet") {
				const Point reportedPoint = intersection(p1, p2, p3);

				THEN("P is reportedly contained within all planes") {
					REQUIRE(p1.contains(reportedPoint));
					REQUIRE(p2.contains(reportedPoint));
					REQUIRE(p3.contains(reportedPoint));
				}

				THEN("The common point is P") {
					REQUIRE(p == reportedPoint);
				}
			}
		}
	}

	GIVEN("A bulk of random plane-triplets") {

		const std::size_t N_SAMPLES = 100;
		for(std::size_t i = 0; i < N_SAMPLES; i++) {
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