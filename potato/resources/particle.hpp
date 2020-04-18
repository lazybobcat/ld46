#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

struct Particle
{
    enum Type
    {
        Default,
        Water,
        ParticleCount
    };

    sf::Vector2f    position;
    sf::Color       color;
    sf::Time        lifetime;
    float           direction;
    float           speed = -125.f;
};

#endif // PARTICLE_H
