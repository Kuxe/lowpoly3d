#include "bounding_volume_hierarchy.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <functional>
#include <catch.hpp>

namespace lowpoly3d {

SCENARIO("BVH building") {
	GIVEN("A single triangle model") {

		Model model = getSingleTriangleModel();

		WHEN("Building a BVH over that single triangle") {
			
			BVH<Sphere> bvh(model);

			THEN("The BVH consists of a single bounding volume") {
				REQUIRE(bvh.size() == 1);
			}
			THEN("The root node is a leaf") {
				REQUIRE(bvh.root().is_leaf);
			}
			THEN("The root node is the minimum bounding sphere over the triangle") {
				const auto& root = bvh.root();
				REQUIRE(root.isMBSof({
					model.vertices[0],
					model.vertices[1],
					model.vertices[2]}));
			}
		}
	}

	GIVEN("A two triangle model") {

		Model model = getTwoTriangleModel();

		WHEN("Building a BVH over these two triangles") {
			
			BVH<Sphere> bvh(model);
			
			THEN("The BVH consists of a three bounding volumes") {
				REQUIRE(bvh.size() == 3);
			}
			THEN("The root node is not a leaf") {
				REQUIRE(!bvh.root().is_leaf);
			}
			THEN("The left child of root node is a leaf") {
				REQUIRE(bvh.left(bvh.root()).is_leaf);
			}
			THEN("The right child of root node is a leaf") {
				REQUIRE(bvh.right(bvh.root()).is_leaf);
			}
			THEN("The root index is not equal to any of its child indices") {
				REQUIRE(bvh.root_idx() != bvh.root().left_idx);
				REQUIRE(bvh.root_idx() != bvh.root().right_idx);
			}
			THEN("The BV at the root index is the same BV returned by root()") {
				REQUIRE(&bvh[bvh.root_idx()] == &bvh.root());
			}
		}
	}
}

SCENARIO("BVH inclusiveness") {

	GIVEN("A sphere") {
		SphereGenerator sg({125.0f, 125.0f, 125.0f}, 0);
		auto sphere = sg.generate();
		
		WHEN("a bvh is built over the sphere") {
			auto bvh = BVHModel(&sphere);
			
			THEN( "the bvh root encloses all child bvs" ) {
				bool rootEnclosesAll = true;
				for(const auto& bv : bvh.getBVs()) {
					rootEnclosesAll = rootEnclosesAll && bvh.root().encloses(bv);
				}
				REQUIRE(rootEnclosesAll == true);
			}
			
			THEN("all parents enclose their children") {
				std::function<bool(const BVH<Sphere>& bvh, const BVH<Sphere>::bv_type& parent, bool ok)> exhaustive;
				exhaustive = [&exhaustive](const BVH<Sphere>& bvh, const BVH<Sphere>::bv_type& parent, bool ok) -> bool {
					if(parent.is_leaf) return true;
					return
						parent.encloses(bvh.left(parent)) && parent.encloses(bvh.right(parent)) &&
						exhaustive(bvh, bvh.left(parent), ok) && exhaustive(bvh, bvh.right(parent), ok);
				};
				REQUIRE(exhaustive(bvh, bvh.root(), true) == true);
			}
		}
	}
	// Next up: Check that all leaf nodes actually contain whatever triangle they should contain
}

SCENARIO("BVH collision") {

	GIVEN("A BVH over a single triangle") {
		Model model = getSingleTriangleModel();
		BVHModel bvh(&model);

		WHEN("testing self-intersection with identity transformation") {
			const glm::mat4 identity = glm::mat4(1.0f);
			const bool intersecting = collides(bvh, bvh, identity, identity);
			THEN("intersection is detected") {
				REQUIRE(intersecting);
			}
		}

		WHEN("testing intersection between non-intersecting translated bvh and bvh at identity") {
			const glm::mat4 identity = glm::mat4(1.0f);
			const glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
			const bool intersecting = collides(bvh, bvh, identity, translation);

			THEN("intersection is not detected") {
				REQUIRE_FALSE(intersecting);
			}
		}

	}

	GIVEN("A BVH over a single sphere") {
		Sphere sphere = {glm::vec3(0.0f, 0.0f, 0.0f), 1.0f};
		SphereGenerator generator;
		Model sphereModel = generator.generate(sphere);
		BVHModel bvh(&sphereModel);

		WHEN("Testing self-intersections with identity transformation") {
			const glm::mat4 identity = glm::mat4(1.0f);
			const bool intersection = collides(bvh, bvh, identity, identity);
			
			THEN("interrsection is detected") {
				REQUIRE(intersection);
			}
		}

		WHEN("testing intersection between non-intersecting translated bvh and bvh at identity") {
			const glm::mat4 identity = glm::mat4(1.0f);
			const glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
			const bool intersecting = collides(bvh, bvh, identity, translation);

			THEN("intersection is not detected") {
				REQUIRE_FALSE(intersecting);
			}
		}
	}
}

SCENARIO("Plotting to dotfile") {

	GIVEN("A BVH over a single triangle") {
		BVH<Sphere> bvh(getSingleTriangleModel());

		WHEN("Converting that BVH to a .dot string via dotgraph() method") {
			std::string actual = bvh.dotgraph();
			
			THEN("dotgraph() method outputs the expected .dot string") {
				std::string actual = bvh.dotgraph();
				std::string expected = "graph BVH {\n0\n}\n";
				REQUIRE(actual == expected);
			}
		}
	}

	GIVEN("A BVH over two triangles") {
		BVH<Sphere> bvh(getTwoTriangleModel());

		WHEN("Converting that BVH to a .dot string via dotgraph() method") {
			std::string actual = bvh.dotgraph();
			
			THEN("dotgraph() method outputs the expected .dot string") {
				std::string actual = bvh.dotgraph();
				std::string expected = "graph BVH {\n2\n2 -- 0\n2 -- 1\n}\n";
				REQUIRE(actual == expected);
			}
		}
	}
}

} // End of namespace lowpoly3d