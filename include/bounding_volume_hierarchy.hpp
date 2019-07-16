#ifndef BOUNDING_VOLUME_HIERARCHY
#define BOUNDING_VOLUME_HIERARCHY

#include <algorithm> //std::all
#include <iostream> //std::cout
#include <utility> //std::pair
#include <numeric> //std::iota
#include <functional> //std::function
#include <sstream> //std::stringstream

#include "geometric_primitives/intersects.hpp"
#include "geometric_primitives/sphere.hpp"
#include "geometric_primitives/triangle.hpp"

#include "generators/spheregenerator.hpp"

#include "model.hpp"

#include "utils/no_such_triangle_exception.hpp"
#include "utils/not_implemented_exception.hpp"
#include "utils/throw_if.hpp"

namespace lowpoly3d {

// Returns two subsets of "indices" into trinagles that represent a binary partition of triangles
std::pair<std::vector<std::size_t>, std::vector<std::size_t>> split(const Model& model, const std::vector<std::size_t>& indices);

// A binary bounding volume that holds indices to its children
// If it is a leaf, then both left_idx and right_idx holds the index to its triangle
template<typename BV>
struct BinaryBV : public BV {
	std::size_t left_idx, right_idx;
	bool is_leaf;
	BinaryBV(const BV& bv, std::size_t left_idx, std::size_t right_idx, bool is_leaf) :
		BV(bv), left_idx(left_idx), right_idx(right_idx), is_leaf(is_leaf) { }
};

/* A Bounding Volume Hierarchy (BVH). Bounding volumes are assumed to live in modelspace.
 * A BVH is built on construction of a BVH instance over the provided "model" parameter **/
template<typename TValue>
class BVH {
public:
	using value_type = TValue;
	using bv_type = BinaryBV<value_type>;
	using floating_point_type = typename TValue::floating_point_type;
private:
	std::vector<bv_type> bvs;
	std::size_t depth = 0;

	// Returns an index into bvs of the most recently added BV
	const std::size_t build(const Model& model, const std::vector<std::size_t>& indices, std::size_t depth) {
		if(indices.size() == 1) {
			// Extract triangle at the sole index
			const auto& triangle = model.getTriangleIndices(indices.front());
			bvs.emplace_back(mbs<typename bv_type::floating_point_type, bv_type::dimension>(
				model.vertices[triangle[0]],
				model.vertices[triangle[1]],
				model.vertices[triangle[2]]), indices[0], indices[0], true);
		} else {
			// Split and recurse
			const auto pair = split(model, indices);
			std::size_t first_bv_index = 0, second_bv_index = 0;
			first_bv_index = pair.first.size() != 0 ? build(model, pair.first, depth+1) : 0,
			second_bv_index = pair.second.size() != 0 ? build(model, pair.second, depth+1) : 0;
			const Sphere& bvl = bvs[first_bv_index];
			const Sphere& bvr = bvs[second_bv_index];
			const Sphere bv = mbs(bvl, bvr);
			bvs.emplace_back(bv, first_bv_index, second_bv_index, false);
		}
		this->depth = std::max(this->depth, depth);
		return size()-1;
	}

public:
	BVH(const Model& model) {
		std::vector<std::size_t> indices(model.getNumTriangles());
		std::iota(indices.begin(), indices.end(), 0);
		if(model.getNumTriangles() > 0 && model.getNumVertices() > 2) {
			build(model, indices, 1);
		}
	}

	BVH(const BVH& bvh) : bvs(bvh.bvs), depth(bvh.depth) { }
	BVH(BVH&& bvh) : bvs(std::move(bvh.bvs)), depth(std::move(bvh.depth)) { }

	virtual ~BVH() { }

	BVH& swap(BVH bvh) {
		std::swap(bvs, bvh.bvs);
		std::swap(depth, bvh.depth);
		return *this;
	}

	BVH& operator=(BVH bvh) {
		return swap(bvh);
	}

	BVH& operator=(BVH&& bvh) {
		bvs = std::move(bvh.bvs);
		depth = std::move(bvh.depth);
		return *this;
	}

	std::size_t root_idx() const {
		assert(!bvs.empty());
		return size()-1;
	}

	bv_type& root() {
		return bvs[root_idx()];
	}

	const bv_type& root() const {
		return bvs[root_idx()];
	}

	bv_type& left(const bv_type& parent) {
		assert(!parent.is_leaf);
		return bvs[parent.left_idx];
	}

	bv_type& right(const bv_type& parent) {
		assert(!parent.is_leaf);
		return bvs[parent.right_idx];
	}

	const bv_type& left(const bv_type& parent) const {
		assert(!parent.is_leaf);
		return bvs[parent.left_idx];
	}

	const bv_type& right(const bv_type& parent) const {
		assert(!parent.is_leaf);
		return bvs[parent.right_idx];
	}

	// Returns how well balanced the BVH is, i.e. 1.0f if it is perfectly balanced or 0.0f if close to a linked list
	/*
		0
		|\
		0 0
		  |\
		  0 0
		    |\
		    0 0
		Figure 1: A tree close to a linked list */
	float balance() const {
		if(bvs.empty()) return 0.0f;
		if(size() == 1) return 1.0f;
		const std::size_t worst_depth = (size()+1)/2;
		const std::size_t best_depth = std::log2(size())+1;
		const float numerator = float(depth - best_depth);
		const float denominator = float(worst_depth - best_depth);
		assert(denominator != 0.0f);
		return 1.0f - numerator / denominator;
	}

	bv_type& operator[](std::size_t idx) {
		assert(idx < size());
		return bvs[idx];
		}
	const bv_type& operator[](std::size_t idx) const {
		assert(idx < size());
		return bvs[idx];
		}

	const std::vector<bv_type>& getBVs() const {
		return bvs;
	}

	std::size_t size() const {
		return bvs.size();
		}

	// Assumes that BVs are spheres and creates a single model that represents this BVH geometry
	Model triangulate() const {
		Model ret;
		SphereGenerator sg({255, 0, 255}, 0);
		for(const Sphere& sphere : bvs) {
			ret.append(sg.generate(sphere));
		}
		return ret;
	}

	// Applies the unaryNodeFunction to each node of the BVH rooted at idx in preorder
	void depthfirst(
		const std::function<void(const BVH<value_type>&, const std::size_t&)>& unaryNodeFunction,
		const std::size_t& idx) const {

		const bv_type& current = bvs[idx];
		unaryNodeFunction(*this, idx);
		if(current.is_leaf) return;
		depthfirst(unaryNodeFunction, current.left_idx);
		depthfirst(unaryNodeFunction, current.right_idx);
	}

	void depthfirstExceptLeaves(
		const std::function<void(const BVH<value_type>&, const std::size_t&)>& unaryNodeFunction,
		const std::size_t& idx) const {
		
		const bv_type& current = bvs[idx];
		if(current.is_leaf) return;
		unaryNodeFunction(*this, idx);
		depthfirstExceptLeaves(unaryNodeFunction, current.left_idx);
		depthfirstExceptLeaves(unaryNodeFunction, current.right_idx);
	}

	/* Returns true if predicate is true for all BVs */
	bool all_of(std::function<bool(bv_type const&)> const& predicate) {
		return std::all_of(bvs.begin(), bvs.end(), predicate);
	}

	/* Returns true if predicate is true for any BV */
	bool any_of(std::function<bool(bv_type const&)> const& predicate) {
		return std::any_of(bvs.begin(), bvs.end(), predicate);
	}

	/* Returns true if predicate is true for no BV */
	bool none_of(std::function<bool(bv_type const&)> const& predicate) {
		return std::none_of(bvs.begin(), bvs.end(), predicate);
	}

	std::string dotgraph() const {
		std::stringstream dotsource;
		dotsource << "graph BVH {\n";

		if(size() > 0) {
			// Output root by itself
			dotsource << root_idx() << "\n";

			const auto edge2dotsource = [&dotsource](const BVH<value_type>& bvh, const std::size_t& idx) {
				dotsource << idx << " -- " << bvh[idx].left_idx << "\n";
				dotsource << idx << " -- " << bvh[idx].right_idx << "\n";
			};
			
			depthfirstExceptLeaves(edge2dotsource, root_idx());
		}
		dotsource << "}\n";
		return dotsource.str();
	}
};

/** The Model class equipped with a BVH **/
class BVHModel : public BVH<Sphere> {
	const Model* model;
public:
	BVHModel(const Model* model) : BVH(*model), model(model) { }

	TTriangle<floating_point_type, 3> getTriangle(std::size_t idx) const {
		throw_no_such_triangle_if_geq(idx, model->triangleIndices.size());
		auto const& vertices = model->vertices;
		auto const& triangle = model->triangleIndices[idx];
		return TTriangle<floating_point_type, 3>(
			vertices[triangle[0]],
			vertices[triangle[1]],
			vertices[triangle[2]]
		);
	}
};

// Traverses a BVTT by always splitting the largest BV node. Also traverses the child BVTT node with the smallest signed distance first
bool collides_recursive(const BVHModel& a, const BVHModel& b, const glm::mat4& a_world, const glm::mat4& b_world, std::size_t a_idx, std::size_t b_idx) {

	// If the two current bounding volumes are not colliding, then this execution path cannot collide
	const auto bva = a[a_idx];
	const auto bvb = b[b_idx];
	// Radius on root for terrain is ~4.66 seems to small.
	// Don't confuse this issue with earlier issue of small radius
	// this issue has unknown cause!
	if(!colliding(bva, bvb, a_world, b_world)) {
		return false;
	}

	// Now we know that they are colliding --- so check if both are intermediate nodes
	else if(!a[a_idx].is_leaf && !b[b_idx].is_leaf) {
		// This is where I determine what BVTT to traverse
		// I can chose between AX => BX + CX or AX => AY + AZ
		// So check if size(A) > size(X), if so, chose AX => BX + CX.
		// Otherwise, chose AX => AY + AZ
		if(a[a_idx].size() > b[b_idx].size()) {
			// AX => BX + CX is chosen!
			// Now, we're looking for a collision, so assume that the likelihood for collision is higher the more two BVs penetrate...
			// so descend upon BV-pair with smallest signed distance first (i.e. pair with most penetration depth)
			if(signed_distance(a[a[a_idx].left_idx], b[b_idx], a_world, b_world) < signed_distance(a[a[a_idx].right_idx], b[b_idx], a_world, b_world)) {
				// ||BX|| < ||CX|| so descend into BX before CX
				return collides_recursive(a, b, a_world, b_world, a[a_idx].left_idx, b_idx) || collides_recursive(a, b, a_world, b_world, a[a_idx].right_idx, b_idx);
			} else {
				// ||BX|| >= ||CX|| so descend into CX before BX
				return collides_recursive(a, b, a_world, b_world, a[a_idx].right_idx, b_idx) || collides_recursive(a, b, a_world, b_world, a[a_idx].left_idx, b_idx);
			}
		} else {
			// AX => AY + AZ is chosen!
			// Now, we're looking for a collision, so assume that the likelihood for collision is higher the more two BVs penetrate...
			// so descend upon BV-pair with smallest signed distance first (i.e. pair with most penetration depth)
			if(signed_distance(a[a_idx], b[b[b_idx].left_idx]) < signed_distance(a[a_idx], b[b[b_idx].right_idx])) {
				// ||AY|| < ||AZ|| so descend into AY before AZ
				return collides_recursive(a, b, a_world, b_world, a_idx, b[b_idx].left_idx) || collides_recursive(a, b, a_world, b_world, a_idx, b[b_idx].right_idx);
			} else {
				// ||AY|| => ||AZ|| so descend into AZ before AY
				return collides_recursive(a, b, a_world, b_world, a_idx, b[b_idx].right_idx) || collides_recursive(a, b, a_world, b_world, a_idx, b[b_idx].left_idx);
			}
		}
	}

	// If a_idx is not leaf and b_idx is leaf, then we must decend into children of a
	else if(!a[a_idx].is_leaf && b[b_idx].is_leaf) {
		return collides_recursive(a, b, a_world, b_world, a[a_idx].left_idx, b_idx) || collides_recursive(a, b, a_world, b_world, a[a_idx].right_idx, b_idx);
	}

	// If a_idx is leaf and b_idx is not leaf, then we must decend into children of b
	else if(a[a_idx].is_leaf && !b[b_idx].is_leaf) {
		return collides_recursive(a, b, a_world, b_world, a_idx, b[b_idx].left_idx) || collides_recursive(a, b, a_world, b_world, a_idx, b[b_idx].right_idx);
	}

	// Both are triangles so test triangles
	else {
		auto const& a_leaf = a[a_idx];
		auto const& b_leaf = b[b_idx];
		assert(a_leaf.is_leaf);
		assert(b_leaf.is_leaf);

		auto const a_triangle_idx = a_leaf.left_idx;
		auto const b_triangle_idx = b_leaf.left_idx;
		// It should not matter if we take left or right idx on BV-leaves,
		// since either idx on BV-leaves should point to the same triangle
		// by construction
		assert(a_triangle_idx == a_leaf.right_idx);
		assert(b_triangle_idx == b_leaf.right_idx);

		auto const a_triangle_in_model = a.getTriangle(a_triangle_idx);
		auto const b_triangle_in_model = b.getTriangle(b_triangle_idx);
		auto const a_triangle_in_world = a_triangle_in_model.transform(a_world);
		auto const b_triangle_in_world = b_triangle_in_model.transform(b_world);
		return intersects(a_triangle_in_world, b_triangle_in_world);
	}
}

// Take both BVHs into world-space 
bool collides(const BVHModel& a, const BVHModel& b, const glm::mat4& a_world, const glm::mat4& b_world) {
	return collides_recursive(a, b, a_world, b_world, a.size()-1, b.size()-1);
}

struct CollisionData {
	const BVHModel* model;
	const glm::mat4* matrix;
	CollisionData(const BVHModel* model, const glm::mat4* matrix) : model(model), matrix(matrix) { }
};

bool collides(const CollisionData& a, const CollisionData& b) {
	return collides(*a.model, *b.model, *a.matrix, *b.matrix);
}

} //end of namespace lowpoly3d

#endif //BOUNDING_VOLUME_HIERARCHY