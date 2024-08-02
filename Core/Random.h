#pragma once

#include <random>

#include <glm/glm.hpp>

namespace PParallel
{
	class Random
	{
	public:
		Random() = default;

		~Random() = default;

		int genInt(int min, int max)
		{
			static std::mt19937 generator(std::random_device{}());
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(generator);
		}

		float genFloat(float min, float max)
		{
			static std::mt19937 generator(std::random_device{}());
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(generator);
		}

		glm::vec4 const& genColor()
		{
			return brightColors[genInt(0, static_cast<int>(brightColors.size()) - 1)];
		}

	public:
		std::vector<glm::vec4> const brightColors
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),    // Red
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),    // Orange
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),    // Yellow
			glm::vec4(0.5f, 1.0f, 0.0f, 1.0f),    // Lime
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),    // Green
			glm::vec4(0.0f, 1.0f, 0.5f, 1.0f),    // Teal
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),    // Cyan
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),    // Sky Blue
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),    // Blue
			glm::vec4(0.5f, 0.0f, 1.0f, 1.0f),    // Purple
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),    // Magenta
			glm::vec4(1.0f, 0.0f, 0.5f, 1.0f),    // Pink
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),    // White
			glm::vec4(1.0f, 0.5f, 0.5f, 1.0f),    // Light Red
			glm::vec4(1.0f, 0.8f, 0.5f, 1.0f),    // Light Orange
			glm::vec4(1.0f, 1.0f, 0.5f, 1.0f),    // Light Yellow
			glm::vec4(0.8f, 1.0f, 0.5f, 1.0f),    // Light Lime
			glm::vec4(0.5f, 1.0f, 0.5f, 1.0f),    // Light Green
			glm::vec4(0.5f, 1.0f, 0.8f, 1.0f),    // Light Teal
			glm::vec4(0.5f, 1.0f, 1.0f, 1.0f),    // Light Cyan
		};
	};
}
