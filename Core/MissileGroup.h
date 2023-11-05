#pragma once

#include <vector>

#include "Core/Missile.h"
#include "Core/MissileController.h"
#include "Render/Shader.h"

namespace PParallel
{
	class MissileGroup
	{
	public:
		MissileGroup(std::size_t cnt = 1ULL)
			: m_missiles(cnt)
		{
		};
		
		~MissileGroup() = default;

		void render(Shader& shader)
		{
			for (auto& missile : m_missiles)
			{
				missile.render(shader);
			}
		}

		auto begin()
		{
			return m_missiles.begin();
		}

		auto end()
		{
			return m_missiles.end();
		}

		auto size()
		{
			return m_missiles.size();
		}

		auto& operator[](std::size_t pos)
		{
			return m_missiles[pos];
		}

	private:
		std::vector<Missile> m_missiles;
	};
}