#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PParallel
{
	class Instance
	{
	public:
		void createInstance()
		{
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Scene";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			uint32_t     glfwExtensionCount{ 0U };
			char const** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;
			createInfo.enabledLayerCount = 0U;


			VkResult result{ vkCreateInstance(&createInfo, nullptr, &m_instance) };
			if (result not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create instance!");
			}
		}

		VkInstance& get()
		{
			return m_instance;
		}

		VkInstance* ptr()
		{
			return &m_instance;
		}

	private:
		VkInstance m_instance;
	};
}