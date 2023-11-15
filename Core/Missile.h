#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Shader.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"

namespace PParallel
{
    class Missile
    {
    public:
        Missile(glm::vec3 position = glm::vec3(0.0f),
                glm::mat4 rotation = glm::mat4(1.0f),
                glm::vec3 color    = glm::vec3(1.0f, 0.5f, 0.0f))
            : m_position(position), m_rotation(rotation), m_color(color)
        {
            m_vertexArray.bind();
            m_vertexBuffer.bind();
            m_indexBuffer.bind();

            float vertices[] =
            {
                 0.0f, 1.0f,  0.0f, // Tip
                -0.5f, 0.0f,  0.5f, // Bottom left
                 0.5f, 0.0f,  0.5f, // Bottom right
                 0.5f, 0.0f, -0.5f, // Top right
                -0.5f, 0.0f, -0.5f  // Top left
            };
            m_vertexBuffer.bufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

            unsigned indices[] =
            {
                0, 1, 2,
                0, 2, 3,
                0, 3, 4,
                0, 4, 1,
                1, 3, 2,
                1, 3, 4
            };
            m_indexBuffer.bufferData(sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            m_vertexBuffer.unbind();
            m_vertexArray.unbind();
        }

        void translate(glm::vec3 const& translation)
        {
            m_position += translation;
        }

        void rotate(float angle, glm::vec3 const& axis)
        {
            m_rotation = glm::rotate(m_rotation, angle, axis);
        }

        void resetPosition(glm::vec3 const& position)
        {
            m_position = position;
        }

        void resetRotation(glm::mat4 const& rotation)
        {
            m_rotation = rotation;
        }

        void setColor(glm::vec3 const& color)
        {
            m_color = color;
        }

        void render(Shader& shader)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, m_position) * m_rotation;

            shader.updateUniformMat4(shader.getUniformLocation("u_model"), model);
            shader.updateUniformVec3(shader.getUniformLocation("u_color"), m_color);

            m_vertexArray.bind();
            glDrawElements(GL_TRIANGLES, 3 * 6, GL_UNSIGNED_INT, 0);
            m_vertexArray.unbind();
        }

        glm::vec3 const& getPosition() const
        {
            return m_position;
        }

        glm::mat4 const& getRotation() const
        {
            return m_rotation;
        }

        glm::vec3 const& getColor() const
        {
            return m_color;
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