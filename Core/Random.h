#pragma once

#include <random>

#include <glm/glm.hpp>

namespace PParallel
{
	class Random
	{
	public:
		Random(int int_max = 0, float float_max = 100.0f)
			: m_rng(0),
			  m_int_dist(0, int_max),
			  m_float_dist(-float_max, float_max)
		{
		}

		~Random() = default;

		int genInt()
		{
			return m_int_dist(m_rng);
		}
		float genFloat()
		{
			return m_float_dist(m_rng);
		}

		glm::vec3 genVec3(float height = 0.0f)
		{
			return glm::vec3(genFloat(), height, genFloat());
		}

	private:
		std::mt19937                          m_rng;
		std::uniform_int_distribution<int>    m_int_dist;
		std::uniform_real_distribution<float> m_float_dist;
	};
}
