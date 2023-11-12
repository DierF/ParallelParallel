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
			// Step 1: Get the current orientation
			glm::vec3 currentForward = glm::vec3(missile.getRotation() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

			// Step 2: Calculate the view matrix to look at the target direction
			glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f), target - missile.getPosition(),
											   glm::vec3(0.0f, 1.0f, 0.0f));

			// Step 3: Convert the view matrix to a quaternion
			glm::quat rotationQuat = glm::quat_cast(viewMatrix);

			// Step 4: Combine the rotations
			glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);

			// Step 5: Update the object's transformation matrix
			missile.resetRotation(rotationMatrix);
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