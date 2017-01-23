#include "renderdata.hpp"

namespace lowpoly3d {

RenderData::RenderData(const glm::mat4& modelMatrix, const std::string& model, const std::string& shader) :
	modelMatrix(modelMatrix), model(model), shader(shader) { }

}
