#pragma once

#include <vector>

#include "Core/Missile.h"
#include "Core/Particle.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

namespace PParallel
{
	class MissileGroup
	{
	public:
		MissileGroup(std::size_t cnt)
			: m_missiles(cnt)
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			m_vertexBuffer.bufferData(cnt * sizeof(Missile), m_missiles.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(7 * sizeof(float)));

			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		};
		
		~MissileGroup() = default;

		void render()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			m_vertexBuffer.bufferData(m_missiles.size() * sizeof(Missile), m_missiles.data(), GL_STATIC_DRAW);

			glDrawArrays(GL_POINTS , 0, m_missiles.size() * (sizeof(Missile) / sizeof(Particle)));
			
			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
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
		std::vector<Missile>  m_missiles;
		VertexArray           m_vertexArray;
		VertexBuffer          m_vertexBuffer;
	};
}