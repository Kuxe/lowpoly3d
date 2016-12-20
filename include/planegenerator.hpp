#include <glm/glm.hpp>
#include "modelgenerator.hpp"

class PlaneGenerator : public ModelGenerator {
private:
	glm::vec3 normal;
	Color color;
public:
	PlaneGenerator(const glm::vec3& normal = {0.0, 1.0, 0.0}, const Color& color = {255, 0, 255});
	Model generate() override;
};