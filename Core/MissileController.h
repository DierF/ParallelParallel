#pragma once

#include <glm/glm.hpp>

#include "Core/Missile.h"

namespace PParallel
{
	class MissileController
	{
	public:
		static void translate(Missile& missile, glm::vec3 const& translation)
		{
		}

		static void resetPosition(Missile& missile, glm::vec3 const& position)
		{
		}

		static void resetColor(Missile& missile, glm::vec4 const& color)
		{
		}

		static bool move(Missile& missile, glm::vec3 const& target, float speed)
		{
			//glm::vec3 translation = target - missile.m_translation0;
			//if (glm::length(translation) <= 1.0f)
			//{
			//	return true;
			//}
			//translate(missile, glm::normalize(translation) * speed);
			//return false;
		}
	};
}