#ifndef RENDERDATA_HPP
#define RENDERDATA_HPP

#include <glm/mat4x4.hpp>
#include <string>

namespace lowpoly3d {

//The renderer receives an array of RenderDatas,
//looping through each RenderData and renders the
//model given by modelId and modelMatrix
struct RenderData {
	glm::mat4 modelMatrix;
	std::string model, shader;
	RenderData() {};
	RenderData(const glm::mat4& modelMatrix, const std::string& model, const std::string& shader = "Default");
};

}

#endif //RENDERDATA_HPP