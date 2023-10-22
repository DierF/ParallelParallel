#pragma once

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
				m_renderer.draw(m_window.get(), m_timer.elapsedMilliseconds());
				m_window.update();
				m_timer.reset();
			}
		}

	private:
		Window    m_window;
		Renderer  m_renderer;
		Timer     m_timer;
	};
}