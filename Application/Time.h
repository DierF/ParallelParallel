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
			return static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - m_start).count());
		}

		float elapsedMilliseconds()
		{
			return elapsedNanoseconds() * 0.001f * 0.001f;
		}

		float elapsedSeconds()
		{
			return elapsedMilliseconds() * 0.001f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};
}