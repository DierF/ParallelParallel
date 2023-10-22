#pragma once

#include <glm/glm.hpp>

#include "Core/KeyCode.h"
#include "Core/MouseCode.h"
#include "Application/Window.h"

namespace PParallel
{
	class Input
	{
	public:
		static bool IsKeyPressed(GLFWwindow* window, KeyCode key)
		{
			int state = glfwGetKey(window, key);
			return state == GLFW_PRESS;
		}

		static bool IsMousePressed(GLFWwindow* window, MouseCode mouse)
		{
			int state = glfwGetMouseButton(window, mouse);
			return state == GLFW_PRESS;
		}

		static glm::vec2 GetMousePosition(GLFWwindow* window)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			return { (float)x, (float)y };
		}
	};
}