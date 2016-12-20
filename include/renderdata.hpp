#ifndef RENDERDATA_HPP
#define RENDERDATA_HPP

#include <glm/glm.hpp>
#include "cereal/archives/json.hpp"

/** Make glm::vec3 serializable by cereal **/
namespace glm {
	template<typename Archive> void serialize(Archive& archive, glm::vec2& v) {
	    archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y));
	}
	template<typename Archive> void serialize(Archive& archive, glm::vec3& v) {
	    archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y),    cereal::make_nvp("z", v.z));
	}
	template<typename Archive> void serialize(Archive& archive, glm::tvec3<uint8_t>& v) {
	    archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y),    cereal::make_nvp("z", v.z));
	}
	template<typename Archive> void serialize(Archive& archive, glm::mat3& m) {
	archive(
		CEREAL_NVP(m[0][0]), CEREAL_NVP(m[0][1]), CEREAL_NVP(m[0][2]),
		CEREAL_NVP(m[1][0]), CEREAL_NVP(m[1][1]), CEREAL_NVP(m[1][2]),
		CEREAL_NVP(m[2][0]), CEREAL_NVP(m[2][1]), CEREAL_NVP(m[2][2])
		);
	}
	template<typename Archive> void serialize(Archive& archive, glm::mat4& m) {
	    archive(
	    	CEREAL_NVP(m[0][0]), CEREAL_NVP(m[0][1]), CEREAL_NVP(m[0][2]), CEREAL_NVP(m[0][3]),
	    	CEREAL_NVP(m[1][0]), CEREAL_NVP(m[1][1]), CEREAL_NVP(m[1][2]), CEREAL_NVP(m[1][3]),
	    	CEREAL_NVP(m[2][0]), CEREAL_NVP(m[2][1]), CEREAL_NVP(m[2][2]), CEREAL_NVP(m[2][3]),
	    	CEREAL_NVP(m[3][0]), CEREAL_NVP(m[3][1]), CEREAL_NVP(m[3][2]), CEREAL_NVP(m[3][3])
	    );
	}
}

//The renderer receives an array of RenderDatas,
//looping through each RenderData and renders the
//model given by modelId and modelMatrix
class Gamedata;
struct RenderData {
	glm::mat4 modelMatrix;
	int vertexArray;
	RenderData() {};
	RenderData(const glm::mat4& modelMatrix, const int vertexArray);

	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			CEREAL_NVP(modelMatrix),
			CEREAL_NVP(vertexArray)
		); 
	}
};

#endif //RENDERDATA_HPP