#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "particle_physics.hpp"
#include "mymath.hpp"
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>

#define N_PARTICLES 50
#define PARTICLE_MASS 200
#define PARTICLE_RADIUS 5
#define GRAVITY_CONST 0.1
#define BOUNCINESS 0.3
#define PAD 0
#define X_LIMIT 1366
#define Y_LIMIT 768
#define VELOCITY_LIMIT 2000
#define TICKET 600


#define INT_TYPE 0
#define FLOAT_TYPE 1
struct Setting
{
    const char* name;
    void* value;
    void* update_value; // how many the value will change.
    int type;
    
};

/* return a rand float less than max_n. */
float randfloat(int max_n) {
    return float(rand() % (max_n));
}

/* return a list of n_particles. */
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

/* regenerate a list of particles. */
Particle** regenerateParticles(Particle** particles, int n_particles, int old_n_particles, float particle_mass, float particle_radius, float limit_x, float limit_y, float velocity_limit) {
    
    float position_x, position_y, velocity_x, velocity_y;

    // if have less n_particles than before, free it. 
    for (int i = n_particles; i < old_n_particles; i++) {
        free(particles[i]);
    }

    // realloc the list.
    particles = (Particle**)realloc(particles, n_particles * sizeof(Particle*));

    // if have more n_particles than before, create it.
    for (int i = old_n_particles; i < n_particles; i++) {
        particles[i] = (Particle*)malloc(sizeof(Particle));
        position_x = randfloat(limit_x);
        position_y = randfloat(limit_y);
        velocity_x = randfloat(velocity_limit * 2) - velocity_limit;
        velocity_y = randfloat(velocity_limit * 2) - velocity_limit;
        *particles[i] = Particle(position_x, position_y, velocity_x, velocity_y, particle_mass, particle_radius);
    }

    // return the new list of particles.
    return particles;
}

/* Colorize the sf::shape of a particle based of their velocity. */
void setShapeColorVelocity(sf::Shape* particle_shape, Particle* particle, float velocity_limit) {
    float red_scale, blue_scale;
    float velocity_color_scale;
    velocity_color_scale = 255 * particle->velocity.getVelocityModuleSum() / (velocity_limit * 2);
    red_scale = velocity_color_scale;
    blue_scale = 255 - velocity_color_scale;
    particle_shape->setFillColor(sf::Color(red_scale, 0, blue_scale));
}

/* Apply physics in the particles and draw it. */
void update(sf::RenderWindow* window, Particle** particles, float n_particles, float pad, float bounciness, float velocity_limit, float x_limit, float y_limit, float gravity_const, float ticket) {
    sf::CircleShape particle_shape;
    
    for (int i = 0; i < n_particles; i++) {
        // For each particle in particles
        for (int j = i + 1; j < n_particles; j++) {
            // apply the colision and gravity physics for each other particle in particles.
            colision(particles[i], particles[j], pad, bounciness, false);
            gravity(particles[i], particles[j], gravity_const);
        }
        // update the particles.
        particles[i]->update(ticket, x_limit, y_limit, velocity_limit);
        setShapeColorVelocity(&particle_shape, particles[i], velocity_limit); /// colorize shape based on their velocity.

        // draw the particles in a sf::shape.
        particle_shape.setRadius(particles[i]->getRadius());
        particle_shape.setPosition(sf::Vector2f(particles[i]->position.getX(), particles[i]->position.getY()));
        window->draw(particle_shape);
    }
}

int main()
{
    srand((unsigned int)time(NULL));
    
    // values.
    int n_particles = N_PARTICLES;
    float particle_mass = PARTICLE_MASS;
    float particles_radius = PARTICLE_RADIUS;
    float gravity_const = GRAVITY_CONST;
    float bounciness = BOUNCINESS;
    float pad = PAD;
    float velocity_limit = VELOCITY_LIMIT;
    float ticket = TICKET;

    // how much the values will change.
    int update_value_n_particles = 1;
    float update_value_gravity_const = 0.05;
    float update_value_bounciness = 0.05;
    float update_value_pad = 0.05;
    float update_value_velocity_limit = 100;
    float update_value_ticket = 25;

    int old_n_particles = n_particles;

    // list of settings.
    Setting settings[] =
    {
        {"n_particles",      &n_particles,      &update_value_n_particles,      INT_TYPE},
        {"gravity_const",    &gravity_const,    &update_value_gravity_const,    FLOAT_TYPE},
        {"bounciness",       &bounciness,       &update_value_bounciness,       FLOAT_TYPE},
        {"pad",              &pad,              &update_value_pad,              FLOAT_TYPE},
        {"velocity_limit",   &velocity_limit,   &update_value_velocity_limit,   FLOAT_TYPE},
        {"ticket",           &ticket,           &update_value_ticket,           FLOAT_TYPE}
    };
    const int settingCount = 6;
    int currentSetting = 0;


    Particle** particles;
    sf::RenderWindow window(sf::VideoMode(X_LIMIT, Y_LIMIT), "Particles Simulator");
    particles = initiateParticles(n_particles, particle_mass, particles_radius, X_LIMIT, Y_LIMIT, velocity_limit);
    
    // configure the sf::text.
    sf::Font font;
    font.loadFromFile("sansation.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);

    // 
    std::ostringstream osstr;


    while (window.isOpen())
    {
        Sleep(1000 / TICKET);
        window.clear(); // clear window
        update(&window, particles, old_n_particles, pad,
            bounciness, velocity_limit, X_LIMIT, Y_LIMIT, gravity_const, ticket); // update particles.
        osstr.str(""); // erase osstr.
         
        // fullfill osstr.
        for (int i = 0; i < settingCount; i++) {
            osstr << (i == currentSetting ? " >>  " : "     ") << settings[i].name << " = ";
            switch (settings[i].type) {
            case INT_TYPE:
                osstr << *(int*)settings[i].value;
                break;
            case FLOAT_TYPE:
                osstr << *(float*)settings[i].value;
                break;
            }
            osstr << "\n";
        }

        // draw config text.
        text.setString(osstr.str());
        text.setPosition(sf::Vector2f(0.f, 20.f));
        window.draw(text);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Arrow key pressed:
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Return:
                    particles = regenerateParticles(particles, n_particles, old_n_particles, particle_mass, particles_radius, X_LIMIT, Y_LIMIT, velocity_limit);
                    old_n_particles = n_particles;
                    break;
                case sf::Keyboard::Down:   currentSetting = (currentSetting + 1) % settingCount; break;
                case sf::Keyboard::Up:     currentSetting = (currentSetting + settingCount - 1) % settingCount; break;
                case sf::Keyboard::Left:
                    switch (settings[currentSetting].type) {
                    case INT_TYPE:
                        *(int*)settings[currentSetting].value -= *(int*)settings[currentSetting].update_value;
                        break;
                    case FLOAT_TYPE:
                        *(float*)settings[currentSetting].value -= *(float*)settings[currentSetting].update_value;
                        break;
                    }
                    break;
                case sf::Keyboard::Right:
                    switch (settings[currentSetting].type) {
                    case INT_TYPE:
                        *(int*)settings[currentSetting].value += *(int*)settings[currentSetting].update_value;
                        break;
                    case FLOAT_TYPE:
                        *(float*)settings[currentSetting].value += *(float*)settings[currentSetting].update_value;
                        break;
                    }
                    break;
                default: break;
                }

                if (n_particles < 0) {
                    n_particles = 0;
                }
            }
        }
        window.display();

       
    }
    for (int i = 0; i < old_n_particles; i++) {
        free(particles[i]);
    }
    free(particles);
    return 0;
}