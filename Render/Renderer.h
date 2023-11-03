#pragma once

#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Application/Time.h"
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

		void draw()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		Shader& refShader()
		{
			return m_shader;
		}

	private:
		Shader m_shader;
	};
}