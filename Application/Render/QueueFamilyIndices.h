#pragma once

#include <optional>


namespace PParallel
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() and presentFamily.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(VkSurfaceKHR& surface,
										 VkPhysicalDevice& physicalDevice)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount{ 0U };
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		uint32_t idx{ 0U };
		for (auto const& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags bitand VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = idx;
			}

			VkBool32 presentSupport{ false };
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, idx, surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = idx;
			}

			if (indices.isComplete())
			{
				break;
			}

			++idx;
		}

		return indices;
	}
}