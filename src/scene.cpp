#include "scene.hpp"

namespace lowpoly3d
{

static double PI_HALF = std::acos(-1.0) / 2.0; 

SceneConstants::SceneConstants()
	: SceneConstants(glm::mat4(1.0f), PI_HALF, false) {}

SceneConstants::SceneConstants(glm::mat4 const& iView, double iSunRadians, bool iShowWireframes)
	: mView(iView), mSunRadians(iSunRadians), mShowWireframes(iShowWireframes) {}

glm::mat4 const& SceneConstants::getView() const
{
	return mView;
}

double SceneConstants::getSunRadians() const
{
	return mSunRadians;
}

bool SceneConstants::getShowWireframes() const
{
	return mShowWireframes;
}

Scene::Scene() = default;

Scene::Scene(SceneConstants const& iSceneConstants)
	: mSceneConstants(iSceneConstants) {}

SceneConstants const& Scene::getSceneConstants() const
{
	return mSceneConstants;	
}

Scene::RenderDatas const& Scene::getRenderDatas() const
{
	return mRenderDatas;
}

Scene getDefaultScene()
{
	SceneConstants const defaultSceneConstants;
	Scene const scene { defaultSceneConstants };
	return scene;
}

} // End of namespace lowpoly3d