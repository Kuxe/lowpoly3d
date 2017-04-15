#ifndef SCENE_HPP
#define SCENE_HPP

#include "renderdata.hpp"
#include <glm/mat4x4.hpp>

namespace lowpoly3d {

/** The scene struct contains all data which is needed for a frame to rendered and displayed
	(previously provided by renderquerier interface) **/

struct Scene {
	std::vector<RenderData> renderDatas;
	glm::mat4 view;
	float gametime;
	float sunRadians;
};

}

#endif //SCENE_HPP