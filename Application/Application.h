#pragma once

#include <iostream>
#include <format>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Render/Renderer.h"
#include "Core/Time.h"

namespace PParallel
{
	class Application
	{
	public:
		Application()
		{
			init();
			run();
		}

		~Application()
		{
		}

		void init()
		{
			m_window.init();
			m_renderer.init();
		}

		void run()
		{
			m_timer.reset();
			while (not m_window.shouldClose())
			{
				float deltaTime = m_timer.elapsedMilliseconds();
				m_timer.reset();
				std::cout << std::format("FPS: {}\n", static_cast<int>(1000.0f / deltaTime));
				m_renderer.draw(deltaTime);
				m_window.update();
			}
		}

	private:
		Window    m_window;
		Renderer  m_renderer;
		Timer     m_timer;
	};
}