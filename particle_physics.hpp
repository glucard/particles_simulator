#ifndef PARTICLE_PHYSICS_HPP
#define PARTICLE_PHYSICS_HPP

#include "particle.hpp"

/* Detect collision between two particles and apply its impact. */
int collision(Particle* particle_1, Particle* particle_2, float pad, float bounciness, bool isTrigger);

/* Calculate and apply the gravity force between two particles. */
void gravity(Particle* particle_1, Particle* particle_2, float g);

#endif
