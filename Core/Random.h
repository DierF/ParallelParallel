#pragma once

#include <random>

#include <glm/glm.hpp>

namespace PParallel
{
	class Random
	{
		

	public:
		Random()
			: m_rng(0),
			  m_float_dist(-80.0f, 80.0f)
		{
		}

		~Random() = default;

		int genInt(int min = 0, int max = 1000)
		{
			return m_rng() % (max - min) + min;
		}

		float genFloat()
		{
			return m_float_dist(m_rng);
		}

		glm::vec3 genVec3(float y = 0.0f)
		{
			return glm::vec3(genFloat(), y, genFloat());
		}

		glm::vec4 const& genColor()
		{
			return brightColors[genInt(0, brightColors.size())];
		}

	private:
		std::mt19937                          m_rng;
		std::uniform_real_distribution<float> m_float_dist;

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
