#ifndef CONEGENERATOR_HPP
#define CONEGENERATOR_HPP

#include "geometric_primitives/cone.hpp"

#include "modelgenerator.hpp"

namespace lowpoly3d
{
	// Generates cones with bases in xy-plane an with tip pointing along y-axis.
	class ConeGenerator : public ModelGenerator
	{
	public:
		// 'height' determines height of generated cones and resolution determines
		// the number of vertices at the disc-base of the cone
		ConeGenerator(Cone const& cone, std::uint32_t resolution,	Color const& color = {255, 0, 255});
		Model generate() override;
	private:
		Cone cone;
		std::uint32_t resolution;
		Color color;
	};
}

#endif // CONEGENERATOR_HPP