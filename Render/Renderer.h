#pragma once

#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Core/FileReader.h"
#include "Core/Camera.h"
#include "Render/ShaderProgram.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexArray.h"

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

			m_shaderProgram.init();
			m_vertexBuffer.init();
			m_vertexArray.init();

			float vertices[9]
			{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glEnableVertexAttribArray(0);
		}

		void draw(GLFWwindow* window, float deltaTime)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			m_camera.update(window, deltaTime);

			m_shaderProgram.updateUniformMat4(m_shaderProgram.getUniformLocation("u_viewProjection"),
											  m_camera.getViewProjectionMatrix());
			m_shaderProgram.bind();
			m_vertexArray.bind();
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

	private:
		VertexArray   m_vertexArray;
		VertexBuffer  m_vertexBuffer;
		ShaderProgram m_shaderProgram;
		Camera		  m_camera;
	};
}