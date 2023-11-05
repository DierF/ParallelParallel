#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PParallel
{
	class VertexBuffer
	{
	public:
		VertexBuffer()
		{
			glGenBuffers(1, &m_vertexBuffer);
		}

		~VertexBuffer()
		{
			glDeleteBuffers(1, &m_vertexBuffer);
		}

		void bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		}

		void bufferData(GLsizeiptr size, void const* data, GLenum usage)
		{
			glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		}

		void unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

	private:
		GLuint m_vertexBuffer;
	};
}