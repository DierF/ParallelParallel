#pragma once

#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Core/Camera.h"
#include "Core/Missile.h"
#include "Render/Shader.h"

namespace PParallel
{
	class Renderer
	{
	public:
		Renderer() = default;

		~Renderer()
		{
		}

		void init()
		{
			if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				throw std::runtime_error("failed to initialize GLAD");
			}

			m_shader.init();

			m_missile.init();
		}

		void draw(float deltaTime)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			m_camera.update(deltaTime);

			m_shader.updateUniformMat4(m_shader.getUniformLocation("u_viewProjection"),
											  m_camera.getViewProjectionMatrix());

			m_missile.rotate(0.005f * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
			m_missile.render(m_shader);
		}

	private:
		Shader		  m_shader;
		Camera		  m_camera;
		Missile       m_missile;
	};
}