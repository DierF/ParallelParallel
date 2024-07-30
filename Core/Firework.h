#pragma once

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Core/Particle.h"

namespace PParallel
{
	class Firework
	{
	public:
		Firework(glm::vec4 const& color,
			     glm::vec3 const& position,
			     glm::vec3 const& velocity,
			     float            lifetime,
				 float            radius)
			: m_color(color),
			m_position(position),
			m_velocity(velocity),
			m_lifetime(lifetime),
			m_radius(radius)
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
			// lifetime: float
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(10 * sizeof(float)));
			// size: float
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(11 * sizeof(float)));

			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		~Firework() = default;

		void tick(float deltaTime)
		{
			// update lifetime
			m_lifetime -= deltaTime;

			// update old particles
			for (auto& p : m_tail)
			{
				// update lifetime
				p.lifetime -= deltaTime;
				//if (p.lifetime <= 0.0f)
				//{
				//	continue;
				//}

				// update color
				if (p.lifetime < 1000.0f)
				{
					p.color = m_color * (p.lifetime / 1000.0f);
				}

				// update position
				p.position += 0.001f * deltaTime * p.velocity;

				// update velocity
				// apply gravity
				p.velocity.y -= 0.001f * deltaTime * 10.0f;
				// apply air resistance
				//m_velocity *= 0.99f;
			}
			// erase dead
			auto func = [](auto const& p) { return p.lifetime > 0.0f; };
			m_tail.erase(std::partition(m_tail.begin(), m_tail.end(), func), m_tail.end());

			if (m_lifetime > 0.0f)
			{
				// update position
				m_position += 0.001f * deltaTime * m_velocity;

				// update velocity
				// apply gravity
				m_velocity.y -= 0.001f * deltaTime * 10.0f;
				// apply air resistance
				//m_velocity *= 0.99f;

				// emit new particles
				float cnt = 50.0f;
				// golden angle in radians
				constexpr float const phi = std::numbers::pi_v<float> * 2 * (std::numbers::phi_v<float> - 1.0f);
				for (float i = 0.0f; i < cnt; ++i)
				{
					// y goes from 1 to - 1
					float y = 1.0f - (i / (cnt - 1.0f)) * 2.0f;
					// radius at this y height
					float radiusAtY = std::sqrt(1.0f - y * y);
					// golden angle increment
					float theta = phi * i;
					float x = std::cos(theta) * radiusAtY;
					float z = std::sin(theta) * radiusAtY;
					glm::vec3 position = m_position + glm::vec3(x, y, z) * m_radius;
					m_tail.emplace_back(m_color, position, glm::vec3(0.0f), m_lifetime);
				}
			}
		}

		bool alive()
		{
			return m_lifetime > 0.0f;
		}

		auto explode()
		{
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

		float     m_radius;

		std::vector<Particle> m_tail;
	};
}