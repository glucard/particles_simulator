#ifndef PARTICLE_PHYSICS_HPP
#define PARTICLE_PHYSICS_HPP

#include "particle.hpp"

int colision(Particle* particle_1, Particle* particle_2, float pad, float bounciness, bool isTrigger);
void gravity(Particle* particle_1, Particle* particle_2, float g);

#endif
