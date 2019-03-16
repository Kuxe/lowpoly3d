#include <catch.hpp>

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