#pragma once

#include <vector>
#include <thread>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Application/FileReader.h"
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
			: m_paused(false), m_threadCount(std::thread::hardware_concurrency())
		{
			int initialFireworkNum = std::stoi(FileReader::readFile("../InitialFireworkCount.txt"));

			addFirework(5000, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.03f, glm::vec3(0.0f), 0.08f);

			for (float i = -100.0f; i < 100.0f; i += 10.0f)
			{
				addFirework(m_random.genInt(100, 500),
					m_random.genColor(),
					(i + 300.0f) / 10000.0f,
					glm::vec3(i, 0.0f, -100.0f),
					static_cast<float>(m_random.genInt(5, 10)) / 1000.0f);
				addFirework(m_random.genInt(100, 500),
					m_random.genColor(),
					(i + 300.0f) / 10000.0f,
					glm::vec3(-i, 0.0f, 100.0f),
					static_cast<float>(m_random.genInt(5, 10)) / 1000.0f);
				addFirework(m_random.genInt(100, 500),
					m_random.genColor(),
					(i + 300.0f) / 10000.0f,
					glm::vec3(100.0f, 0.0f, i),
					static_cast<float>(m_random.genInt(5, 10)) / 1000.0f);
				addFirework(m_random.genInt(100, 500),
					m_random.genColor(),
					(i + 300.0f) / 10000.0f,
					glm::vec3(-100.0f, 0.0f, -i),
					static_cast<float>(m_random.genInt(5, 10)) / 1000.0f);
			}

			for (int i = 0; i < initialFireworkNum; ++i)
			{
				addFirework(m_random.genInt(100, 500),
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
			if (!m_paused)
			{
				tick(deltaTime);
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

		void tick(float deltaTime)
		{
			// If not much work to do, then do it in serial.
			if (m_fireworks.size() < m_threadCount * 100ULL)
			{
				for (auto& firework : m_fireworks)
				{
					firework.tick(deltaTime);
				}
				return;
			}
			std::vector<std::thread> threads;
			for (unsigned id = 0U; id < m_threadCount; ++id)
			{
				std::size_t first = (id + 0ULL) * m_fireworks.size() / m_threadCount;
				std::size_t last  = (id + 1ULL) * m_fireworks.size() / m_threadCount;

				auto func =
					[](auto first, auto last, float deltaTime)
				{
					while (first != last)
					{
						first->tick(deltaTime);
						++first;
					}
				};
				threads.emplace_back(func, m_fireworks.begin() + first, m_fireworks.begin() + last, deltaTime);
			}
			for (auto& each : threads)
			{
				each.join();
			}
		}

		void render()
		{
			m_renderer.clearBuffer();
			m_cameraController.render(m_renderer.getShader());
			m_ground.render();
			for (auto& each : m_fireworks)
			{
				each.render();
			}
		}

	private:
		CameraController          m_cameraController;
		Renderer                  m_renderer;
		Random                    m_random;
		Ground                    m_ground;
		std::vector<MissileGroup> m_fireworks;
		bool                      m_paused;

	private:
		unsigned m_threadCount;
	};
}