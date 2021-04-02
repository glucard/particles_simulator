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
#define BOUNCINESS 0.3
#define PAD 0
#define X_LIMIT 1366
#define Y_LIMIT 768
#define VELOCITY_LIMIT 2000
#define TICKET 600

struct Setting
{
    const char* name;
    void* value;
};

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

Particle** regenerateParticles(Particle** particles, int n_particles, int old_n_particles, float particle_mass, float particle_radius, float limit_x, float limit_y, float velocity_limit) {
    for (int i = n_particles; i < old_n_particles; i++) {
        free(particles[i]);
    }
    particles = (Particle**)realloc(particles, n_particles * sizeof(Particle*));

    float position_x, position_y, velocity_x, velocity_y;
    for (int i = old_n_particles; i < n_particles; i++) {
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
    srand((unsigned int)time(NULL));

    int n_particles = N_PARTICLES;
    float particle_mass = PARTICLE_MASS;
    float particles_radius = PARTICLE_RADIUS;
    float bounciness = BOUNCINESS;
    float pad = PAD;
    float velocity_limit = VELOCITY_LIMIT;
    float ticket = TICKET;

    int old_n_particles = n_particles;

    Setting settings[] =
    {
        {"n_particles",     &n_particles},
        {"particle_mass", &particle_mass},
        {"particles_radius",          &particles_radius},
        {"bounciness",          &bounciness},
        {"pad", &pad},
        {"velocity_limit",       &velocity_limit},
        {"ticket",        &ticket}
    };

    const int settingCount = 7;
    int currentSetting = 0;

    Particle** particles;
    sf::RenderWindow window(sf::VideoMode(X_LIMIT, Y_LIMIT), "SFML works!");
    particles = initiateParticles(n_particles, particle_mass, particles_radius, X_LIMIT, Y_LIMIT, velocity_limit);
    
    sf::Font font;
    font.loadFromFile("sansation.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);

    std::ostringstream osstr;

    while (window.isOpen())
    {
        Sleep(1000 / TICKET);
        window.clear();
        update(&window, particles, old_n_particles, pad, bounciness, velocity_limit, X_LIMIT, Y_LIMIT, ticket);
        osstr.str("");
         
        for (int i = 0; i < settingCount; i++) {
            osstr << (i == currentSetting ? " >>  " : "     ") << settings[i].name << " = ";
            if (settings[i].name == "n_particles") {
                osstr << *(int*)settings[i].value;
            }
            else {
                osstr << *(float*)settings[i].value;
            }
            osstr << "\n";
             
            
        }
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
                    if (settings[currentSetting].name == "n_particles") {
                        *(int*)settings[currentSetting].value -= 1;
                    }
                    else if (settings[currentSetting].name == "particle_mass") {
                        *(float*)settings[currentSetting].value -= 5;
                    }
                    else if (settings[currentSetting].name == "particles_radius") {
                        *(float*)settings[currentSetting].value -= 5;
                    }
                    else if (settings[currentSetting].name == "bounciness") {
                        *(float*)settings[currentSetting].value -= 0.05;
                    }
                    else if (settings[currentSetting].name == "pad") {
                        *(float*)settings[currentSetting].value -= 0.05;
                    }
                    else if (settings[currentSetting].name == "ticket") {
                        *(float*)settings[currentSetting].value -= 25;
                    }
                    break;
                case sf::Keyboard::Right:
                    if (settings[currentSetting].name == "n_particles") {
                        *(int*)settings[currentSetting].value += 1;
                    }
                    else if (settings[currentSetting].name == "particle_mass") {
                        *(float*)settings[currentSetting].value += 5;
                    }
                    else if (settings[currentSetting].name == "particles_radius") {
                        *(float*)settings[currentSetting].value += 5;
                    }
                    else if (settings[currentSetting].name == "bounciness") {
                        *(float*)settings[currentSetting].value += 0.05;
                    }
                    else if (settings[currentSetting].name == "pad") {
                        *(float*)settings[currentSetting].value += 0.05;
                    }
                    else if (settings[currentSetting].name == "ticket") {
                        *(float*)settings[currentSetting].value += 25;
                    }
                    break;
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