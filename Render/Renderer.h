#pragma once

#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Application/Time.h"
#include "Core/Missile.h"
#include "Core/CameraController.h"
#include "Render/Shader.h"

namespace PParallel
{
	class Renderer
	{
	public:
		Renderer() = default;

		~Renderer() = default;

		void init()
		{
			if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				throw std::runtime_error("failed to initialize GLAD");
			}

			m_shader.init();
		}

		void clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void renderMissile(Missile& missile)
		{
			missile.render(m_shader);
		}

		void renderCamera(CameraController& cameraController)
		{
			cameraController.render(m_shader);
		}

	private:
		Shader m_shader;
	};
}