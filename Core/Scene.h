#pragma once

#include <list>
#include <thread>
#include <iterator>

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
			for (int i = 0; i < 1200; ++i)
			{
				addFirework(m_random.genInt(100, 1000),
					m_random.genColor(),
					static_cast<float>(m_random.genInt(1, 5)) / 100.0f,
					m_random.genVec3(),
					static_cast<float>(m_random.genInt(5, 10)) / 1000.0f);
			}
		}

		~Scene() = default;

		void addFirework(std::size_t const cnt,
			             glm::vec4 const&  color,
			             float const       launchSpeed,
			             glm::vec3 const&  launchPosition,
			             float const       explodeSpeed)
		{
			m_fireworks.emplace_back(cnt, color, launchSpeed, launchPosition, explodeSpeed);
		}

		void popFirework()
		{
			if (m_fireworks.empty())
			{
				return;
			}
			m_fireworks.pop_back();
		}

		void update(float deltaTime)
		{
			checkKeyEvent();
			tickCamera(deltaTime);
			if (not m_paused)
			{
				tickObjects(deltaTime);
			}
			render();
		}

		void checkKeyEvent()
		{
			if (Window::get().isSpaceKeyPressed())
			{
				m_paused = not m_paused;
			}
			if (Window::get().isJKeyPressed())
			{
				addFirework(m_random.genInt(100, 1000),
					m_random.genColor(),
					static_cast<float>(m_random.genInt(1, 5)) / 100.0f,
					m_random.genVec3(),
					static_cast<float>(m_random.genInt(5, 10)) / 1000.0f);
			}
			if (Window::get().isKKeyPressed())
			{
				popFirework();
			}
		}

		void tickCamera(float deltaTime)
		{
			m_cameraController.update(deltaTime);
		}

		void tickObjects(float deltaTime)
		{
#if 1
			int p = std::thread::hardware_concurrency();
			std::vector<std::thread> threads;
			auto iter = m_fireworks.begin();
			for (int id = 0; id < p; ++id)
			{
				int first = (id + 0) * m_fireworks.size() / p;
				int last  = (id + 1) * m_fireworks.size() / p;
				int size  = last - first;
				auto next = std::next(iter, size);

				auto func =
					[](auto first, auto last, float deltaTime)
				{
					while (first not_eq last)
					{
						first->tick(deltaTime);
						std::advance(first, 1);
					}
				};
				threads.emplace_back(func, iter, next, deltaTime);
				iter = next;
			}
			for (auto& each : threads)
			{
				each.join();
			}
#else
			for (auto& each : m_fireworks)
			{
				each.tick(deltaTime);
			}
#endif
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
		std::list<MissileGroup>   m_fireworks;
		bool                      m_paused;
	};
}