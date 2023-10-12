#pragma once

#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "QueueFamilyIndices.h"

namespace PParallel
{
	class CommandPool
	{
	public:
		void createCommandPool(VkSurfaceKHR& surface,
							   VkPhysicalDevice& physicalDevice,
							   VkDevice& device)
		{
			QueueFamilyIndices queueFamilyIndices{ findQueueFamilies(surface, physicalDevice) };

			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

			if (vkCreateCommandPool(device, &poolInfo, nullptr, &m_commandPool) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create command pool!");
			}
		}

		VkCommandPool& get()
		{
			return m_commandPool;
		}

		VkCommandPool* ptr()
		{
			return &m_commandPool;
		}

	private:
		VkCommandPool m_commandPool;
	};
}