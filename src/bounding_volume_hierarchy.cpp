#include "bounding_volume_hierarchy.hpp"

namespace lowpoly3d {

// Returns two subsets of "indices" into trinagles that represent a binary partition of triangles
std::pair<std::vector<std::size_t>, std::vector<std::size_t>> split(const Model& model, const std::vector<std::size_t>& indices) {
	using Partition = std::pair<std::vector<std::size_t>, std::vector<std::size_t>>;
	if(indices.size() > 2) {

		glm::vec3 mean(0.0f); // Mean over all triangle midpoints
		for(std::size_t i : indices) {
			mean += model.triangle_midpoint(i); // Add all vertices of i:th triangle to mean 
		}

		mean = (1.0f/float(indices.size())) * mean;

		/** Ok, now mean has been taken over all triangle midpoints. Time to find a normal to the splitting line!
			
			1. 	Pick two arbitrary points. Main idea: If there are two equally sized clusters A, B of points,
				then a one point from A and one point from B is picked with 50%. Hence, the difference
				between two picked points is a good normal to the splitting plane with 50% probability.

			2.	Pick a point. If it lies above the plane, move the plane normal in the direction of the new point.
				This will help the point align to one of the clusters. It is essential to only align the plane
				if a point is above it --- otherwise we're at risk of just tilting the plane back and forth
				if we chose a bad initial plane.

			3. Repate 2 until there are no more points **/

		glm::vec3 normal = model.triangle_midpoint(indices[0]) - model.triangle_midpoint(indices[1]);
		for(std::size_t indices_idx = 2; indices_idx < indices.size(); indices_idx++) {
			const glm::vec3 diff = model.triangle_midpoint(indices[indices_idx]) - mean;
			if(glm::dot(diff, normal) > 0) {
				normal += diff;
			}
		}

		// The normal to the splitting line is found, so time to partition!
		std::pair<std::vector<std::size_t>, std::vector<std::size_t>> binary_partition;
		for(std::size_t i : indices) {
			if(glm::dot(model.triangle_midpoint(i) - mean, normal) > 0) {
				binary_partition.first.push_back(i);
			} else {
				binary_partition.second.push_back(i);
			}
		}
		return binary_partition;
	} else if(indices.size() == 2) {
		return Partition(std::vector<size_t>{indices[0]}, std::vector<size_t>{indices[1]});
	} else if(indices.size() == 1) {
		return Partition(std::vector<size_t>{indices[0]}, std::vector<size_t>{});
	} else {
		return Partition();
	}
}

} // End of namespace lowpoly3d