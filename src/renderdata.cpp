#include "renderdata.hpp"
#include "drawfeature.hpp"

namespace lowpoly3d {

RenderData::RenderData() = default;
RenderData::~RenderData() = default;

RenderData::RenderData(const glm::mat4& modelMatrix, const std::string& model, const std::string& shader) :
	modelMatrix(modelMatrix), model(model), shader(shader) { }


RenderDataBuilder::RenderDataBuilder() = default;

RenderData RenderDataBuilder::build() const
{
	return mRenderData;
}

RenderDataBuilder& RenderDataBuilder::setTransformationInWorld(glm::mat4 const& iTransformationInWorld)
{
	mRenderData.modelMatrix = iTransformationInWorld;
	return *this;
}

RenderDataBuilder& RenderDataBuilder::setModel(std::string const& iModel)
{
	mRenderData.model = iModel;
	return *this;
}

RenderDataBuilder& RenderDataBuilder::setShader(std::string const& iShader)
{
	mRenderData.shader = iShader;
	return *this;
}

RenderDataBuilder& RenderDataBuilder::setDrawFeatureTarget(DrawFeatureTarget&& iDrawFeatureTarget)
{
	mRenderData.drawFeatureTarget = iDrawFeatureTarget;
	return *this;
}

} // End of namespace lowpoly3d
