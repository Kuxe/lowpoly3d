#include <catch2/catch_all.hpp>

#include "utils/lerp.hpp"

using namespace lowpoly3d;

SCENARIO("Linearly Interpolating") {

    GIVEN("Three positive reals") {
        WHEN("Interpolating with minimum or maximum interpolation parameter") {
            REQUIRE(lerp(0.0, 1.0, 0.0) == 0.0);
            REQUIRE(lerp(0.0, 1.0, 1.0) == 1.0);

            REQUIRE(lerp(0.0, 0.0, 0.0) == 0.0);
            REQUIRE(lerp(0.0, 0.0, 1.0) == 0.0);

            REQUIRE(lerp(1.0, 1.0, 1.0) == 1.0);
            REQUIRE(lerp(1.0, 1.0, 0.0) == 1.0);
        }

        WHEN("Interpolating with t=0.5") {
            THEN("Lerp outputs the average") {
                REQUIRE(lerp(0.0, 1.0, 0.5) == 0.5);
                REQUIRE(lerp(0.0, 2.0, 0.5) == 1.0);
                REQUIRE(lerp(0.0, 4.0, 0.5) == 2.0);
                REQUIRE(lerp(2.3, 57.89, 0.5) == (57.89 + 2.3) / 2.0);
            }
        }
		WHEN("Interpolating with invalid t") {
			THEN("An exception is thrown") {
				REQUIRE_THROWS(lerp(0.0, 0.0, -0.000009));
				REQUIRE_THROWS(lerp(0.0, 0.0, 1.0000001));
			}
		}
		WHEN("Interpolating with valid t") {
			THEN("An no exception is thrown") {
				REQUIRE_NOTHROW(lerp(0.0, 0.0, -0.000000));
				REQUIRE_NOTHROW(lerp(0.0, 0.0, 1.000000));
			}
		}
    }
}