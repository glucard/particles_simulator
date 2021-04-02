#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "particle_physics.hpp"
#include "mymath.hpp"
#include <windows.h>

#define N_PARTICLES 50
#define PARTICLE_MASS 200
#define PARTICLE_RADIUS 5
#define BOUNCINESS 0.3
#define PAD 0
#define X_LIMIT 1366
#define Y_LIMIT 768
#define VELOCITY_LIMIT 2000
#define TICKET 600

float randfloat(int max_n) {
    return float(rand() % (max_n));
}

Particle** initiateParticles(int n_particles, float particle_mass, float particle_radius, float limit_x, float limit_y, float velocity_limit) {
    Particle** particles = (Particle**)malloc(n_particles * sizeof(Particle*));
    float position_x, position_y, velocity_x, velocity_y;
    for (int i = 0; i < n_particles; i++) {
        particles[i] = (Particle*)malloc(sizeof(Particle));
        position_x = randfloat(limit_x);
        position_y = randfloat(limit_y);
        velocity_x = randfloat(velocity_limit * 2) - velocity_limit;
        velocity_y = randfloat(velocity_limit * 2) - velocity_limit;
        *particles[i] = Particle(position_x, position_y, velocity_x, velocity_y, particle_mass, particle_radius);
    }
    return particles;
}

void setShapeColorVelocity(sf::Shape* particle_shape, Particle* particle, float velocity_limit) {
    float red_scale, blue_scale;
    float velocity_color_scale;
    velocity_color_scale = 255 * particle->velocity.getVelocityModuleSum() / (velocity_limit * 2);
    red_scale = velocity_color_scale;
    blue_scale = 255 - velocity_color_scale;
    particle_shape->setFillColor(sf::Color(red_scale, 0, blue_scale));
}

void update(sf::RenderWindow* window, Particle** particles, float n_particles, float pad, float bounciness, float velocity_limit, float x_limit, float y_limit, float ticket) {
    sf::CircleShape particle_shape;
    
    for (int i = 0; i < n_particles; i++) {
        for (int j = i + 1; j < n_particles; j++) {
            colision(particles[i], particles[j], pad, bounciness, false);
            gravity(particles[i], particles[j], 0.1);
        }
        particles[i]->update(ticket, x_limit, y_limit, velocity_limit);
        setShapeColorVelocity(&particle_shape, particles[i], velocity_limit);
        particle_shape.setRadius(particles[i]->getRadius());
        particle_shape.setPosition(sf::Vector2f(particles[i]->position.getX(), particles[i]->position.getY()));
        window->draw(particle_shape);
    }
}

int main()
{
    int n_particles = N_PARTICLES;
    float particle_mass = PARTICLE_MASS;
    float particles_radius = PARTICLE_RADIUS;
    float bounciness = BOUNCINESS;
    float pad = PAD;
    float velocity_limit = VELOCITY_LIMIT;
    float ticket = TICKET;
    srand((unsigned int)time(NULL));
    Particle** particles;
    sf::RenderWindow window(sf::VideoMode(X_LIMIT, Y_LIMIT), "SFML works!");
    particles = initiateParticles(N_PARTICLES, PARTICLE_MASS, PARTICLE_RADIUS, X_LIMIT, Y_LIMIT, VELOCITY_LIMIT);
    while (window.isOpen())
    {
        Sleep(1000 / TICKET);
        window.clear();
        update(&window, particles, N_PARTICLES, PAD, BOUNCINESS, VELOCITY_LIMIT, X_LIMIT, Y_LIMIT, TICKET);
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.display();
    }
    for (int i = 0; i < N_PARTICLES; i++) {
        free(particles[i]);
    }
    free(particles);
    return 0;
}