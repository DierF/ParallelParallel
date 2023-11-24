#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Core/CameraController.h"
#include "Core/MissileGroup.h"
#include "Core/MissileController.h"
#include "Core/Random.h"
#include "Core/Ground.h"
#include "Render/Renderer.h"

namespace PParallel
{
	class Scene
	{
	public:
		Scene(std::size_t cnt = 1ULL)
			: m_fireworks(cnt),
			  m_paused(true)
		{
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
			for (auto& each : m_fireworks)
			{
				each.tick(deltaTime);
			}
		}

		void render()
		{
			m_renderer.clearBuffer();
			m_renderer.renderCamera(m_cameraController);
			for (auto& each : m_fireworks)
			{
				m_renderer.renderMissiles(each);
			}
			m_renderer.renderGround(m_ground);
		}

	private:
		CameraController          m_cameraController;
		Renderer                  m_renderer;
		Random                    m_random;
		Ground                    m_ground;
		std::vector<MissileGroup> m_fireworks;
		bool                      m_paused;
	};
}