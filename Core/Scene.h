#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Core/CameraController.h"
#include "Core/MissileGroup.h"
#include "Core/Random.h"
#include "Core/Ground.h"
#include "Render/Renderer.h"

namespace PParallel
{
	class Scene
	{
	public:
		Scene(std::size_t attackerCount    = 10000ULL,
			  std::size_t interceptorCount = 10000ULL)
			: m_attackers(attackerCount),
			  m_attackerTargets(attackerCount),
			  m_interceptors(interceptorCount),
			  m_interceptorTargets(interceptorCount),
			  m_paused(true)
		{
			for (std::size_t i = 0ULL; i < attackerCount; ++i)
			{
				MissileController::resetPosition(m_attackers[i], m_random.genVec3());
				m_attackerTargets[i] = m_random.genVec3(500.0f);
			}

			for (std::size_t i = 0ULL; i < interceptorCount; ++i)
			{
				MissileController::resetPosition(m_interceptors[i], glm::vec3(0.0f, 0.0f, 0.0f));
				m_interceptorTargets[i] = &m_attackers[i];
			}

			for (auto& interceptor : m_interceptors)
			{
				MissileController::resetColor(interceptor, glm::vec3(1.0f, 0.0f, 0.0f));
			}
		};

		~Scene() = default;

		void update(float deltaTime)
		{
			if (Window::get().isSpaceKeyPressed())
			{
				m_paused = not m_paused;
			}
			tickCamera(deltaTime);
			if (not m_paused)
			{
				tickObjects(deltaTime);
			}
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
			for (std::size_t i = 0ULL; i < m_attackers.size(); ++i)
			{
				if (MissileController::move(m_attackers[i], m_attackerTargets[i], 0.009f * deltaTime))
				{
					MissileController::resetPosition(m_attackers[i], glm::vec3(0.0f));
				}
			}
		}
		
		void tickInterceptors(float deltaTime)
		{
			for (std::size_t i = 0ULL; i < m_interceptors.size(); ++i)
			{
				if (MissileController::move(m_interceptors[i],
					m_interceptorTargets[i]->m_translation0, 0.01f * deltaTime))
				{
					MissileController::resetPosition(m_attackers[i], m_random.genVec3());
					MissileController::resetPosition(m_interceptors[i], glm::vec3(0.0f, 0.0f, 0.0f));
				}
			}
		}

		void render()
		{
			m_renderer.clearBuffer();
			m_renderer.renderCamera(m_cameraController);
			m_renderer.renderMissiles(m_attackers);
			m_renderer.renderMissiles(m_interceptors);
			m_renderer.renderGround(m_ground);
		}

	private:
		CameraController       m_cameraController;
		Renderer               m_renderer;
		Random                 m_random;
		Ground                 m_ground;
		MissileGroup	       m_attackers;
		std::vector<glm::vec3> m_attackerTargets;
		MissileGroup	       m_interceptors;
		std::vector<Missile*>  m_interceptorTargets;
		bool                   m_paused;
	};
}