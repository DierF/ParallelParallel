#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/FileReader.h"

namespace PParallel
{
	class ShaderProgram
	{
	public:
		ShaderProgram() = default;

		~ShaderProgram()
		{
			glDeleteProgram(m_shaderProgram);
		}

		void init()
		{
			m_shaderProgram = glCreateProgram();
			compile();
		}

		void compile()
		{
			std::string vertexShaderFile = FileReader::readFile("../Render/vertex.glsl");
			std::string fragmentShaderFile = FileReader::readFile("../Render/fragment.glsl");
			char const* vertexShaderSource = vertexShaderFile.c_str();
			char const* fragmentShaderSource = fragmentShaderFile.c_str();

			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			glAttachShader(m_shaderProgram, vertexShader);
			glAttachShader(m_shaderProgram, fragmentShader);
			glLinkProgram(m_shaderProgram);
			glUseProgram(m_shaderProgram);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

		void bind()
		{
			glUseProgram(m_shaderProgram);
		}

		GLint getUniformLocation(std::string const& name)
		{
			return glGetUniformLocation(m_shaderProgram, name.c_str());
		}

		void updateUniformMat4(GLint location, glm::mat4 const& mat)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		}

		GLuint get()
		{
			return m_shaderProgram;
		}

	private:
		GLuint m_shaderProgram;
	};
}