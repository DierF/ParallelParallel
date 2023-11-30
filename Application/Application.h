#pragma once

#include <iostream>
#include <format>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Application/Time.h"
#include "Core/Scene.h"

namespace PParallel
{
	class Application
	{
	public:
		Application() = default;

		~Application() = default;

		void run()
		{
			int total = 0;
			int cnt   = 0;
			m_timer.reset();
			while (not m_window.shouldClose())
			{
				float deltaTime = m_timer.elapsedMilliseconds();
				m_timer.reset();
				int curr = static_cast<int>(1000.0f / deltaTime);
				if (cnt > 100)
				{
					total = 0;
					cnt = 0;
				}
				total += curr;
				++cnt;
				std::cout << std::format("FPS: {:-4d}  AVERAGE FPS: {}\n", curr, total / cnt);
				m_scene.update(deltaTime);
				m_window.update();
			}
		}

	private:
		Window m_window;
		Scene  m_scene;
		Timer  m_timer;
	};
}