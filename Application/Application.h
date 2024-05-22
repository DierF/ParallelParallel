#pragma once

#include <iostream>
#include <cmath>

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
			while (not m_window.shouldClose())
			{
				float deltaTime = m_timer.elapsedMilliseconds();
				m_timer.reset();
				long fps = std::lround(1000.0f / deltaTime);
				m_scene.update(deltaTime);
				m_window.update(fps);
			}
		}

	private:
		Window m_window;
		Scene  m_scene;
		Timer  m_timer;
	};
}