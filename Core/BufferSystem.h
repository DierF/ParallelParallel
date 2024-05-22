#pragma once

#include <list>
#include <stdexcept>

#include <glad/gl.h>

#include "Core/Particle.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

#define bufferSize 1'048'576

namespace PParallel
{
	class Buffer
	{
		friend class BufferSystem;

	public:
		Buffer() = delete;

		Buffer(GLenum mode)
			: m_buffer(new char[bufferSize]{}), m_bufferSize(bufferSize),
			m_size(0ULL), m_mode(mode)
		{
			if (m_mode == GL_POINTS)
			{
				m_vertexArray.bind();
				m_vertexBuffer.bind();
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(7 * sizeof(float)));
				m_vertexBuffer.unbind();
				m_vertexArray.unbind();
			}
		}
		
		~Buffer()
		{
			delete[] m_buffer;
		}

		void render()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();
			m_vertexBuffer.bufferData(m_size, m_buffer, GL_STATIC_DRAW);
			glDrawArrays(m_mode, 0, static_cast<GLsizei>(m_size));
			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

	private:
		char*        m_buffer;
		std::size_t  m_bufferSize;
		std::size_t  m_size;
		GLenum       m_mode;
		VertexArray  m_vertexArray;
		VertexBuffer m_vertexBuffer;
	};

	class BufferSystem
	{
	public:
		BufferSystem() = default;

		~BufferSystem() = default;

		void* acquire(std::size_t size, GLenum mode)
		{
			if (size > bufferSize)
			{
				throw std::runtime_error("Buffer Size not enough!");
			}
			for (auto& each : m_buffers)
			{
				if (each.m_mode not_eq mode)
				{
					continue;
				}
				std::size_t remain = each.m_bufferSize - each.m_size;
				if (remain >= size)
				{
					void* ptr = each.m_buffer + each.m_size;
					each.m_size += size;
					return ptr;
				}
			}
			m_buffers.emplace_back(mode);
			m_buffers.back().m_size += size;
			return m_buffers.back().m_buffer;
		}

		void release(void* ptr, std::size_t size)
		{
			std::memset(ptr, 0, size);
		}

		void render()
		{
			for (auto& each : m_buffers)
			{
				each.render();
			}
		}

	private:
		std::list<Buffer> m_buffers;
	};
}