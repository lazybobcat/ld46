#include <datatables.hpp>
#include <resources/particle.hpp>


std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);

    data[Particle::Default].color = sf::Color(152, 64, 113);
    data[Particle::Default].lifetime = sf::seconds(1.3f);

    data[Particle::Water].color = sf::Color(57, 124, 190);
    data[Particle::Water].lifetime = sf::seconds(1.2f);

    return data;
}
