#ifndef CELESTIALBODY_HPP
#define CELESTIALBODY_HPP

#include <math.h>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>

namespace lowpoly3d {

/** A class for spheres moving in periodic orbits, such as the sun or the moon **/
class CelestialBody {
	glm::vec3 center, axis;
	float omega0;
public:
	CelestialBody(const glm::vec3& center, const glm::vec3& axis, float omega0) :
		center(center), axis(axis), omega0(omega0) { }

	glm::vec3 getPos(float t) const {
		float magnitude = glm::length(axis);
		glm::vec3 basey = axis/magnitude;
		glm::vec3 basex = glm::cross({basey.z, basey.x, basey.y}, basey);
		glm::vec3 basez = glm::cross(basey, basex);
		//Rotate a vector around y-axis in some basis, then convert rotated vector from that basis to standard basis
		return magnitude * glm::transpose(glm::mat3{basex, basey, basez}) * (glm::vec3{sinf(t + omega0), 0.0, cosf(t + omega0)}) + center;
	}
};

}

#endif //CELESTIALBODY_HPP 