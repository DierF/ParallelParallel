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
			missile.translate(translation);
		}

		static void rotate(Missile& missile, float angle, glm::vec3 const& axis)
		{
			missile.rotate(angle, axis);
		}

		static void reset(Missile& missile, glm::vec3 const& position)
		{
			missile.reset(position);
		}
	};
}