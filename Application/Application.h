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

		void init()
		{
			m_window.init();
			m_scene.init();
		}

		void run()
		{
			m_timer.reset();
			while (not m_window.shouldClose())
			{
				float deltaTime = m_timer.elapsedMilliseconds();
				m_timer.reset();
				std::cout << std::format("FPS: {}\n", static_cast<int>(1000.0f / deltaTime));
				m_scene.update(deltaTime);
				m_window.update();
			}
		}

	private:
		Window    m_window;
		Scene	  m_scene;
		Timer     m_timer;
	};
}