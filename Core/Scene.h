#pragma once

#include <vector>
#include <list>
#include <memory>
#include <thread>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Application/FileReader.h"

#include "Core/CameraController.h"
#include "Core/Firework.h"
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

			addFirework(m_random.genColor(),
				glm::vec3(0.0f, 0.0f, -250.0f),
				glm::vec3(0.0f, 50.0f, 0.0f),
				3000.0f,
				0.25f);
		}

		~Scene() = default;

		void addFirework(glm::vec4 const& color,
						 glm::vec3 const& position,
						 glm::vec3 const& velocity,
						 float            lifetime,
						 float            radius)
		{
			m_fireworks.emplace_back(std::make_shared<Firework>(color, position, velocity, lifetime, radius));
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
			//if (Window::get().isJKeyPressed())
			//{
			//	addFirework(m_random.genInt(100, 1000),
			//		m_random.genColor(),
			//		static_cast<float>(m_random.genInt(1, 5)) / 100.0f,
			//		m_random.genVec3(),
			//		static_cast<float>(m_random.genInt(5, 10)) / 1000.0f);
			//}
			//if (Window::get().isKKeyPressed())
			//{
			//	popFirework();
			//}
		}

		void tickCamera(float deltaTime)
		{
			m_cameraController.update(deltaTime);
		}

		void tick(float deltaTime)
		{
			// If not much work to do, then do it in serial.
			//if (m_fireworks.size() < m_threadCount * 100ULL)
			//{
				for (auto& firework : m_fireworks)
				{
					firework->tick(deltaTime);
				}

				cleanup();
				return;
			//}
			//std::vector<std::thread> threads;
			//for (unsigned id = 0U; id < m_threadCount; ++id)
			//{
			//	std::size_t first = (id + 0ULL) * m_fireworks.size() / m_threadCount;
			//	std::size_t last  = (id + 1ULL) * m_fireworks.size() / m_threadCount;
			//
			//	auto func =
			//		[](auto first, auto last, float deltaTime)
			//	{
			//		while (first != last)
			//		{
			//			first->tick(deltaTime);
			//			++first;
			//		}
			//	};
			//	threads.emplace_back(func, std::next(m_fireworks.begin(), first), std::next(m_fireworks.begin(), last), deltaTime);
			//}
			//for (auto& each : threads)
			//{
			//	each.join();
			//}
			//
			//cleanup();
		}

		void cleanup()
		{
			std::vector<std::shared_ptr<Firework>> newFireworks;
			auto iter = m_fireworks.begin();
			while (iter != m_fireworks.end())
			{
				if (iter->get()->alive())
				{
					++iter;
					continue;
				}
				auto params = std::move(iter->get()->explode());
				newFireworks.insert(newFireworks.end(), params.begin(), params.end());
				iter = m_fireworks.erase(iter);
			}
			m_fireworks.insert(m_fireworks.end(), newFireworks.begin(), newFireworks.end());
		}

		void render()
		{
			m_renderer.clearBuffer();
			m_cameraController.render(m_renderer.getShader());
			m_ground.render();
			for (auto& each : m_fireworks)
			{
				each->render();
			}
		}

	private:
		Random m_random;

		Renderer m_renderer;

		CameraController m_cameraController;

		Ground              m_ground;
		std::list<std::shared_ptr<Firework>> m_fireworks;
		
		bool m_paused;

	private:
		unsigned m_threadCount;
	};
}