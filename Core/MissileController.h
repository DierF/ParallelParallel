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
			missile.m_translation0 += translation;
			missile.m_translation1 += translation;
			missile.m_translation2 += translation;
			missile.m_translation3 += translation;
			missile.m_translation4 += translation;
		}

		static void resetPosition(Missile& missile, glm::vec3 const& position)
		{
			missile.m_translation0 = position;
			missile.m_translation1 = position;
			missile.m_translation2 = position;
			missile.m_translation3 = position;
			missile.m_translation4 = position;
		}

		static void resetColor(Missile& missile, glm::vec3 const& color)
		{
			missile.m_color0 = color;
			missile.m_color1 = color;
			missile.m_color2 = color;
			missile.m_color3 = color;
			missile.m_color4 = color;
		}

		static bool move(Missile& missile, glm::vec3 const& target, float speed)
		{
			glm::vec3 translation = target - missile.m_translation0;
			if (glm::length(translation) <= 1.0f)
			{
				return true;
			}
			translate(missile, glm::normalize(translation) * speed);
			return false;
		}
	};
}