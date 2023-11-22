#pragma once

#include <glm/glm.hpp>

namespace PParallel
{
    struct Particle
    {
        glm::vec3 m_position    = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_color       = glm::vec3(1.0f, 1.0f, 1.0f);
    };
}