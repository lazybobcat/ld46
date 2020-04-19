//
// Created by loic on 18/04/2020.
//

#ifndef LD46_FIELD_NODE_HPP
#define LD46_FIELD_NODE_HPP

#include <resources/resourceidentifiers.hpp>
#include <resources/resourceholder.hpp>
#include <entities/scenenode.hpp>
#include <entities/emitternode.hpp>

enum class FieldState
{
    Dirt = 0,
    Growing,
    Grown,
    GrownHumid,
    Full
};

class PlayerEntity;

class FieldNode : public SceneNode
{
public:
    explicit FieldNode(TextureHolder& textures);

    void onWaterEvent(sf::Vector2f position, PlayerEntity& player);

    sf::FloatRect getBoundingRect() const override
    {
        return getWorldTransform().transformRect(mSprite.getGlobalBounds());
    }

    unsigned int getCategory() const override;

private:
    void upgrade();
    void resetDecayTimer();

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;
    Animation  mAnimation;
    FieldState mState = FieldState::Dirt;

    int mWateredCount = 0;
    bool mGrowing = false;
    sf::Time mGrowTimer = sf::Time::Zero;
    sf::Time mDecayTimer = sf::Time::Zero;

    EmitterNode* mFullEmitter = nullptr;
    EmitterNode* mGrowingEmitter = nullptr;
};


#endif //LD46_FIELD_NODE_HPP
