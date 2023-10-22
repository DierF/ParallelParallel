#pragma once

#include <chrono>

namespace PParallel
{
	class Timer
	{
	public:
		Timer()
		{
			reset();
		}

		void reset()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		float elapsedNanoseconds()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>
					(std::chrono::high_resolution_clock::now() - m_start).count()
					* 0.001f * 0.001f * 0.001f;
		}

		float elapsedMilliseconds()
		{
			return elapsedNanoseconds() * 1000.0f;
		}

		float elapsedSeconds()
		{
			return elapsedMilliseconds() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};
}