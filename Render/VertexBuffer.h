#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PParallel
{
	class VertexBuffer
	{
	public:
		VertexBuffer() = default;

		~VertexBuffer()
		{
			glDeleteBuffers(1, &m_vertexBuffer);
		}

		void init()
		{
			glGenBuffers(1, &m_vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		}

	private:
		GLuint m_vertexBuffer;
	};
}