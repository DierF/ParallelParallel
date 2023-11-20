#pragma once

#include <vector>

#include "Core/Missile.h"
#include "Core/MissileController.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"

namespace PParallel
{
	class MissileGroup
	{
	public:
		MissileGroup(std::size_t cnt)
			: m_missiles(cnt), m_indices(cnt * 3 * 6)
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();
			m_indexBuffer.bind();

			m_vertexBuffer.bufferData(cnt * sizeof(Missile), m_missiles.data(), GL_STATIC_DRAW);

			for (std::size_t i = 0ULL; i < cnt; ++i)
			{
				m_indices[i * 3 * 6 +  0] = i * 5 + 0;
				m_indices[i * 3 * 6 +  1] = i * 5 + 1;
				m_indices[i * 3 * 6 +  2] = i * 5 + 2;

				m_indices[i * 3 * 6 +  3] = i * 5 + 0;
				m_indices[i * 3 * 6 +  4] = i * 5 + 2;
				m_indices[i * 3 * 6 +  5] = i * 5 + 3;

				m_indices[i * 3 * 6 +  6] = i * 5 + 0;
				m_indices[i * 3 * 6 +  7] = i * 5 + 3;
				m_indices[i * 3 * 6 +  8] = i * 5 + 4;

				m_indices[i * 3 * 6 +  9] = i * 5 + 0;
				m_indices[i * 3 * 6 + 10] = i * 5 + 4;
				m_indices[i * 3 * 6 + 11] = i * 5 + 1;

				m_indices[i * 3 * 6 + 12] = i * 5 + 1;
				m_indices[i * 3 * 6 + 13] = i * 5 + 3;
				m_indices[i * 3 * 6 + 14] = i * 5 + 2;

				m_indices[i * 3 * 6 + 15] = i * 5 + 1;
				m_indices[i * 3 * 6 + 16] = i * 5 + 3;
				m_indices[i * 3 * 6 + 17] = i * 5 + 4;
			}
			m_indexBuffer.bufferData(m_indices.size() * sizeof(unsigned), m_indices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		};
		
		~MissileGroup() = default;

		void render()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			m_vertexBuffer.bufferData(m_missiles.size() * sizeof(Missile), m_missiles.data(), GL_STATIC_DRAW);

			glDrawElements(GL_TRIANGLES, m_missiles.size() * 3 * 6, GL_UNSIGNED_INT, 0);
			
			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		auto begin()
		{
			return m_missiles.begin();
		}

		auto end()
		{
			return m_missiles.end();
		}

		auto size()
		{
			return m_missiles.size();
		}

		Missile& operator[](std::size_t pos)
		{
			return m_missiles[pos];
		}

	private:
		std::vector<Missile>  m_missiles;
		std::vector<unsigned> m_indices;
		VertexArray           m_vertexArray;
		VertexBuffer          m_vertexBuffer;
		IndexBuffer           m_indexBuffer;
	};
}