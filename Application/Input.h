#pragma once

#include <glm/glm.hpp>

#include "Application/KeyCode.h"
#include "Application/MouseCode.h"
#include "Application/Window.h"

namespace PParallel
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key)
		{
			int state = glfwGetKey(Window::get().nativeWindow(), key);
			return state == GLFW_PRESS;
		}

		static bool IsMousePressed(MouseCode mouse)
		{
			int state = glfwGetMouseButton(Window::get().nativeWindow(), mouse);
			return state == GLFW_PRESS;
		}

		static glm::vec2 GetMousePosition()
		{
			double x, y;
			glfwGetCursorPos(Window::get().nativeWindow(), &x, &y);
			return { (float)x, (float)y };
		}
	};
}