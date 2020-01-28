#ifndef SCENE_HPP
#define SCENE_HPP

#include "renderdata.hpp"
#include <glm/mat4x4.hpp>
#include <vector>

namespace lowpoly3d {

/** SceneConstants hold constants that must be defined for each scene */
struct SceneConstants
{
public:
	SceneConstants();
	SceneConstants(glm::mat4 const& iView, double iSunRadians, bool iShowWireframes);

	glm::mat4 const& getView() const;
	double getSunRadians() const;
	bool getShowWireframes() const;

private:
	glm::mat4 mView;
	double mSunRadians;
	bool mShowWireframes;
};

/** The scene struct contains all data which is needed for a frame to rendered and displayed
	(previously provided by renderquerier interface) **/
class Scene {
public:

	using RenderDatas = std::vector<RenderData>;

	Scene();
	Scene(SceneConstants const& iSceneConstants);

	template<typename TIterator>
	void insert(TIterator iBegin, TIterator iEnd)
	{
		std::size_t const size = iEnd - iBegin;
		mRenderDatas.reserve(mRenderDatas.size() + size);
		mRenderDatas.insert(mRenderDatas.end(), iBegin, iEnd);
	}

	RenderDatas const& getRenderDatas() const;
	SceneConstants const& getSceneConstants() const;

private:
	RenderDatas mRenderDatas;
	SceneConstants mSceneConstants;
};

// Returns a scene with a default set of scene constants
Scene getDefaultScene();

}

#endif //SCENE_HPP