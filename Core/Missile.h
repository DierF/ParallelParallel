#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace PParallel
{
    struct Missile
    {
    public:
        glm::vec3 m_position0    = glm::vec3( 0.0f, 1.0f,  0.0f); // Tip
        glm::vec3 m_translation0 = glm::vec3( 0.0f, 0.0f,  0.0f);
        glm::vec3 m_color0       = glm::vec3( 1.0f, 0.5f,  0.0f);

        glm::vec3 m_position1    = glm::vec3(-0.5f, 0.0f,  0.5f); // Bottom left
        glm::vec3 m_translation1 = glm::vec3( 0.0f, 0.0f,  0.0f);
        glm::vec3 m_color1       = glm::vec3( 1.0f, 0.5f,  0.0f);

        glm::vec3 m_position2    = glm::vec3( 0.5f, 0.0f,  0.5f); // Bottom right
        glm::vec3 m_translation2 = glm::vec3( 0.0f, 0.0f,  0.0f);
        glm::vec3 m_color2       = glm::vec3( 1.0f, 0.5f,  0.0f);

        glm::vec3 m_position3    = glm::vec3( 0.5f, 0.0f, -0.5f); // Top right
        glm::vec3 m_translation3 = glm::vec3( 0.0f, 0.0f,  0.0f);
        glm::vec3 m_color3       = glm::vec3( 1.0f, 0.5f,  0.0f);

        glm::vec3 m_position4    = glm::vec3(-0.5f, 0.0f, -0.5f); // Top left
        glm::vec3 m_translation4 = glm::vec3( 0.0f, 0.0f,  0.0f);
        glm::vec3 m_color4       = glm::vec3( 1.0f, 0.5f,  0.0f);
    };
}