#pragma once

#include <vector>
#include <tuple>
#include <numbers>

#include <glm/glm.hpp>

namespace PParallel
{
	enum ExplosionType
	{
		NoExplosion = 0,
		SphereExplosion
	};

	class Explosion
	{
	public:
		using param_t = std::tuple<glm::vec4, glm::vec3, glm::vec3, float, std::size_t, std::size_t, ExplosionType>;
	};

	class NoExplosion : public Explosion
	{
	public:
		static std::vector<param_t> explode(glm::vec4 const& color,
			                                glm::vec3 const& position,
			                                glm::vec3 const& velocity,
			                                float            lifetime,
			                                std::size_t      tailSize,
			                                float            tailLife,
			                                ExplosionType    explosion_t)
		{
			return {};
		}
	};

	class SphereExplosion : public Explosion
	{
	public:
		static std::vector<param_t> explode(glm::vec4 const& color,
											 glm::vec3 const& position,
											 glm::vec3 const& velocity,
											 float            lifetime,
											 std::size_t      tailSize,
											 float            tailLife,
											 ExplosionType    explosion_t)
		{
			std::vector<param_t> res;

			std::size_t cnt = 50ULL;
			// golden angle in radians
			constexpr float const phi = std::numbers::pi_v<float> *2 * (std::numbers::phi_v<float> -1.0f);
			for (std::size_t i = 0ULL; i < cnt; ++i)
			{
				// y goes from 1 to - 1
				float y = 1.0f - (static_cast<float>(i) / static_cast<float>(cnt - 1ULL)) * 2.0f;
				// radius at this y height
				float radiusAtY = std::sqrt(1.0f - y * y);
				// golden angle increment
				float theta = phi * static_cast<float>(i);
				float x = std::cos(theta) * radiusAtY;
				float z = std::sin(theta) * radiusAtY;

				float explodeSpeed = 200.0f;
				res.emplace_back(color, position, glm::normalize(glm::vec3(x, y, z)) * explodeSpeed,
					lifetime, tailSize, tailLife, ExplosionType::NoExplosion);
			}

			return res;
		}
	};

	class Exploder
	{
	public:
		static auto explode(glm::vec4 const& color,
							  glm::vec3 const& position,
							  glm::vec3 const& velocity,
							  float            lifetime,
							  std::size_t      tailSize,
							  float            tailLife,
							  ExplosionType    explosion_t)
		{
			switch (explosion_t)
			{
			default:
			case ExplosionType::NoExplosion:
				return NoExplosion::explode(color, position, velocity, lifetime, tailSize, tailLife, explosion_t);
			case ExplosionType::SphereExplosion:
				return SphereExplosion::explode(color, position, velocity, lifetime, tailSize, tailLife, explosion_t);
			}
		}
	};
}