#pragma once

#include <cstdint>
#include <vector>
#include <set>
#include <string>
#include <limits>
#include <algorithm>
#include <filesystem>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Render/QueueFamilyIndices.h"
#include "Render/SwapChainSupportDetails.h"
#include "Render/Instance.h"
#include "Render/PhysicalDevice.h"
#include "Render/Device.h"
#include "Render/Surface.h"
#include "Render/CommandPool.h"
#include "Render/CommandBuffer.h"
#include "FileReader.h"

namespace PParallel
{
	class Application
	{
	public:
		static uint32_t const WIDTH { 1200U };
		static uint32_t const HEIGHT{ 900U };
		
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
			m_instance.createInstance();
			m_surface.createSurface(m_instance.get(), m_window);
			m_physicalDevice.pickPhysicalDevice(m_deviceExtensions, m_instance.get(), m_surface.get());
			m_device.createLogicalDevice(m_deviceExtensions, m_surface.get(), m_physicalDevice.get(),
										 m_graphicsQueue, m_presentQueue);
			createSwapChain();
			createImageViews();
			createRenderPass();
			createGraphicsPipeline();
			createFramebuffers();
			m_commandPool.createCommandPool(m_surface.get(), m_physicalDevice.get(), m_device.get());
			m_commandBuffer.createCommandBuffer(m_device.get(), m_commandPool.get());
			createSyncObjects();
		}

		void mainLoop()
		{
			while (not glfwWindowShouldClose(m_window))
			{
				glfwPollEvents();
				drawFrame();
			}

			vkDeviceWaitIdle(m_device.get());
		}

		void cleanup()
		{
			vkDestroySemaphore(m_device.get(), m_imageAvailableSemaphore, nullptr);
			vkDestroySemaphore(m_device.get(), m_renderFinishedSemaphore, nullptr);
			vkDestroyFence(m_device.get(), m_inFlightFence, nullptr);
			vkDestroyCommandPool(m_device.get(), m_commandPool.get(), nullptr);
			for (auto framebuffer : m_swapChainFramebuffers)
			{
				vkDestroyFramebuffer(m_device.get(), framebuffer, nullptr);
			}
			vkDestroyPipeline(m_device.get(), m_graphicsPipeline, nullptr);
			vkDestroyPipelineLayout(m_device.get(), m_pipelineLayout, nullptr);
			vkDestroyRenderPass(m_device.get(), m_renderPass, nullptr);
			for (auto imageView : m_swapChainImageViews)
			{
				vkDestroyImageView(m_device.get(), imageView, nullptr);
			}
			vkDestroySwapchainKHR(m_device.get(), m_swapChain, nullptr);
			vkDestroyDevice(m_device.get(), nullptr);
			vkDestroySurfaceKHR(m_instance.get(), m_surface.get(), nullptr);
			vkDestroyInstance(m_instance.get(), nullptr);
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& availableFormats)
		{
			for (auto const& availableFormat : availableFormats)
			{
				if (availableFormat.format     == VK_FORMAT_B8G8R8A8_SRGB and
					availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					return availableFormat;
				}
			}
			return availableFormats.front();
		}

		VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> const& availablePresentModes)
		{
			for (auto const& availablePresentMode : availablePresentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return availablePresentMode;
				}
			}
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR const& capabilities)
		{
			if (capabilities.currentExtent.width not_eq std::numeric_limits<uint32_t>::max())
			{
				return capabilities.currentExtent;
			}
			else
			{
				int width, height;
				glfwGetFramebufferSize(m_window, &width, &height);
				VkExtent2D actualExtent{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
												capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height,capabilities.minImageExtent.height,
												 capabilities.maxImageExtent.height);

				return actualExtent;
			}
		}

		void createSwapChain()
		{
			SwapChainSupportDetails swapChainSupport{ PhysicalDevice::querySwapChainSupport(m_surface.get(),
													  m_physicalDevice.get())};
			VkSurfaceFormatKHR      surfaceFormat   { chooseSwapSurfaceFormat(swapChainSupport.formats) };
			VkPresentModeKHR        presentMode     { chooseSwapPresentMode(swapChainSupport.presentModes) };
			VkExtent2D              extent          { chooseSwapExtent(swapChainSupport.capabilities) };
			
			uint32_t imageCount{ swapChainSupport.capabilities.minImageCount + 1U };
			if (swapChainSupport.capabilities.maxImageCount > 0U and
				imageCount > swapChainSupport.capabilities.maxImageCount)
			{
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface          = m_surface.get();
			createInfo.minImageCount    = imageCount;
			createInfo.imageFormat      = surfaceFormat.format;
			createInfo.imageColorSpace  = surfaceFormat.colorSpace;
			createInfo.imageExtent      = extent;
			createInfo.imageArrayLayers = 1U;
			createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices indices              { findQueueFamilies(m_surface.get(), m_physicalDevice.get())};
			uint32_t           queueFamilyIndices[2]{ indices.graphicsFamily.value(), indices.presentFamily.value() };

			if (indices.graphicsFamily not_eq indices.presentFamily)
			{
				createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2U;
				createInfo.pQueueFamilyIndices   = queueFamilyIndices;
			}
			else
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			}

			createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode    = presentMode;
			createInfo.clipped        = VK_TRUE;
			createInfo.oldSwapchain   = VK_NULL_HANDLE;

			if (vkCreateSwapchainKHR(m_device.get(), &createInfo, nullptr, &m_swapChain) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create swap chain!");
			}

			vkGetSwapchainImagesKHR(m_device.get(), m_swapChain, &imageCount, nullptr);
			m_swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(m_device.get(), m_swapChain, &imageCount, m_swapChainImages.data());

			m_swapChainImageFormat = surfaceFormat.format;
			m_swapChainExtent      = extent;
		}

		void createImageViews()
		{
			m_swapChainImageViews.resize(m_swapChainImages.size());
			for (size_t idx{ 0ULL }; idx < m_swapChainImages.size(); ++idx)
			{
				VkImageViewCreateInfo createInfo{};
				createInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image    = m_swapChainImages[idx];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format   = m_swapChainImageFormat;

				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel   = 0U;
				createInfo.subresourceRange.levelCount     = 1U;
				createInfo.subresourceRange.baseArrayLayer = 0U;
				createInfo.subresourceRange.layerCount     = 1U;

				if (vkCreateImageView(m_device.get(), &createInfo, nullptr, &m_swapChainImageViews[idx]) not_eq VK_SUCCESS)
				{
					throw std::runtime_error("failed to create image views!");
				}
			}
		}

		void createGraphicsPipeline()
		{
			std::vector<char> vertShaderCode{ readFile("../../Application/vert.spv") };
			std::vector<char> fragShaderCode{ readFile("../../Application/frag.spv") };

			VkShaderModule vertShaderModule{ createShaderModule(vertShaderCode) };
			VkShaderModule fragShaderModule{ createShaderModule(fragShaderCode) };

			VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
			vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName  = "main";

			VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
			fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName  = "main";

			VkPipelineShaderStageCreateInfo shaderStages[2]{ vertShaderStageInfo, fragShaderStageInfo };

			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
			vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount   = 0U;
			vertexInputInfo.pVertexBindingDescriptions      = nullptr;
			vertexInputInfo.vertexAttributeDescriptionCount = 0U;
			vertexInputInfo.pVertexAttributeDescriptions    = nullptr;

			VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
			inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport{};
			viewport.x        = 0.0f;
			viewport.y        = 0.0f;
			viewport.width    = (float)m_swapChainExtent.width;
			viewport.height   = (float)m_swapChainExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = m_swapChainExtent;

			std::vector<VkDynamicState> dynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

			VkPipelineDynamicStateCreateInfo dynamicState{};
			dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
			dynamicState.pDynamicStates    = dynamicStates.data();

			VkPipelineViewportStateCreateInfo viewportState{};
			viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1U;
			viewportState.scissorCount  = 1U;

			VkPipelineRasterizationStateCreateInfo rasterizer{};
			rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable        = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth               = 1.0f;
			rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable         = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f;
			rasterizer.depthBiasClamp          = 0.0f;
			rasterizer.depthBiasSlopeFactor    = 0.0f;

			VkPipelineMultisampleStateCreateInfo multisampling{};
			multisampling.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable   = VK_FALSE;
			multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
			multisampling.minSampleShading      = 1.0f;
			multisampling.pSampleMask           = nullptr;
			multisampling.alphaToCoverageEnable = VK_FALSE;
			multisampling.alphaToOneEnable      = VK_FALSE;

			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			colorBlendAttachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT |
												       VK_COLOR_COMPONENT_G_BIT |
												       VK_COLOR_COMPONENT_B_BIT |
												       VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable         = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;

			VkPipelineColorBlendStateCreateInfo colorBlending{};
			colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable     = VK_FALSE;
			colorBlending.logicOp           = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount   = 1U;
			colorBlending.pAttachments      = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount         = 0U;
			pipelineLayoutInfo.pSetLayouts            = nullptr;
			pipelineLayoutInfo.pushConstantRangeCount = 0U;
			pipelineLayoutInfo.pPushConstantRanges    = nullptr;
			
			if (vkCreatePipelineLayout(m_device.get(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create pipeline layout!");
			}

			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount          = 2U;
			pipelineInfo.pStages             = shaderStages;
			pipelineInfo.pVertexInputState   = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState      = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState   = &multisampling;
			pipelineInfo.pDepthStencilState  = nullptr;
			pipelineInfo.pColorBlendState    = &colorBlending;
			pipelineInfo.pDynamicState       = &dynamicState;
			pipelineInfo.layout              = m_pipelineLayout;
			pipelineInfo.renderPass          = m_renderPass;
			pipelineInfo.subpass             = 0U;
			pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;
			pipelineInfo.basePipelineIndex   = -1;

			if (vkCreateGraphicsPipelines(m_device.get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline)
				not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create graphics pipeline!");
			}

			vkDestroyShaderModule(m_device.get(), fragShaderModule, nullptr);
			vkDestroyShaderModule(m_device.get(), vertShaderModule, nullptr);
		}

		VkShaderModule createShaderModule(std::vector<char> const& bytecode)
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = bytecode.size();
			createInfo.pCode    = reinterpret_cast<const uint32_t*>(bytecode.data());

			VkShaderModule shaderModule;
			if (vkCreateShaderModule(m_device.get(), &createInfo, nullptr, &shaderModule) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create shader module!");
			}
			return shaderModule;
		}

		void createRenderPass()
		{
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format         = m_swapChainImageFormat;
			colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			
			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0U;
			colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1U;
			subpass.pColorAttachments    = &colorAttachmentRef;

			VkSubpassDependency dependency{};
			dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass    = 0U;
			dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1U;
			renderPassInfo.pAttachments    = &colorAttachment;
			renderPassInfo.subpassCount    = 1U;
			renderPassInfo.pSubpasses      = &subpass;
			renderPassInfo.dependencyCount = 1U;
			renderPassInfo.pDependencies   = &dependency;
			
			if (vkCreateRenderPass(m_device.get(), &renderPassInfo, nullptr, &m_renderPass) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create render pass!");
			}
		}

		void createFramebuffers()
		{
			m_swapChainFramebuffers.resize(m_swapChainImageViews.size());
			for (size_t idx{ 0 }; idx < m_swapChainImageViews.size(); ++idx)
			{
				VkImageView attachments[1]{ m_swapChainImageViews[idx] };

				VkFramebufferCreateInfo framebufferInfo{};
				framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass      = m_renderPass;
				framebufferInfo.attachmentCount = 1U;
				framebufferInfo.pAttachments    = attachments;
				framebufferInfo.width           = m_swapChainExtent.width;
				framebufferInfo.height          = m_swapChainExtent.height;
				framebufferInfo.layers          = 1U;

				if (vkCreateFramebuffer(m_device.get(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[idx])
					not_eq VK_SUCCESS)
				{
					throw std::runtime_error("failed to create framebuffer!");
				}
			}
		}

		void createSyncObjects()
		{
			VkSemaphoreCreateInfo semaphoreInfo{};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			if (vkCreateSemaphore(m_device.get(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) not_eq VK_SUCCESS or
				vkCreateSemaphore(m_device.get(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) not_eq VK_SUCCESS or
				vkCreateFence(m_device.get(), &fenceInfo, nullptr, &m_inFlightFence) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to create semaphores and fence!");
			}
		}

		void drawFrame()
		{
			vkWaitForFences(m_device.get(), 1U, &m_inFlightFence, VK_TRUE, UINT64_MAX);

			uint32_t imageIndex;
			vkAcquireNextImageKHR(m_device.get(), m_swapChain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

			vkResetCommandBuffer(m_commandBuffer.get(), 0);
			m_commandBuffer.recordCommandBuffer(m_renderPass,
												m_swapChainFramebuffers,
												m_swapChainExtent,
												m_graphicsPipeline,
												imageIndex);

			VkSemaphore          waitSemaphores[1]  { m_imageAvailableSemaphore };
			VkSemaphore          signalSemaphores[1]{ m_renderFinishedSemaphore };
			VkPipelineStageFlags waitStages[1]      { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			VkSubmitInfo submitInfo{};
			submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.waitSemaphoreCount   = 1U;
			submitInfo.pWaitSemaphores      = waitSemaphores;
			submitInfo.pWaitDstStageMask    = waitStages;
			submitInfo.commandBufferCount   = 1U;
			submitInfo.pCommandBuffers      = m_commandBuffer.ptr();
			submitInfo.signalSemaphoreCount = 1U;
			submitInfo.pSignalSemaphores    = signalSemaphores;

			if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFence) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to submit draw command buffer!");
			}

			VkSwapchainKHR swapChains[1]{ m_swapChain };

			VkPresentInfoKHR presentInfo{};
			presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1U;
			presentInfo.pWaitSemaphores    = signalSemaphores;
			presentInfo.swapchainCount     = 1U;
			presentInfo.pSwapchains        = swapChains;
			presentInfo.pImageIndices      = &imageIndex;
			presentInfo.pResults           = nullptr;

			vkQueuePresentKHR(m_presentQueue, &presentInfo);
		}

	private:
		std::vector<char const*> const m_deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	private:
		GLFWwindow*                m_window;
		Instance				   m_instance;
		PhysicalDevice			   m_physicalDevice;
		Device					   m_device;
		Surface					   m_surface;
		VkQueue                    m_graphicsQueue;
		VkQueue                    m_presentQueue;
		VkSwapchainKHR             m_swapChain;
		std::vector<VkImage>       m_swapChainImages;
		VkFormat                   m_swapChainImageFormat;
		VkExtent2D                 m_swapChainExtent;
		std::vector<VkImageView>   m_swapChainImageViews;
		VkRenderPass               m_renderPass;
		VkPipelineLayout           m_pipelineLayout;
		VkPipeline                 m_graphicsPipeline;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;
		CommandPool                m_commandPool;
		CommandBuffer              m_commandBuffer;
		VkSemaphore                m_imageAvailableSemaphore;
		VkSemaphore                m_renderFinishedSemaphore;
		VkFence                    m_inFlightFence;
	};
}
