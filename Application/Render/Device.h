#pragma once

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PParallel
{
	class Device
	{
	public:
		void createLogicalDevice(std::vector<char const*> const& deviceExtensions,
								 VkSurfaceKHR& surface,
								 VkPhysicalDevice& physicalDevice,
								 VkQueue& graphicsQueue,
								 VkQueue& presentQueue)
		{
			QueueFamilyIndices indices{ findQueueFamilies(surface, physicalDevice) };

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies{ indices.graphicsFamily.value(), indices.presentFamily.value() };
			float queuePriority{ 1.0f };

			for (uint32_t queueFamily : uniqueQueueFamilies)
			{
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1U;
				queueCreateInfo.pQueuePriorities = &queuePriority;

				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pEnabledFeatures = &deviceFeatures;
			createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
			createInfo.ppEnabledExtensionNames = deviceExtensions.data();

			if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create logical device!");
			}

			vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0U, &graphicsQueue);
			vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0U, &presentQueue);
		}

		VkDevice& get()
		{
			return m_device;
		}

		VkDevice* ptr()
		{
			return &m_device;
		}

	private:
		VkDevice m_device;
	};
}