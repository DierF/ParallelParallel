#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Core/CameraController.h"
#include "Core/MissileGroup.h"
#include "Render/Renderer.h"

namespace PParallel
{
	class Scene
	{
	public:
		Scene(std::size_t attackerCount    = 10ULL,
			  std::size_t interceptorCount = 10ULL)
			: m_attackers(attackerCount),
			  m_attackerTargets(attackerCount),
			  m_attackerAimed(attackerCount),
			  m_interceptors(interceptorCount),
			  m_interceptorTargets(interceptorCount)
		{
			for (std::size_t i = 0ULL; i < attackerCount; ++i)
			{
				m_attackers[i]       = glm::vec3(-5.0f + static_cast<float>(i), 0.0f, 0.0f);
				m_attackerTargets[i] = glm::vec3(-5.0f + static_cast<float>(i), 10.0f, 0.0f);
			}

			for (std::size_t i = 0ULL; i < interceptorCount; ++i)
			{
				m_interceptorTargets[i] = &m_attackers[i];
			}

			for (auto& interceptor : m_interceptors)
			{
				interceptor.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
			}
		};

		~Scene() = default;

		void update(float deltaTime)
		{
			tickCamera(deltaTime);
			tickObjects(deltaTime);
			render();
		}

		void tickCamera(float deltaTime)
		{
			m_cameraController.update(deltaTime);
		}

		void tickObjects(float deltaTime)
		{
			tickAttackers(deltaTime);
			tickInterceptors(deltaTime);
		}

		void tickAttackers(float deltaTime)
		{
			float angle = 0.0005f * deltaTime;
			for (std::size_t i = 0ULL; i < m_attackers.size(); ++i)
			{
				MissileController::rotate(m_attackers[i], angle, glm::vec3(0.0f, 0.0f, 1.0f));

				if (glm::distance(m_attackers[i].getPosition(), m_attackerTargets[i]) <= 1.0f)
				{
					MissileController::reset(m_attackers[i], glm::vec3(0.0f));
				}
				else
				{
					glm::vec3 translate = m_attackerTargets[i] - m_attackers[i].getPosition();
					MissileController::translate(m_attackers[i], glm::normalize(translate) * 0.001f);
				}
			}
		}
		
		void tickInterceptors(float deltaTime)
		{
			float angle = 0.0005f * deltaTime;
			for (std::size_t i = 0ULL; i < m_interceptors.size(); ++i)
			{
				glm::vec3 translate = m_interceptorTargets[i]->getPosition() - m_interceptors[i].getPosition();
				MissileController::translate(m_interceptors[i], glm::normalize(translate) * 0.002f);
			}
		}

		void render()
		{
			m_renderer.clear();
			m_renderer.renderCamera(m_cameraController);
			m_renderer.renderMissiles(m_attackers);
			m_renderer.renderMissiles(m_interceptors);
		}

	private:
		CameraController       m_cameraController;
		Renderer               m_renderer;
		MissileGroup	       m_attackers;
		std::vector<glm::vec3> m_attackerTargets;
		std::vector<bool>      m_attackerAimed;
		MissileGroup	       m_interceptors;
		std::vector<Missile*>  m_interceptorTargets;
	};
}