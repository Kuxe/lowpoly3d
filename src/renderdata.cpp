#include "renderdata.hpp"

RenderData::RenderData(const glm::mat4& modelMatrix, const int vertexArray) :
	modelMatrix(modelMatrix), vertexArray(vertexArray) { }