#include <entities/emitternode.hpp>
#include <entities/particlenode.hpp>
#include <utils.hpp>

EmitterNode::EmitterNode(Particle::Type type) :
    SceneNode(),
    mAccumulatedTime(sf::Time::Zero),
    mType(type),
    mParticleSystem(nullptr)
{
    mInterval = sf::seconds(1.f) / 1.2f;
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if(mParticleSystem)
    {
        emitParticles(dt);
    }
    else
    {
        auto finder = [this](ParticleNode& container, sf::Time) {
            if(container.getParticleType() == mType)
                mParticleSystem = &container;
        };
        Command command;
        command.action = derivedAction<ParticleNode>(finder);
        command.category = Category::ParticleSystem;
        commands.push(command);
    }
}

void EmitterNode::emitParticles(sf::Time dt)
{
    if (mStopped) return;

    mAccumulatedTime += dt;

    float rand = randomFloat(0.2f, 1.6f);

    while(mAccumulatedTime > (mInterval + sf::seconds(rand)))
    {
        mAccumulatedTime -= mInterval;
        mParticleSystem->addParticle(getWorldPosition());
    }
}

void EmitterNode::emitWaterParticles()
{
    if(mParticleSystem == nullptr)    return;

    int nb_particles = 12;
    auto position = getWorldPosition();

    for(int i = 0; i < nb_particles; ++i)
    {
        mParticleSystem->addParticle(position, randomFloat(60.f, 120.f));
    }
}
