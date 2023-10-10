#pragma once

#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PParllel
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value();
		}
	};

	class Application
	{
	public:
		static uint32_t const WIDTH { 800U };
		static uint32_t const HEIGHT{ 600U };

	public:
		void run()
		{
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}

	private:
		void initWindow()
		{
			glfwInit();

			//  do not create an OpenGL context
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			m_window = glfwCreateWindow(WIDTH, HEIGHT, "Application", nullptr, nullptr);
		}

		void initVulkan()
		{
			createInstance();
			pickPhysicalDevice();
			createLogicalDevice();
		}

		void mainLoop()
		{
			while (not glfwWindowShouldClose(m_window))
			{
				glfwPollEvents();
			}
		}

		void cleanup()
		{
			vkDestroyDevice(m_device, nullptr);

			vkDestroyInstance(m_instance, nullptr);

			glfwDestroyWindow(m_window);

			glfwTerminate();
		}

		void createInstance()
		{
			VkApplicationInfo appInfo{};
			appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName   = "Scene";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName        = "No Engine";
			appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion         = VK_API_VERSION_1_0;


			uint32_t     glfwExtensionCount{ 0U };
			char const** glfwExtensions    { glfwGetRequiredInstanceExtensions(&glfwExtensionCount)};

			VkInstanceCreateInfo createInfo{};
			createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo        = &appInfo;
			createInfo.enabledExtensionCount   = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;
			createInfo.enabledLayerCount       = 0U;

			
			VkResult result{ vkCreateInstance(&createInfo, nullptr, &m_instance) };
			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create instance!");
			}
		}

		void pickPhysicalDevice()
		{
			uint32_t deviceCount{ 0U };
			vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
			if (deviceCount == 0)
			{
				throw std::runtime_error("failed to find a physical device!");
			}
			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

			for (auto const& device : devices)
			{
				if (isDeviceSuitable(device))
				{
					m_physicalDevice = device;
					break;
				}
			}
			if (m_physicalDevice == VK_NULL_HANDLE)
			{
				throw std::runtime_error("failed to find a suitable physical device!");
			}
		}

		bool isDeviceSuitable(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices{ findQueueFamilies(device) };
			return indices.isComplete();
		}

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount{ 0U };
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			uint32_t idx{ 0U };
			for (auto const& queueFamily : queueFamilies)
			{
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.graphicsFamily = idx;
				}

				if (indices.isComplete())
				{
					break;
				}

				++idx;
			}

			return indices;
		}

		void createLogicalDevice()
		{
			QueueFamilyIndices indices{ findQueueFamilies(m_physicalDevice) };

			float queuePriority{ 1.0f };

			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
			queueCreateInfo.queueCount       = 1U;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pQueueCreateInfos = &queueCreateInfo;
			createInfo.queueCreateInfoCount = 1U;
			createInfo.pEnabledFeatures = &deviceFeatures;
			createInfo.enabledExtensionCount = 0U;

			if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create logical device!");
			}

			vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0U, &m_graphicsQueue);
		}

	private:
		GLFWwindow*      m_window;
		VkInstance       m_instance;
		VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
		VkDevice         m_device;
		VkQueue          m_graphicsQueue;
	};
}
