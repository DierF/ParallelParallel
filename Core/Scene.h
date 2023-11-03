#pragma once

#include "Application/Time.h"
#include "Core/CameraController.h"
#include "Core/Missile.h"
#include "Core/MissileController.h"
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
			m_missile.init();
		}

		void update(float deltaTime)
		{
			m_renderer.draw();

			m_cameraController.update(deltaTime);

			m_renderer.refShader().updateUniformMat4(m_renderer.refShader().getUniformLocation("u_viewProjection"),
				m_cameraController.refCamera().getViewProjectionMatrix());

			float angle = 0.0005f * deltaTime;
			MissileController::update(m_missile, glm::vec3(0.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
			m_missile.render(m_renderer.refShader());
		}

	private:
		CameraController  m_cameraController;
		Missile			  m_missile;
		Renderer          m_renderer;
	};
}