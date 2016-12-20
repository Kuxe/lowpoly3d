#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <fstream>
#include "renderdata.hpp"
#include "modeldefs.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "celestialbody.hpp"
#include <glm/glm.hpp>

/** Gamedata contains whatever (non-pointer) data is required to get the simulation going,
	such as:
		* Color of sun
		* Normal of water plane
		* Any number of renderdatas

	Gamedata is parsed into variables and then fed to the simulation ctor.
	Any member of Gamdata must have a default constructor. Gamedata needs to be instanciated before
	cereal fills it with data!
**/
struct Gamedata {

	/** Gametime speed factor **/
	float gametimeSpeedFactor = 1.0f;

	/** Parameters used by generators **/
	Color sunColor = {255, 0, 255};
	uint8_t sunSubdivides = 0;
	CelestialBody suncb = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 3.14f * 0.5f};
	CelestialBody mooncb = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 3.14f * 1.5f};
	bool forceDaytime;
	glm::vec3 planeNormal;
	Color planeColor = {255, 0, 255};

	/** "Constant" renderdatas and "linear" renderdatas wrp programming complexity :) **/
	RenderData sunRd;
	std::vector<RenderData> renderDatas;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			CEREAL_NVP(gametimeSpeedFactor),
			CEREAL_NVP(sunColor),
			CEREAL_NVP(sunSubdivides),
			CEREAL_NVP(suncb),
			CEREAL_NVP(mooncb),
			CEREAL_NVP(forceDaytime),
			CEREAL_NVP(planeNormal),
			CEREAL_NVP(planeColor),
			CEREAL_NVP(sunRd),
			CEREAL_NVP(renderDatas)
		); 
	}
};

#endif //GAMEDATA_HPP