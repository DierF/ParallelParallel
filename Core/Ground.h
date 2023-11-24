#pragma once

#include <glm/glm.hpp>

#include "Render/Shader.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"

namespace PParallel
{
	class Ground
	{
    public:
        Ground(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 color = glm::vec3(0.5f))
        {
            m_vertexArray.bind();
            m_vertexBuffer.bind();
            m_indexBuffer.bind();

            float vertices[] =
            {
                   0.5f, 0.5f,    0.5f, 1.0f,  // Bottom left
                -100.0f, 0.0f,  100.0f,
                   1.0f,
                   0.5f, 0.5f,    0.5f, 1.0f,  // Bottom right
                 100.0f, 0.0f,  100.0f,
                   1.0f,
                   0.5f, 0.5f,    0.5f, 1.0f,  // Top right
                 100.0f, 0.0f, -100.0f,
                   1.0f,
                   0.5f, 0.5f,    0.5f, 1.0f,  // Top left
                -100.0f, 0.0f, -100.0f,
                   1.0f,
            };
            m_vertexBuffer.bufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

            unsigned indices[] =
            {
                0, 2, 1,
                0, 2, 3
            };
            m_indexBuffer.bufferData(sizeof(indices), indices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(7 * sizeof(float)));

            m_vertexBuffer.unbind();
            m_vertexArray.unbind();
        }

        void render()
        {
            m_vertexArray.bind();
            glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);
            m_vertexArray.unbind();
        }

    private:
        VertexArray  m_vertexArray;
        VertexBuffer m_vertexBuffer;
        IndexBuffer  m_indexBuffer;
	};
}