#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/Missile.h"

namespace PParallel
{
	class MissileController
	{
	public:
		static void translate(Missile& missile, glm::vec3 const& translation)
		{
			missile.translate(translation);
		}

		static void rotate(Missile& missile, float angle, glm::vec3 const& axis)
		{
			missile.rotate(angle, axis);
		}

		static void resetPosition(Missile& missile, glm::vec3 const& position)
		{
			missile.resetPosition(position);
		}

		static void resetRotation(Missile& missile, glm::mat4 const& rotation)
		{
			missile.resetRotation(rotation);
		}

		static void aim(Missile& missile, glm::vec3 const& target)
		{
			
		}

		static bool move(Missile& missile, glm::vec3 const& target, float speed)
		{
			glm::vec3 translation = target - missile.getPosition();
			if (glm::length(translation) <= 1.0f)
			{
				return true;
			}
			translate(missile, glm::normalize(translation) * speed);
			return false;
		}
	};
}