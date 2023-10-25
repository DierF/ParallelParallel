#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Render/Shader.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

namespace PParallel
{
    class Missile
    {
    public:
        Missile(glm::vec3 position = glm::vec3(0.0f),
                glm::mat4 rotation = glm::mat4(1.0f))
            : m_position(position), m_rotation(rotation)
        {
        }

        void init()
        {
            m_vertexBuffer.init();
            m_vertexArray.init();

            m_vertexArray.bind();
            m_vertexBuffer.bind();

            // Calculate the vertices of the right triangle based on m_position and m_rotation
            glm::vec3 v1 = glm::vec3(-0.5f, -0.5f, 0.0f); // Bottom-left vertex
            glm::vec3 v2 = glm::vec3(0.5f, -0.5f, 0.0f);  // Bottom-right vertex
            glm::vec3 v3 = glm::vec3(-0.5f, 0.5f, 0.0f);  // Top-left vertex

            // Apply the transformation matrix m_rotation to the vertices
            v1 = glm::vec3(m_rotation * glm::vec4(v1, 1.0f));
            v2 = glm::vec3(m_rotation * glm::vec4(v2, 1.0f));
            v3 = glm::vec3(m_rotation * glm::vec4(v3, 1.0f));

            // Translate the vertices to the object's position
            v1 += m_position;
            v2 += m_position;
            v3 += m_position;

            // Store the vertices in a vector
            std::vector<float> vertices =
            {
                v1.x, v1.y, v1.z,
                v2.x, v2.y, v2.z,
                v3.x, v3.y, v3.z
            };

            m_vertexBuffer.bufferData(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            m_vertexArray.unbind();
            m_vertexBuffer.unbind();
        }

        void translate(glm::vec3 const& translation)
        {
            m_position += translation;
        }

        void rotate(float angle, glm::vec3 const& axis)
        {
            m_rotation = glm::rotate(m_rotation, angle, axis);
        }

        void render(Shader& shader)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_position);
            model = model * m_rotation;

            glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

            shader.updateUniformMat4(shader.getUniformLocation("u_model"), model);
            shader.updateUniformVec3(shader.getUniformLocation("u_color"), color);

            m_vertexArray.bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

    private:
        glm::vec3 m_position;
        glm::mat4 m_rotation;
        VertexArray m_vertexArray;
        VertexBuffer m_vertexBuffer;
    };
}