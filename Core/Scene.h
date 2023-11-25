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
		Scene()
			: m_paused(true)
		{
			// Make sure no reallocation!
			m_fireworks.reserve(1000);

			m_fireworks.emplace_back(1000ULL,
				                     glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
				                     0.01f,
				                     glm::vec3(0.0f, 0.0f, -50.0f),
				                     0.005f);
			m_fireworks.emplace_back(1000ULL,
				                     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
				                     0.01f,
				                     glm::vec3(-20.0f, 0.0f, -50.0f),
				                     0.005f);
			m_fireworks.emplace_back(1000ULL,
				                     glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
				                     0.01f,
				                     glm::vec3(20.0f, 0.0f, -50.0f),
				                     0.005f);
		}

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