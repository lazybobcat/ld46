#ifndef PARTICLENODE_H
#define PARTICLENODE_H

#include <SFML/Graphics/VertexArray.hpp>

#include <deque>
#include <vector>

#include <entities/scenenode.hpp>
#include <resources/resourceholder.hpp>
#include <resources/resourceidentifiers.hpp>
#include <resources/particle.hpp>
#include <resources/affectors.hpp>
#include <events/commandqueue.hpp>

class ParticleNode : public SceneNode
{
public:
    ParticleNode(Particle::Type type, TextureHolder& textures);
    virtual ~ParticleNode() = default;

    void                    addParticle(sf::Vector2f position);
    void                    addParticle(sf::Vector2f position, float angle);
    Particle::Type          getParticleType() const;

    void                    addAffector(std::function<void(Particle&, sf::Time)> affector);

protected:
    virtual void            updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void            drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

    void                    addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
    void                    computeVertices() const;



private:
    typedef std::function<void(Particle&, sf::Time)>    Affector;

protected:
    std::deque<Particle>    mParticles;
    std::vector<Affector>   mAffectors;
    sf::Texture&            mTexture;
    Particle::Type          mType;

    mutable sf::VertexArray mVertexArray;
    mutable bool            mNeedsVertexUpdate;
};

#endif // PARTICLENODE_H
