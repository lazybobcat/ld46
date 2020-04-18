//
// Created by loic on 18/04/2020.
//

#ifndef LD46_PLAYER_ENTITY_HPP
#define LD46_PLAYER_ENTITY_HPP

#include <entities/entity.hpp>
#include <entities/emitternode.hpp>

enum class Direction
{
    Up, Down, Left, Right
};

class PlayerEntity : public Entity
{
public:
    explicit PlayerEntity(TextureHolder& textures);

    unsigned int getCategory() const override;

    void water();
    void fill(int amount = 10) { mWaterCapacity = amount; }

    bool onCooldown() const { return mCooldown != sf::Time::Zero; }
    int capacity() const { return mWaterCapacity; }

    Direction direction = Direction::Down;
    float     speed = 400.f;

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;
    EmitterNode*    mEmitter;

    bool mHasToWater = false;
    int mWaterCapacity = 10;
    sf::Time mCooldown;
};


#endif //LD46_PLAYER_ENTITY_HPP
