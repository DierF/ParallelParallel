#pragma once

#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PParallel
{
	class CommandBuffer
	{
	public:
		void createCommandBuffer(VkDevice& device, VkCommandPool& commandPool)
		{
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1U;

			if (vkAllocateCommandBuffers(device, &allocInfo, &m_commandBuffer) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to allocate command buffers!");
			}
		}

		void recordCommandBuffer(VkRenderPass& renderPass,
								 std::vector<VkFramebuffer> const& swapChainFramebuffers,
								 VkExtent2D& swapChainExtent,
								 VkPipeline& graphicsPipeline,
								 uint32_t& imageIndex)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChainExtent;

			VkClearValue clearColor{ { { 0.0f, 0.0f, 0.0f, 1.0f } } };
			renderPassInfo.clearValueCount = 1U;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(swapChainExtent.width);
			viewport.height = static_cast<float>(swapChainExtent.height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			vkCmdSetViewport(m_commandBuffer, 0U, 1U, &viewport);

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = swapChainExtent;

			vkCmdSetScissor(m_commandBuffer, 0U, 1U, &scissor);

			vkCmdDraw(m_commandBuffer, 3U, 1U, 0U, 0U);

			vkCmdEndRenderPass(m_commandBuffer);

			if (vkEndCommandBuffer(m_commandBuffer) not_eq VK_SUCCESS)
			{
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		VkCommandBuffer& get()
		{
			return m_commandBuffer;
		}

		VkCommandBuffer* ptr()
		{
			return &m_commandBuffer;
		}

	private:
		VkCommandBuffer m_commandBuffer;
	};
}