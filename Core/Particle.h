#pragma once

#include <glm/glm.hpp>

namespace PParallel
{
    struct Particle
    {
        glm::vec4 color    = glm::vec4(0.0f);
        glm::vec3 position = glm::vec3(0.0f);
        float     size     = 3.0f;
    };
}