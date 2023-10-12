#pragma once

#include <stdexcept>
#include <vector>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

namespace PParallel
{
	class PhysicalDevice
	{
	public:
		void pickPhysicalDevice(std::vector<char const*> const& deviceExtensions,
								VkInstance& instance,
								VkSurfaceKHR& surface)
		{
			uint32_t deviceCount{ 0U };
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
			if (deviceCount == 0U)
			{
				throw std::runtime_error("failed to find a physical device!");
			}
			std::vector<VkPhysicalDevice> PhysicalDevices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, PhysicalDevices.data());

			for (auto& physicalDevice : PhysicalDevices)
			{
				if (isDeviceSuitable(deviceExtensions, surface, physicalDevice))
				{
					m_physicalDevice = physicalDevice;
					break;
				}
			}
			if (m_physicalDevice == VK_NULL_HANDLE)
			{
				throw std::runtime_error("failed to find a suitable physical device!");
			}
		}
		
		static bool isDeviceSuitable(std::vector<char const*> const& deviceExtensions,
									 VkSurfaceKHR& surface,
									 VkPhysicalDevice& physicalDevice)
		{
			QueueFamilyIndices indices{ findQueueFamilies(surface, physicalDevice) };

			bool extensionsSupported{ checkDeviceExtensionSupport(deviceExtensions, physicalDevice) };

			bool swapChainAdequate{ false };
			if (extensionsSupported)
			{
				SwapChainSupportDetails swapChainSupport{ querySwapChainSupport(surface, physicalDevice) };
				swapChainAdequate = not swapChainSupport.formats.empty() and not swapChainSupport.presentModes.empty();
			}

			return indices.isComplete() and extensionsSupported and swapChainAdequate;
		}

		static bool checkDeviceExtensionSupport(std::vector<char const*> const& deviceExtensions,
												VkPhysicalDevice& physicalDevice)
		{
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
			for (auto const& extension : availableExtensions)
			{
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		static SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR& surface,
															 VkPhysicalDevice& physicalDevice)
		{
			SwapChainSupportDetails details;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
			if (formatCount not_eq 0U)
			{
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
			if (presentModeCount not_eq 0U)
			{
				details.presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount,
					details.presentModes.data());
			}

			return details;
		}

		VkPhysicalDevice& get()
		{
			return m_physicalDevice;
		}

		VkPhysicalDevice* ptr()
		{
			return &m_physicalDevice;
		}

	private:
		VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
	};
}