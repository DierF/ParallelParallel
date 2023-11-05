#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application/FileReader.h"

namespace PParallel
{
	class Shader
	{
	public:
		Shader()
		{
			m_shader = glCreateProgram();
			compile();
		};

		~Shader()
		{
			glDeleteProgram(m_shader);
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

			glAttachShader(m_shader, vertexShader);
			glAttachShader(m_shader, fragmentShader);
			glLinkProgram(m_shader);
			glUseProgram(m_shader);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

		void use()
		{
			glUseProgram(m_shader);
		}

		GLint getUniformLocation(std::string const& name)
		{
			return glGetUniformLocation(m_shader, name.c_str());
		}

		void updateUniformMat4(GLint location, glm::mat4 const& mat)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		}

		void updateUniformVec3(GLint location, glm::vec3 const& vec)
		{
			glUniform3f(location, vec.x, vec.y, vec.z);
		}

		GLuint get()
		{
			return m_shader;
		}

	private:
		GLuint m_shader;
	};
}