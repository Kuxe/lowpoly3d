#include "bounding_volume_hierarchy.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <functional>
#include <catch2/catch_all.hpp>

#include "generators/terraingenerator.hpp"

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

	GIVEN("A procedurally generated terrain") {
		TerrainGenerator tg;
		Model terrainModel = tg.generate();

		WHEN("Building a BVH over the terrain") {
			BVH<Sphere> terrainBVH(terrainModel);

			THEN("Each parent BV encloses its child BV") {
				auto enclosesChildren = [](const BVH<Sphere>& bvh, const std::size_t& idx) {
					auto const& current = bvh[idx];
					auto const& left = bvh.left(current);
					auto const& right = bvh.right(current);
					REQUIRE(current.encloses(left));
					REQUIRE(current.encloses(right));
				};

				terrainBVH.depthfirstExceptLeaves(enclosesChildren, terrainBVH.root_idx());
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

	GIVEN("A BVH over a single triangle and a rectangle") {
		using vertex_type = Model::vertex_type;
		using color_type = Model::color_type;
		using triangle_indices_type = Model::triangle_indices_type;

		auto rectangleModel = []() {
			std::vector<vertex_type> const vertices {
			{0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}
		};

		std::vector<color_type> const colors {
			{0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}
		};

		std::vector<triangle_indices_type> const indices {
			{0, 1, 3},
			{1, 2, 3}
		};
			
			return Model{vertices, colors, indices};
		}();

		auto triangleModel = []() {
			std::vector<vertex_type> const vertices {
				{0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 0.0f},
			};

			std::vector<color_type> const colors {
				{0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, 0.0f}
			};

			std::vector<triangle_indices_type> const indices {
				{0, 1, 2}
			};
			
			return Model{vertices, colors, indices};
		}();

		BVHModel
			rectangleBVH(&rectangleModel),
			triangleBVH(&triangleModel);
		
		WHEN("Testing intersections with identity transformations") {
			const glm::mat4 identity = glm::mat4(1.0f);
			const bool intersection = collides(rectangleBVH, triangleBVH, identity, identity);
			
			THEN("intersection is detected") {
				REQUIRE(intersection);
			}
		}

		WHEN("Testing intersections with transformations so that they do collide") {
			auto const identity = glm::mat4(1.0f);
			auto const translationTransformation = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
			auto const intersection = collides(rectangleBVH, triangleBVH, identity, identity);
			
			THEN("intersection is detected") {
				REQUIRE(intersection);
			}
		}

		WHEN("Testing intersections with transformations so that they touch") {
			auto const identity = glm::mat4(1.0f);
			auto const translationTransformation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			auto const intersection = collides(rectangleBVH, triangleBVH, identity, translationTransformation);
			
			THEN("intersection is reported") {
				REQUIRE(intersection);
			}
		}
		
		WHEN("Testing intersections with transformations so that they do not collide") {
			auto const identity = glm::mat4(1.0f);
			auto const translationTransformation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
			auto const intersection = collides(rectangleBVH, triangleBVH, identity, translationTransformation);
			
			THEN("no intersection is reported") {
				REQUIRE_FALSE(intersection);
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

		WHEN("testing intersection between intersecting translated bvh and bvh at identity") {
			const glm::mat4 identity = glm::mat4(1.0f);
			const glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			const bool intersecting = collides(bvh, bvh, identity, translation);

			THEN("intersection is detected") {
				REQUIRE(intersecting);
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