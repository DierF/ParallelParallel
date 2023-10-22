#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PParallel
{
	class VertexArray
	{
	public:
		VertexArray() = default;

		~VertexArray()
		{
			glDeleteVertexArrays(1, &m_vertexArray);
		}

		void init()
		{
			glGenVertexArrays(1, &m_vertexArray);

			bind();
		}

		void bind()
		{
			glBindVertexArray(m_vertexArray);
		}

	private:
		GLuint m_vertexArray;
	};
}