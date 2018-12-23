#include "geometric_primitives/sphere.hpp"
#include <iostream>
#include <catch.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <catch.hpp>

namespace lowpoly3d {

SCENARIO("Lots of sphere tests (todo: TDD:ify or BDD:ify these)") {
	// Check that colliding works as intended
	// Sanity check that two equal spheres collides
	REQUIRE(colliding(
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}));

	// These spheres are almost touching --- not colliding!
	REQUIRE(colliding(
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{2.01f, 0.0f, 0.0f}, 1.0f}));

	// Just far away enough, expect no collision
	REQUIRE(!colliding(
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        glm::mat4(1.0f),
        glm::translate(
            glm::tmat4x4<Sphere::floating_point_type>(1.0f),
            glm::tvec3<Sphere::floating_point_type>(2.1f, 0.0f, 0.0f))));

	// Just close enough, expect collision
	REQUIRE(colliding(
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        glm::mat4(1.0f),
        glm::translate(
            glm::tmat4x4<Sphere::floating_point_type>(1.0f),
            glm::tvec3<Sphere::floating_point_type>(1.9f, 0.0f, 0.0f))));

	// One sphere is contained in the other
	REQUIRE(colliding(
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{0.0f, 0.0f, 0.0f}, 0.5f},
        glm::mat4(1.0f), glm::mat4(1.0f)));

	// One sphere is contained in the other, other way around
	REQUIRE(colliding(
        Sphere{{0.0f, 0.0f, 0.0f}, 0.5f},
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        glm::mat4(1.0f), glm::mat4(1.0f)));

	// One sphere is contained in the other, other way around
	REQUIRE(colliding(
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        glm::mat4(1.0f), glm::mat4(0.5f)));

	// Check that mbs work as intended
	REQUIRE(mbs(
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}) == 
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f});

	REQUIRE(mbs(
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
		Sphere{{0.0f, 0.0f, 0.0f}, 0.0f}) ==
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f});

	REQUIRE(mbs(
		Sphere{{0.0f, 0.0f, 0.0f}, 0.0f},
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}) == 
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f});

	// Fully enclosed at same position
	REQUIRE(mbs(
		Sphere{{0.0f, 0.0f, 0.0f}, 0.5f},
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}) ==
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f});

	// Fully enclose, not at same position
	REQUIRE(mbs(
		Sphere{{0.5f, 0.0f, 0.0f}, 0.5f},
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}) ==
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f});

	// Partial enclose
	REQUIRE(mbs(
		Sphere{{0.5f, 0.0f, 0.0f}, 0.6f},
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}) != 
		Sphere{{0.0f, 0.0f, 0.0f}, 1.0f});


	GIVEN("Two intersecting spheres, one larger than the other") {
		const Sphere s1{{1.0f, 0.0f, 0.0f}, 1.0f};
		const Sphere s2{{3.0f, 0.0f, 0.0f}, 2.0f};
		WHEN("Computing the minimum bounding sphere over both intersecting spheres") {
			const Sphere minimumBoundingSphere = mbs(s1, s2);
			THEN("The computed minimum bounding sphere contains both spheres") {
				REQUIRE(minimumBoundingSphere.encloses(s1));
				REQUIRE(minimumBoundingSphere.encloses(s2));
			}

			THEN("The reported minimum bounding sphere is the minimum bounding sphere") {
				REQUIRE(minimumBoundingSphere == Sphere{{2.5f, 0.0f, 0.0f}, 2.5f});
			}
		}
	}

	// Touching
	REQUIRE(mbs(
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{3.0f, 0.0f, 0.0f}, 2.0f}) ==
        Sphere{{2.0f, 0.0f, 0.0f}, 3.0f});

	// Touching
	REQUIRE(mbs(
        Sphere{{3.0f, 0.0f, 0.0f}, 2.0f},
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}) ==
        Sphere{{2.0f, 0.0f, 0.0f}, 3.0f});

	// Disjoint
	REQUIRE(mbs(
        Sphere{{4.0f, 0.0f, 0.0f}, 1.0f},
        Sphere{{0.0f, 0.0f, 0.0f}, 1.0f}) ==
        Sphere{{2.0f, 0.0f, 0.0f}, 3.0f});

	GIVEN("A triangle") {
		using Triangle = std::array<glm::vec3, 3>;
		const Triangle triangle = { glm::vec3
			{0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0}};

		WHEN("Computing the minimum bounding sphere over that triangle") {
			const Sphere triangleMbs = mbs<float, 3>(triangle);

			THEN("The reported minimum bounding sphere is minimal") {
				// ||p_i - c|| = r, i=1,2,3 is satisfied iff a ball of radius r positioned at c is
				// a mbs over a triangle
				for(const auto& p : triangle) {
					REQUIRE(glm::distance(p, triangleMbs.p) == triangleMbs.r);
				}
			}
		}
	}


	}
} // End of namespace lowpoly3d