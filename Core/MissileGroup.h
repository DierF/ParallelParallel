#pragma once

#include <vector>
#include <numbers>

#include "Core/Missile.h"
#include "Core/MissileController.h"
#include "Core/Particle.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

namespace PParallel
{
	class MissileGroup
	{
	public:
		MissileGroup() = delete;

		MissileGroup(std::size_t const cnt,
			         glm::vec4 const&  color,
			         float const       launchSpeed,
			         glm::vec3 const&  launchPosition,
		             float const       explodeSpeed)
			: m_missiles(cnt), m_missileStats(cnt), m_exploded(false),
			m_color(color), m_launchSpeed(launchSpeed),
			m_launchPosition(launchPosition), m_explodeSpeed(explodeSpeed)
		{
			for (auto& each : m_missiles)
			{
				MissileController::resetPosition(each, m_launchPosition);
				MissileController::resetSize(each);
				MissileController::resetColor(each, m_color);
			}

			for (auto& each : m_missileStats)
			{
				each.velocity.y = m_launchSpeed;
			}

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

		void tick(float deltaTime)
		{
			if (not m_exploded and MissileController::isGone(m_missiles.front()))
			{
				explode();
				return;
			}

			glm::vec4 color = m_missiles[0].p0.color;
			glm::vec3 diff = glm::normalize(glm::vec3(color.r, color.g, color.b)) * deltaTime * 0.0006f;
			color.r -= diff.r;
			color.g -= diff.g;
			color.b -= diff.b;
			if (m_exploded and MissileController::isGone(m_missiles.front()))
			{
				reset();
				return;
			}
			for (std::size_t i = 0ULL; i < m_missiles.size(); ++i)
			{
				MissileController::move(m_missiles[i], m_missileStats[i], deltaTime);
				MissileController::resetColor(m_missiles[i], color);
			}
		}

		void explode()
		{
			m_exploded = true;

			glm::vec3 explosionPoint = m_missiles.front().p0.position;
			for (auto& each : m_missiles)
			{
				MissileController::resetPosition(each, explosionPoint);
				MissileController::resetColor(each, m_color);
			}

			// golden angle in radians
			float phi = std::numbers::pi_v<float> * 2 * (std::numbers::phi_v<float> - 1.0f);
			for (std::size_t i = 0ULL; i < m_missiles.size(); ++i)
			{
				// y goes from 1 to - 1
				float y = 1.0f - (static_cast<float>(i) / static_cast<float>(m_missiles.size() - 1ULL)) * 2.0f;
				// radius at this y height
				float radiusAtY = std::sqrt(1.0f - y * y);
				// golden angle increment
				float theta = phi * static_cast<float>(i);
				float x = std::cos(theta) * radiusAtY;
				float z = std::sin(theta) * radiusAtY;

				m_missileStats[i].velocity = glm::normalize(glm::vec3(x, y, z)) * m_explodeSpeed;
			}
		}

		void render()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			m_vertexBuffer.bufferData(m_missiles.size() * sizeof(Missile), m_missiles.data(), GL_STATIC_DRAW);

			glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_missiles.size()) * (sizeof(Missile) / sizeof(Particle)));
			
			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		void reset()
		{
			for (auto& each : m_missiles)
			{
				MissileController::resetPosition(each, m_launchPosition);
				MissileController::resetSize(each);
				MissileController::resetColor(each, m_color);
			}

			for (auto& each : m_missileStats)
			{
				each.velocity.x = 0.0f;
				each.velocity.y = m_launchSpeed;
				each.velocity.z = 0.0f;
			}
			m_exploded = false;
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
		VertexArray              m_vertexArray;
		VertexBuffer             m_vertexBuffer;
		std::vector<Missile>     m_missiles;
		std::vector<MissileStat> m_missileStats;
		glm::vec4                m_color;
		float                    m_launchSpeed;
		glm::vec3                m_launchPosition;
		float                    m_explodeSpeed;
		bool                     m_exploded;
	};
}