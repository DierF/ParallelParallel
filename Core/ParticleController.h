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
			particle.m_translation += translation;
		}

		static void resetPosition(Particle& particle, glm::vec3 const& position)
		{
			particle.m_translation = position;
		}

		static void resetColor(Particle& particle, glm::vec3 const& color)
		{
			particle.m_color = color;
		}
	};
}