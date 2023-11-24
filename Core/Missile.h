#pragma once

#include <vector>

#include "Core/Particle.h"

namespace PParallel
{
    struct Missile
    {
        Particle p0;
        Particle p1;
        Particle p2;
        Particle p3;
        Particle p4;
    };

    struct MissileStat
    {
        glm::vec3 velocity = glm::vec3(0.0f);
    };
}