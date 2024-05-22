#pragma once

#include <glad/gl.h>
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
		
		void bufferSubData(GLintptr offset, GLsizeiptr size, void const* newData)
		{
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, newData);
		}

		void unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

	private:
		GLuint m_vertexBuffer;
	};
}