#ifndef EMITTERNODE_H
#define EMITTERNODE_H

#include <entities/scenenode.hpp>
#include <resources/particle.hpp>
#include <events/commandqueue.hpp>

class ParticleNode;

class EmitterNode : public SceneNode
{
public:
    explicit EmitterNode(Particle::Type type);

    void emitWaterParticles();

    void stop() { mStopped = true; }
    void start() { mStopped = false; }

    void setEmissionRate(float rate)
    {
        mInterval = sf::seconds(1.f) / rate;
    }

private:
    virtual void    updateCurrent(sf::Time dt, CommandQueue &commands);

    void            emitParticles(sf::Time dt);

private:
    bool            mStopped = false;
    float           mEmissionRate = 1.2f;
    sf::Time        mInterval;
    sf::Time        mAccumulatedTime;
    Particle::Type  mType;
    ParticleNode*   mParticleSystem;
};

#endif // EMITTERNODE_H
