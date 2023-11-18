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
            glm::mat4 rotation = glm::mat4(1.0f),
            glm::vec3 color = glm::vec3(0.5f))
            : m_position(position), m_rotation(rotation), m_color(color)
        {
            m_vertexArray.bind();
            m_vertexBuffer.bind();
            m_indexBuffer.bind();

            float vertices[] =
            {
                -1000.0f, 0.0f,  1000.0f, // Bottom left
                 1000.0f, 0.0f,  1000.0f, // Bottom right
                 1000.0f, 0.0f, -1000.0f, // Top right
                -1000.0f, 0.0f, -1000.0f  // Top left
            };
            m_vertexBuffer.bufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

            unsigned indices[] =
            {
                0, 2, 1,
                0, 2, 3
            };
            m_indexBuffer.bufferData(sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            m_vertexBuffer.unbind();
            m_vertexArray.unbind();
        }

        void render(Shader& shader)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, m_position) * m_rotation;

            shader.updateUniformMat4(shader.getUniformLocation("u_model"), model);
            shader.updateUniformVec3(shader.getUniformLocation("u_color"), m_color);

            m_vertexArray.bind();
            glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);
            m_vertexArray.unbind();
        }

    private:
        glm::vec3    m_position;
        glm::mat4    m_rotation;
        glm::vec3    m_color;
        VertexArray  m_vertexArray;
        VertexBuffer m_vertexBuffer;
        IndexBuffer  m_indexBuffer;
	};
}