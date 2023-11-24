#pragma once

#include "glm/glm.hpp"

#include "Core/Particle.h"

namespace PParallel
{
	class ParticleController
	{
	public:
		static void translate(Particle& particle, glm::vec3 const& translation)
		{
			particle.position += translation;
		}

		static void resetPosition(Particle& particle, glm::vec3 const& position)
		{
			particle.position = position;
		}

		static void resetColor(Particle& particle, glm::vec4 const& color)
		{
			particle.color = color;
		}
	};
}