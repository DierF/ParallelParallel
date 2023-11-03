#pragma once

#include <glm/glm.hpp>

#include "Core/Missile.h"

namespace PParallel
{
	class MissileController
	{
	public:
		static void update(Missile& missile, glm::vec3 const& translation, float angle, glm::vec3 const& axis)
		{
			missile.translate(translation);
			missile.rotate(angle, axis);
		}
	};
}