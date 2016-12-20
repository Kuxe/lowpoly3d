#include <glm/glm.hpp>
#include "modelgenerator.hpp"

class CircleGenerator : public ModelGenerator {
private:
	glm::vec3 normal;
	Color color;
public:
	CircleGenerator(const glm::vec3& normal = {0.0, 1.0, 0.0}, const Color& color = {255, 0, 255});
	Model generate() override;
};