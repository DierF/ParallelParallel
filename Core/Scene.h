#pragma once

#include "Application/Time.h"
#include "Core/CameraController.h"
#include "Core/MissileGroup.h"
#include "Render/Renderer.h"

namespace PParallel
{
	class Scene
	{
	public:
		Scene() = default;

		~Scene() = default;

		void init()
		{
			m_renderer.init();
			m_attackers.init();
			m_interceptors.init();
			for (auto& interceptor : m_interceptors)
			{
				interceptor.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}

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
			float angle = 0.0005f * deltaTime;
			MissileController::rotate(*m_attackers.begin(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
			MissileController::reset(*m_interceptors.begin(), glm::vec3(1.0f, 0.0f, 0.0f));
			MissileController::rotate(*m_interceptors.begin(), angle, glm::vec3(0.0f, 0.0f, -1.0f));
		}

		void render()
		{
			m_renderer.clear();
			m_renderer.renderCamera(m_cameraController);
			m_renderer.renderMissiles(m_attackers);
			m_renderer.renderMissiles(m_interceptors);
		}

	private:
		CameraController m_cameraController;
		MissileGroup	 m_attackers;
		MissileGroup	 m_interceptors;
		Renderer         m_renderer;
	};
}