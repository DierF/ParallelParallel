#pragma once

#include <glm/glm.hpp>

namespace PParallel
{
    struct Particle
    {
        glm::vec4 color    = glm::vec4(1.0f);
        glm::vec3 position = glm::vec3(0.0f);
        float     size     = 100.0f;
    };

    struct ParticleStat
    {
        glm::vec3 velocity = glm::vec3(0.0f);
    };
}