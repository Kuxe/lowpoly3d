#include <glm/glm.hpp>
#include "modelgenerator.hpp"

class PlaneGenerator : public ModelGenerator {
private:
	glm::vec3 normal;
	Color color;
	uint8_t subdivides;
public:
	PlaneGenerator(const glm::vec3& normal = {0.0, 1.0, 0.0}, const Color& color = {255, 0, 255}, const uint8_t subdivies = 0);
	Model generate() override;
};