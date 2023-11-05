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
			MissileController::update(m_missile, glm::vec3(0.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
		}

		void render()
		{
			m_renderer.clear();
			m_renderer.renderCamera(m_cameraController);
			m_renderer.renderMissile(m_missile);
		}

	private:
		CameraController  m_cameraController;
		Missile			  m_missile;
		Renderer          m_renderer;
	};
}