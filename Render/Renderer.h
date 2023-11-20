#pragma once

#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Application/Time.h"
#include "Core/Ground.h"
#include "Core/Missile.h"
#include "Core/MissileGroup.h"
#include "Core/CameraController.h"
#include "Render/Shader.h"

namespace PParallel
{
	class Renderer
	{
	public:
		Renderer() = default;

		~Renderer() = default;

		void clearBuffer()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void renderMissiles(MissileGroup& missileGroup)
		{
			missileGroup.render();
		}

		void renderCamera(CameraController& cameraController)
		{
			cameraController.render(m_shader);
		}

		void renderGround(Ground& ground)
		{
			ground.render(m_shader);
		}

	private:
		Shader m_shader;
	};
}