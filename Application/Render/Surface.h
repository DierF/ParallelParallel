#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PParallel
{
	class Surface
	{
	public:
		void createSurface(VkInstance& instance, GLFWwindow*& window)
		{
			if (glfwCreateWindowSurface(instance, window, nullptr, &m_surface) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create window surface!");
			}
		}

		VkSurfaceKHR& get()
		{
			return m_surface;
		}

		VkSurfaceKHR* ptr()
		{
			return &m_surface;
		}

	private:
		VkSurfaceKHR m_surface;
	};
}