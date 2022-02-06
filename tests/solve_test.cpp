#include <catch2/catch_all.hpp>

#include "utils/solve.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> //glm::vec3 into std::cout

namespace Catch {
    template<>
    struct StringMaker<glm::vec2> {
        static std::string convert(glm::vec2 const& value ) {
            return glm::to_string(value);
        }
    };
}

TEST_CASE("solveDotSystem2D") {
	using namespace lowpoly3d;
	auto const nanf = std::numeric_limits<float>::quiet_NaN();
	auto const nanpoint2f = Point2f(nanf, nanf);

	REQUIRE(solveDotSystem2D<float>(Point2f{1.0f, 3.0f}, Point2f{5.0f, 6.0f}, 14.0f, 34.0f) == Point2f{2.0f, 4.0f});
	REQUIRE(glm::all(glm::isnan(solveDotSystem2D<float>(Point2f{1.0f, 0.0f}, Point2f{1.0f, 0.0f}, 0.0f, 0.0f))));
}

SCENARIO("solveScalingVecs") {
	using namespace lowpoly3d;

	GIVEN("Two linearly independent base vectors e1, e2 and a point p") {
		auto const e1 = Point2f {2.0f, 1.0f};
		auto const e2 = Point2f {1.0f, -1.0f};
		auto const point = Point2f {0.0f, 3.0f};

		WHEN("Computing two coefficients r, s of the base vectors s.t e1*r + e2*s = p") {
			auto const coeffs = solveScalingVecs(e1, e2, point);

			THEN("The reported coefficients satisfy e1*r + e2*s = p") {
				REQUIRE(coeffs.x*e1 + coeffs.y*e2 == point);
			}
			THEN("The reported coefficients are minimal") {
				REQUIRE(coeffs == Point2f{1.0f, -2.0f});
			}
		}
	}

	GIVEN("Two linearly dependent base vectors e1, e2 and a collinear point p") {
		auto const e1 = Point2f {1.0f, 0.0f};
		auto const e2 = Point2f {0.5f, 0.0f};
		auto const point = Point2f {1.0f, 0.0f};

		WHEN("Computing two coefficients r, s of the base vectors s.t e1*r + e2*s = p") {
			auto const coeffs = solveScalingVecs(e1, e2, point);

			THEN("The reported coefficients satisfy e1*r + e2*s = p") {
				REQUIRE(coeffs.x*e1 + coeffs.y*e2 == point);
			}
		}
	}

	GIVEN("Two linearly dependent base vectors e1, e2 and a non-collinear point p") {
		auto const e1 = Point2f {1.0f, 0.0f};
		auto const e2 = Point2f {0.5f, 0.0f};
		auto const point = Point2f {1.0f, 1.0f};

		WHEN("Computing two coefficients r, s of the base vectors s.t that any answer is nonsense because the point p is not spanned by e1, e2") {
			auto const coeffs = solveScalingVecs(e1, e2, point);

			THEN("The reported coefficients are NaNs") {
				REQUIRE(glm::all(glm::isnan(coeffs)));
			}
		}
	}

	GIVEN("Two equal base vectors e1, e2 and an equal point p") {
		auto const e1 = Point2f {0.0f, 1.0f};
		auto const e2 = Point2f {0.0f, 1.0f};
		auto const point = Point2f {0.0f, 1.0f};
		
		WHEN("Computing the two coefficients of the two equal bases that are equal to the given point") {
			auto const coeffs = solveScalingVecs(e1, e2, point);

			THEN("The reported coefficients are (1, 0)") {
				REQUIRE(coeffs[0] == 1.0f);
				REQUIRE(coeffs[1] == 0.0f);
			}

			THEN("The reported coefficients satisfy e1*r + e2*s = p") {
				INFO("coeffs[0]=" << coeffs[0] << ", coeffs[1]=" << coeffs[1]);
				REQUIRE(coeffs.x*e1 + coeffs.y*e2 == point);
			}
		}
	}

	GIVEN("(1,0) and the zero vector as two base vectors and a point on the x-axis, which is spanned by (1,0)") {
		auto const zerovec = Point2 {0.0f, 0.0f};
		auto const xvec = Point2 {1.0f, 0.0f};
		auto const point = Point2 {2.0f, 0.0f};

		WHEN("Computing the two coefficients of (1,0) and (0,0)") {
			auto const coeffs = solveScalingVecs(xvec, zerovec, point);

			THEN("The reported coefficients are (2, 0)") {
				CHECK(coeffs[0] == 2.0f);
				CHECK(coeffs[1] == 0.0f);
			}
		}
	}

	GIVEN("The zero vector and (1,0) as two base vectors and a point on the x-axis, which is spanned by (1,0)") {
		auto const zerovec = Point2 {0.0f, 0.0f};
		auto const xvec = Point2 {1.0f, 0.0f};
		auto const point = Point2 {2.0f, 0.0f};

		WHEN("Computing the two coefficients of (1,0) and (0,0)") {
			auto const coeffs = solveScalingVecs(zerovec, xvec, point);

			THEN("The reported coefficients are (0, 2)") {
				CHECK(coeffs[0] == 0.0f);
				CHECK(coeffs[1] == 2.0f);
			}
		}
	}

	GIVEN("Two base vectors (0,-1), (0, 0) and a point (0,0)") {
		auto const zerovec = Point2 {0.0f, 0.0f};
		auto const nyvec = Point2 {0.0f, -1.0f};
		auto const point = Point2 {0.0f, 0.0f};

		WHEN("Computing the two coefficients of (0,-1) and (0,0)") {
			auto const coeffs = solveScalingVecs(nyvec, zerovec, point);

			THEN("The reported coefficients are (0, 0)") {
				auto const eps = std::numeric_limits<float>::epsilon();
				INFO("(0,0) and (0,-1) are reported as collinear: " << (glm::areCollinear(nyvec, zerovec, eps) ? "Yes." : "No."));
				INFO("glm::dot(point, point)=" << glm::dot(point, point));
				INFO("glm::dot(nyvec, point)=" << glm::dot(nyvec, point));
				INFO("glm::dot(nyvec, point)/glm::dot(nyvec, nyvec)=" << glm::dot(nyvec, point)/glm::dot(nyvec, nyvec));
				INFO("glm::dot(nyvec, point)/glm::dot(nyvec, nyvec) * nyvec = " << glm::to_string(glm::dot(nyvec, point)/glm::dot(nyvec, nyvec) * nyvec));
				INFO("glm::dot(nyvec, point)/glm::dot(nyvec, nyvec) * nyvec == point? " << (glm::all(glm::epsilonEqual(glm::dot(nyvec, point)/glm::dot(nyvec, nyvec) * nyvec, point, eps)) ? "Yes. (as expected)" : "No. (this was unexpected)"));
				CHECK(coeffs[0] == 0.0f);
				CHECK(coeffs[1] == 0.0f);
			}
		}
	}
}