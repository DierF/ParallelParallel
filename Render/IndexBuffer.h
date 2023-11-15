#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PParallel
{
	class IndexBuffer
	{
	public:
		IndexBuffer()
		{
			glGenBuffers(1, &m_indexBuffer);
		}

		~IndexBuffer()
		{
			glDeleteBuffers(1, &m_indexBuffer);
		}

		void bind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		}

		void bufferData(GLsizeiptr size, void const* data, GLenum usage)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		}

	private:
		GLuint m_indexBuffer;
	};
}