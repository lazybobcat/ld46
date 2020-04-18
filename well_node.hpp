//
// Created by loic on 18/04/2020.
//

#ifndef LD46_WELL_NODE_HPP
#define LD46_WELL_NODE_HPP

#include <resources/resourceholder.hpp>
#include <entities/scenenode.hpp>
#include <resources/resourceidentifiers.hpp>

class PlayerEntity;

class WellNode : public SceneNode
{
public:
    explicit WellNode(TextureHolder& textures);

    void onWaterEvent(sf::Vector2f position, PlayerEntity& player);

    sf::FloatRect getBoundingRect() const override
    {
        return getWorldTransform().transformRect(mSprite.getGlobalBounds());
    }

    unsigned int getCategory() const override;

private:

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;
};


#endif //LD46_WELL_NODE_HPP
