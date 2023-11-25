#pragma once

#include <glm/glm.hpp>

#include "Core/Missile.h"
#include "Core/Particle.h"

namespace PParallel
{
	class MissileController
	{
	public:
		static void resetPosition(Missile& missile, glm::vec3 const& position)
		{
			missile.p0.position = position;
			missile.p1.position = position;
			missile.p2.position = position;
			missile.p3.position = position;
			missile.p4.position = position;
		}

		static void resetColor(Missile& missile, glm::vec4 const& color)
		{
			missile.p0.color = color;
			missile.p1.color = color;
			missile.p2.color = color;
			missile.p3.color = color;
			missile.p4.color = color;
		}

		static void resetSize(Missile& missile, float size0 = 3.0f)
		{
			missile.p0.size = size0;
			missile.p1.size = missile.p0.size * 0.8f;
			missile.p2.size = missile.p1.size * 0.7f;
			missile.p3.size = missile.p2.size * 0.6f;
			missile.p4.size = missile.p3.size * 0.5f;
		}

		static void move(Missile& missile, MissileStat& missileStat, float deltaTime)
		{
			// Gravity
			//missileStat.velocity.y += -0.0000009f;
			// Air Resistance
			missileStat.velocity -= missileStat.velocity * 0.0001f;
			
			missile.p0.position += missileStat.velocity * deltaTime * 1.00f;
			missile.p1.position += missileStat.velocity * deltaTime * 0.99f;
			missile.p2.position += missileStat.velocity * deltaTime * 0.98f;
			missile.p3.position += missileStat.velocity * deltaTime * 0.97f;
			missile.p4.position += missileStat.velocity * deltaTime * 0.96f;
		}
	};
}