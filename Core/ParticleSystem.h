#pragma once

#include <vector>

#include "Core/Particle.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

namespace PParallel
{
	class ParticleSystem
	{
	public:
		ParticleSystem(std::size_t cnt)
			: m_particles(cnt)
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			m_vertexBuffer.bufferData(cnt * sizeof(Particle), m_particles.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		~ParticleSystem() = default;

		void render()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			m_vertexBuffer.bufferData(m_particles.size() * sizeof(Particle), m_particles.data(), GL_STATIC_DRAW);

			glDrawArrays(GL_POINTS, 0, m_particles.size());

			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		auto begin()
		{
			return m_particles.begin();
		}

		auto end()
		{
			return m_particles.end();
		}

		auto size()
		{
			return m_particles.size();
		}

		auto& operator[](std::size_t pos)
		{
			return m_particles[pos];
		}

	private:
		std::vector<Particle> m_particles;
		VertexArray           m_vertexArray;
		VertexBuffer          m_vertexBuffer;
	};
}