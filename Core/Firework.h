#pragma once

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

#include "Core/Particle.h"
#include "Core/Explosion.h"

namespace PParallel
{
	class Firework
	{
	public:
		Firework(glm::vec4 const& color,
			glm::vec3 const&      position,
			glm::vec3 const&      velocity,
			float                 lifetime,
			std::size_t           tailSize,
			float                 tailLife,
			ExplosionType         explosion_t)
			: m_color(color),
			m_position(position),
			m_velocity(velocity),
			m_lifetime(lifetime),
			m_tailSize(tailSize),
			m_tailLife(tailLife),
			m_explosion_t(explosion_t)
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			// Based on how Particle is defined
			// color: vec4
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
			// position: vec3
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
			// velocity: vec3
			// UNUSED by shader
			// size: float
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(10 * sizeof(float)));
			// lifetime: float
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(11 * sizeof(float)));

			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		~Firework() = default;

		void tick(float deltaTime)
		{
			// update lifetime
			m_age += deltaTime;

			// update old particles
			for (auto& p : m_tail)
			{
				// update lifetime
				p.lifetime -= deltaTime;
				if (p.lifetime <= 0.0f)
				{
					continue;
				}

				// update position
				p.position += 0.001f * deltaTime * p.velocity;

				// update velocity
				// apply air resistance
				p.velocity -= 0.001f * deltaTime * p.velocity * 0.5f;
			}
			// erase dead
			auto func = [](auto const& p) { return p.lifetime > 0.0f; };
			m_tail.erase(std::partition(m_tail.begin(), m_tail.end(), func), m_tail.end());

			if (m_age < m_lifetime)
			{
				// update position
				m_position += 0.001f * deltaTime * m_velocity;

				// apply gravity
				m_velocity.y -= 0.001f * deltaTime * 10.0f;

				// emit new particles
				m_tail.emplace_back(m_color, m_position, m_velocity, m_tailSize, m_tailLife);
			}
		}

		bool alive()
		{
			return m_age < m_lifetime || !m_tail.empty();
		}

		auto explode()
		{
			return Exploder::explode(m_color, m_position, m_velocity,
									 m_lifetime, m_tailSize, m_tailLife, m_explosion_t);
		}

		void render()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			m_vertexBuffer.bufferData(m_tail.size() * sizeof(Particle), m_tail.data(), GL_STATIC_DRAW);

			glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_tail.size()) * sizeof(Particle));

			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

	private:
		VertexArray  m_vertexArray;
		VertexBuffer m_vertexBuffer;

		glm::vec4 m_color;
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		float     m_lifetime;
		float     m_age = 0.0f;

		std::size_t           m_tailSize;
		float                 m_tailLife;
		std::vector<Particle> m_tail;

		ExplosionType m_explosion_t;
	};
}