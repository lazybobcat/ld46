#include <datatables.hpp>
#include <resources/particle.hpp>


std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);

    data[Particle::Default].color = sf::Color(237, 237, 237);
    data[Particle::Default].lifetime = sf::seconds(1.3f);

    data[Particle::Growing].color = sf::Color(169, 218, 163);
    data[Particle::Growing].lifetime = sf::seconds(1.3f);

    data[Particle::Water].color = sf::Color(57, 124, 190);
    data[Particle::Water].lifetime = sf::seconds(1.2f);

    return data;
}

