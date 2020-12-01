#include "draw_geometric_primitives.hpp"
#include "renderdata.hpp"
#include "scene.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> //glm::translate

namespace lowpoly3d {

void draw(Scene& iScene, Point const& iPoint)
{
	RenderDataBuilder rdb;
	rdb.setTransformationInWorld(glm::translate(iPoint));
	// Assume that there is a model loaded in engine called "sphere"
	rdb.setModel("sphere");
	// Assume that there is a shader loaded in engine called "default"
	rdb.setShader("default");

	iScene.insert(rdb.build());
}

} // End of namespace lowpoly3d