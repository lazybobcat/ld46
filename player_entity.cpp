//
// Created by loic on 18/04/2020.
//

#include <entities/emitternode.hpp>
#include <iostream>
#include "player_entity.hpp"
#include "field_node.hpp"
#include "well_node.hpp"

PlayerEntity::PlayerEntity(TextureHolder& textures)
: Entity()
, mEmitter(nullptr)
, mCooldown(sf::seconds(0))
{
    mAnimations[(int)Animations::WalkingLeft].setTexture(textures.get(Textures::Player));
    mAnimations[(int)Animations::WalkingLeft].setAnimationNumber(0);
    mAnimations[(int)Animations::WalkingLeft].setFrameSize(sf::Vector2i(32,32));
    mAnimations[(int)Animations::WalkingLeft].setNumFrames(10);
    mAnimations[(int)Animations::WalkingLeft].setDuration(sf::seconds(1.25f));
    mAnimations[(int)Animations::WalkingLeft].setRepeating(true);
    mAnimations[(int)Animations::WalkingLeft].setScale(sf::Vector2f(4.f, 4.f));

    mAnimations[(int)Animations::WalkingRight].setTexture(textures.get(Textures::Player));
    mAnimations[(int)Animations::WalkingRight].setAnimationNumber(1);
    mAnimations[(int)Animations::WalkingRight].setFrameSize(sf::Vector2i(32,32));
    mAnimations[(int)Animations::WalkingRight].setNumFrames(10);
    mAnimations[(int)Animations::WalkingRight].setDuration(sf::seconds(1.25f));
    mAnimations[(int)Animations::WalkingRight].setRepeating(true);
    mAnimations[(int)Animations::WalkingRight].setScale(sf::Vector2f(4.f, 4.f));

    mAnimations[(int)Animations::WalkingDown].setTexture(textures.get(Textures::Player));
    mAnimations[(int)Animations::WalkingDown].setAnimationNumber(2);
    mAnimations[(int)Animations::WalkingDown].setFrameSize(sf::Vector2i(32,32));
    mAnimations[(int)Animations::WalkingDown].setNumFrames(10);
    mAnimations[(int)Animations::WalkingDown].setDuration(sf::seconds(1.25f));
    mAnimations[(int)Animations::WalkingDown].setRepeating(true);
    mAnimations[(int)Animations::WalkingDown].setScale(sf::Vector2f(4.f, 4.f));

    mAnimations[(int)Animations::WalkingUp].setTexture(textures.get(Textures::Player));
    mAnimations[(int)Animations::WalkingUp].setAnimationNumber(3);
    mAnimations[(int)Animations::WalkingUp].setFrameSize(sf::Vector2i(32,32));
    mAnimations[(int)Animations::WalkingUp].setNumFrames(10);
    mAnimations[(int)Animations::WalkingUp].setDuration(sf::seconds(1.25f));
    mAnimations[(int)Animations::WalkingUp].setRepeating(true);
    mAnimations[(int)Animations::WalkingUp].setScale(sf::Vector2f(4.f, 4.f));

    mAnimations[(int)Animations::Watering].setTexture(textures.get(Textures::Player));
    mAnimations[(int)Animations::Watering].setAnimationNumber(4);
    mAnimations[(int)Animations::Watering].setFrameSize(sf::Vector2i(32,32));
    mAnimations[(int)Animations::Watering].setNumFrames(10);
    mAnimations[(int)Animations::Watering].setDuration(sf::seconds(1.f));
    mAnimations[(int)Animations::Watering].setRepeating(true);
    mAnimations[(int)Animations::Watering].setScale(sf::Vector2f(4.f, 4.f));

    std::unique_ptr<EmitterNode> particles(new EmitterNode(Particle::Water));
    particles->setPosition(128 / 2.f, 128.f);
    particles->stop();
    mEmitter = particles.get();
    attachChild(std::move(particles));
}

void PlayerEntity::water()
{
    if (mCooldown == sf::Time::Zero) {
        mHasToWater = true;
    }
}

unsigned int PlayerEntity::getCategory() const
{
    return Category::Player;
}

void PlayerEntity::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateAnimations(dt);

    if (mHasToWater && mCooldown == sf::Time::Zero) {
        sf::Vector2f position = getWorldPosition();
        position += {128 / 2.f, 128.f};

        if (mWaterCapacity > 0) {
            Command command;
            command.category = Category::Field;
            command.action = derivedAction<FieldNode>([=](FieldNode& field, sf::Time) {
                field.onWaterEvent(position, *this);
            });
            commands.push(command);

            direction = Direction::Left;
            if (mEmitter) mEmitter->emitWaterParticles();
        }

        Command fillCommand;
        fillCommand.category = Category::Well;
        fillCommand.action = derivedAction<WellNode>([=](WellNode& field, sf::Time) {
            field.onWaterEvent(position, *this);
        });
        commands.push(fillCommand);

        // Update player state
        mHasToWater = false;
        mWaterCapacity -= 1;
        mCooldown = sf::seconds(1);
    }
    if (mCooldown > sf::Time::Zero) {
        if (mCooldown < dt) mCooldown = sf::Time::Zero;
        else mCooldown -= dt;
    }
}

void PlayerEntity::updateAnimations(sf::Time dt)
{
    if (mCooldown > sf::Time::Zero) {
        mAnimations[(int)Animations::Watering].update(dt);
        return;
    }

    switch (direction)
    {
        case Direction::Up:
            mAnimations[(int)Animations::WalkingUp].update(dt);
            break;
        case Direction::Down:
            mAnimations[(int)Animations::WalkingDown].update(dt);
            break;
        case Direction::Left:
            mAnimations[(int)Animations::WalkingLeft].update(dt);
            break;
        case Direction::Right:
            mAnimations[(int)Animations::WalkingRight].update(dt);
            break;
    }
}

void PlayerEntity::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mCooldown > sf::Time::Zero) {
        target.draw(mAnimations[(int)Animations::Watering], states);
    } else {
        switch (direction)
        {
            case Direction::Up:
                target.draw(mAnimations[(int)Animations::WalkingUp], states);
                break;
            case Direction::Down:
                target.draw(mAnimations[(int)Animations::WalkingDown], states);
                break;
            case Direction::Left:
                target.draw(mAnimations[(int)Animations::WalkingLeft], states);
                break;
            case Direction::Right:
                target.draw(mAnimations[(int)Animations::WalkingRight], states);
                break;
        }
    }


//    sf::FloatRect bound({0, 0, 32, 32});
//    sf::RectangleShape shape(sf::Vector2f(bound.width * 4, bound.height * 4));
//    shape.setOutlineColor(sf::Color::Red);
//    shape.setOutlineThickness(1.f);
//    shape.setFillColor(sf::Color(255,255,255, 0));
//    shape.setPosition(bound.left, bound.top);
//    target.draw(shape, states);
}
