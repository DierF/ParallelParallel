#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Core/CameraController.h"
#include "Core/MissileGroup.h"
#include "Core/MissileController.h"
#include "Core/ParticleController.h"
#include "Core/Random.h"
#include "Core/Ground.h"
#include "Render/Renderer.h"

namespace PParallel
{
	class Scene
	{
	public:
		Scene(std::size_t cnt = 1ULL)
			: m_missileGroup(cnt),
			  m_paused(true)
		{
			for (std::size_t i = 0ULL; i < cnt; ++i)
			{
			}
		};

		~Scene() = default;

		void update(float deltaTime)
		{
			if (Window::get().isSpaceKeyPressed())
			{
				m_paused = not m_paused;
			}
			tickCamera(deltaTime);
			if (not m_paused)
			{
				tickObjects(deltaTime);
			}
			render();
		}

		void tickCamera(float deltaTime)
		{
			m_cameraController.update(deltaTime);
		}

		void tickObjects(float deltaTime)
		{
			tickAttackers(deltaTime);
		}

		void tickAttackers(float deltaTime)
		{
			//for (std::size_t i = 0ULL; i < m_missileGroup.size(); ++i)
			//{
			//
			//}
		}
		
		void render()
		{
			m_renderer.clearBuffer();
			m_renderer.renderCamera(m_cameraController);
			m_renderer.renderMissiles(m_missileGroup);
			m_renderer.renderGround(m_ground);
		}

	private:
		CameraController       m_cameraController;
		Renderer               m_renderer;
		Random                 m_random;
		Ground                 m_ground;
		MissileGroup	       m_missileGroup;
		bool                   m_paused;
	};
}