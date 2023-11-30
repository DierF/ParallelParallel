#pragma once

#include <list>
#include <thread>
#include <iterator>
#include <queue>
#include <mutex>
#include <condition_variable>

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
			: m_paused(false), m_p(std::thread::hardware_concurrency())
		{
			std::string s = FileReader::readFile("../CustomizedInitialFireworkNumber.txt");
			int initialFireworkNum = std::stoi(s);

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
			if (m_paused)
			{
				render();
			}
			else
			{
				//tickSerial(deltaTime);
				//render();

				tickObjects(deltaTime);
				render();

				//tickAndRender(deltaTime);

				//tickAndSyncRender(deltaTime);
			}
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

		void tickSerial(float deltaTime)
		{
			for (auto& each : m_fireworks)
			{
				each.tick(deltaTime);
			}
		}

		void tickObjects(float deltaTime)
		{
			if (m_fireworks.size() < m_p * 20ULL)
			{
				tickSerial(deltaTime);
				return;
			}
			std::vector<std::thread> threads;
			auto iter = m_fireworks.begin();
			for (unsigned id = 0; id < m_p; ++id)
			{
				std::size_t first = (id + 0ULL) * m_fireworks.size() / m_p;
				std::size_t last  = (id + 1ULL) * m_fireworks.size() / m_p;
				std::size_t size  = last - first;
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

		void tickAndRender(float deltaTime)
		{
			m_renderer.clearBuffer();
			m_cameraController.render(m_renderer.getShader());
			m_ground.render();
			for (auto& each : m_fireworks)
			{
				each.tick(deltaTime);
				each.render();
			}
		}

		void tickAndSyncRender(float deltaTime)
		{
			m_renderer.clearBuffer();
			m_cameraController.render(m_renderer.getShader());
			m_ground.render();
			if (m_fireworks.size() < m_p * 20ULL)
			{
				for (auto& each : m_fireworks)
				{
					each.tick(deltaTime);
					each.render();
				}
				return;
			}
			std::vector<std::jthread> threads;
			auto iter = m_fireworks.begin();
			for (unsigned id = 0; id < m_p; ++id)
			{
				std::size_t first = (id + 0ULL) * m_fireworks.size() / m_p;
				std::size_t last = (id + 1ULL) * m_fireworks.size() / m_p;
				std::size_t size = last - first;
				auto next = std::next(iter, size);

				//auto func =
				//	[](std::mutex& mutex,
				//	   std::list<MissileGroup>::iterator first,
				//	   std::list<MissileGroup>::iterator last,
				//	   float deltaTime)
				//{
				//	while (first not_eq last)
				//	{
				//		first->tick(deltaTime);
				//		{
				//			std::lock_guard<std::mutex> lock(mutex);
				//			first->render();
				//		}
				//		std::advance(first, 1);
				//	}
				//};
				auto func =
					[](std::mutex& mutex,
				       std::condition_variable& cv,
				       std::queue<std::list<MissileGroup>::iterator>& queue,
				       std::list<MissileGroup>::iterator first,
				       std::list<MissileGroup>::iterator last,
				       float deltaTime)
				{
					while (first not_eq last)
					{
						first->tick(deltaTime);
						{
							std::lock_guard<std::mutex> lock(mutex);
							queue.push(first);
						}
						cv.notify_one();
						std::advance(first, 1);
					}
				};
				threads.emplace_back(func, std::ref(m_mutex), std::ref(m_cv), std::ref(m_queue),
					                 iter, next, deltaTime);
				iter = next;
			}
			std::size_t cnt = 0ULL;
			while (cnt < m_fireworks.size())
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.wait(lock, [&] { return not m_queue.empty(); });
				while (not m_queue.empty())
				{
					auto iter = m_queue.front();
					m_queue.pop();
					iter->render();
					++cnt;
				}
			}
		}

	private:
		CameraController        m_cameraController;
		Renderer                m_renderer;
		Random                  m_random;
		Ground                  m_ground;
		std::list<MissileGroup> m_fireworks;
		bool                    m_paused;

	private:
		unsigned                m_p;
		std::mutex              m_mutex;
		std::condition_variable m_cv;
		std::queue<std::list<MissileGroup>::iterator> m_queue;
	};
}