#include "particle_physics.hpp"
#include "mymath.hpp"
#include <math.h>

float calculateDistance(Particle* particle_1, Particle* particle_2) {
	float x = particle_1->position.getX() - particle_2->position.getX();
	float y = particle_1->position.getY() - particle_2->position.getY();
	return norm(x, y);
}

void bounce(Particle* particle_1, Particle* particle_2, float bounciness) {
	float px = particle_2->position.getX() - particle_1->position.getX();
	float py = particle_2->position.getY() - particle_1->position.getY();
	float distance = norm(px, py);
	float cossine = py / distance;
	float sine = px / distance;
	float vx = particle_2->velocity.getX() - particle_1->velocity.getX();
	float vy = particle_2->velocity.getY() - particle_1->velocity.getY();
	float v = norm(vx, vy);
	particle_1->velocity.accelerateX(-v * sine * bounciness);
	particle_1->velocity.accelerateY(-v * cossine * bounciness);
	particle_2->velocity.accelerateX(v * sine * bounciness);
	particle_2->velocity.accelerateY(v * cossine * bounciness);
}

float adjustAxis(float axis, float distance, float new_distance) {
	return (axis / distance) * new_distance;
}

void padding(Particle* particle_1, Particle* particle_2, float distance, float radius, float pad) {
	float px = particle_2->position.getX() - particle_1->position.getX();
	float py = particle_2->position.getY() - particle_1->position.getY();
	float new_distance = radius + pad;
	particle_1->position.moveX(adjustAxis(-px / 2, distance, new_distance - distance));
	particle_1->position.moveY(adjustAxis(-py / 2, distance, new_distance - distance));
	particle_2->position.moveX(adjustAxis(px / 2, distance, new_distance - distance));
	particle_2->position.moveY(adjustAxis(py / 2, distance, new_distance - distance));
}

int colision(Particle* particle_1, Particle* particle_2, float pad, float bounciness, bool isTrigger) {
	float distance = calculateDistance(particle_1, particle_2);
	float radius = particle_1->getRadius() + particle_2->getRadius();
	if (distance <= radius + pad) {
		if (not(isTrigger)) {
			padding(particle_1, particle_2, distance, radius, pad);
			bounce(particle_1, particle_2, bounciness);
		}
		return 1;
	}
	return 0;
}

void gravity(Particle* particle_1, Particle* particle_2, float g) {
	//if (not(colision(particle_1, particle_2, 0, 0, true))) {
	float px = particle_2->position.getX() - particle_1->position.getX();
	float py = particle_2->position.getY() - particle_1->position.getY();
	float distance = norm(px, py);
	float p1_mass = particle_1->getMass();
	float p2_mass = particle_2->getMass();
	float gravity_force = p1_mass * p2_mass * g / pow(distance, 2);
	float const_acceleration = gravity_force / (p1_mass + p2_mass);
	float p1_acceleration = p2_mass * const_acceleration;
	float p2_acceleration = p1_mass * const_acceleration;
	float p1_acel_x = adjustAxis(px, distance, p1_acceleration);
	float p1_acel_y = adjustAxis(py, distance, p1_acceleration);
	float p2_acel_x = adjustAxis(-px, distance, p2_acceleration);
	float p2_acel_y = adjustAxis(-py, distance, p2_acceleration);
	particle_1->velocity.accelerateX(p1_acel_x);
	particle_1->velocity.accelerateY(p1_acel_y);
	particle_2->velocity.accelerateX(p2_acel_x);
	particle_2->velocity.accelerateY(p2_acel_y);
	//}
}