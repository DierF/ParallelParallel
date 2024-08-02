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
        Ground()
        {
            m_vertexArray.bind();
            m_vertexBuffer.bind();
            m_indexBuffer.bind();

            float vertices[] =
            {
                // Bottom left
                0.05f, 0.05f, 0.05f, 1.05f,
                -500.0f, 0.0f, 500.0f,
                0.0f, 0.0f, 0.0f,
                INFINITY,
                1.0f,
                // Bottom right
                0.05f, 0.05f, 0.05f, 1.05f,
                500.0f, 0.0f, 500.0f,
                0.0f, 0.0f, 0.0f,
                INFINITY,
                1.0f,
                // Top right
                0.05f, 0.05f, 0.05f, 1.05f,
                500.0f, 0.0f, -500.0f,
                0.0f, 0.0f, 0.0f,
                INFINITY,
                1.0f,
                // Top left
                0.05f, 0.05f, 0.05f, 1.05f,
                -500.0f, 0.0f, -500.0f,
                0.0f, 0.0f, 0.0f,
                INFINITY,
                1.0f
            };
            m_vertexBuffer.bufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

            unsigned indices[] =
            {
                0, 2, 1,
                0, 2, 3
            };
            m_indexBuffer.bufferData(sizeof(indices), indices, GL_STATIC_DRAW);

            // Based on how Particle is defined
            // color: vec4
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
            // position: vec3
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
            // velocity: vec3
            // UNUSED by shader
            // lifetime: float
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(10 * sizeof(float)));
            // size: float
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(11 * sizeof(float)));

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