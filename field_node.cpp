//
// Created by loic on 18/04/2020.
//

#include <utils.hpp>
#include <iostream>
#include "field_node.hpp"

constexpr float DECAY_TIME = 20;
constexpr float GROW_TIME = DECAY_TIME / 2.f;

FieldNode::FieldNode(TextureHolder& textures)
{
    mSprite.setTexture(textures.get(Textures::Tiles));
    mSprite.setTextureRect({0, 0, 32, 32});
    mSprite.setScale({4.f, 4.f});

    std::unique_ptr<EmitterNode> particles(new EmitterNode(Particle::Growing));
    particles->setPosition(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
    particles->stop();
    mGrowingEmitter = particles.get();
    attachChild(std::move(particles));

    mAnimation.setTexture(textures.get(Textures::TilesFull));
    mAnimation.setAnimationNumber(randomInt(0, 2));
    mAnimation.setFrameSize(sf::Vector2i(32,32));
    mAnimation.setNumFrames(4);
    mAnimation.setDuration(sf::seconds(1.25f));
    mAnimation.setRepeating(true);
    mAnimation.setScale(sf::Vector2f(4.f, 4.f));
}

void FieldNode::onWaterEvent(sf::Vector2f position, PlayerEntity& player)
{
    auto rect = getBoundingRect();
    if (rect.contains(position)) {
        upgrade();
    }
}

void FieldNode::upgrade()
{
    switch (mState)
    {
        case FieldState::Dirt:
            mState = FieldState::Growing;
            resetDecayTimer();
            if (mGrowTimer == sf::Time::Zero) mGrowTimer = sf::seconds(GROW_TIME);
            if (mGrowingEmitter) mGrowingEmitter->start();
            break;
        case FieldState::Grown:
            mState = FieldState::GrownHumid;
            resetDecayTimer();
            if (mGrowTimer == sf::Time::Zero) mGrowTimer = sf::seconds(GROW_TIME);
            if (mGrowingEmitter) mGrowingEmitter->start();
            break;
        case FieldState::Growing:
            if (mGrowTimer == sf::Time::Zero || mGrowTimer > sf::seconds(GROW_TIME)) mGrowTimer = sf::seconds(GROW_TIME);
            if (mGrowingEmitter) mGrowingEmitter->start();
            break;
        case FieldState::Full:
        default:
            resetDecayTimer();
            break;
    }
}

void FieldNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    mGrowTimer -= dt;
    mDecayTimer -= dt;

    if (mGrowTimer <= sf::Time::Zero) mGrowTimer = sf::Time::Zero;
    if (mDecayTimer <= sf::Time::Zero) mDecayTimer = sf::Time::Zero;


    switch (mState)
    {
        case FieldState::Dirt:
            mSprite.setTextureRect({0, 0, 32, 32});
            break;

        case FieldState::Growing:
            mSprite.setTextureRect({32, 0, 32, 32});
            if (mGrowTimer == sf::Time::Zero) {
                mState = FieldState::Grown;
                resetDecayTimer();
                mWateredCount = 0;
                if (mGrowingEmitter) mGrowingEmitter->stop();
            } else if (mDecayTimer == sf::Time::Zero) {
                mState = FieldState::Dirt;
                mWateredCount = 0;
            }
            break;

        case FieldState::Grown:
            mSprite.setTextureRect({64, 0, 32, 32});
            if (mDecayTimer == sf::Time::Zero) {
                mState = FieldState::Growing;
                mGrowTimer = sf::seconds(DECAY_TIME + GROW_TIME);
                resetDecayTimer();
            }
            break;

        case FieldState::GrownHumid:
            mSprite.setTextureRect({96, 0, 32, 32});
            if (mGrowTimer == sf::Time::Zero) {
                mWateredCount++;
                if (mWateredCount >= 3) {
                    mState = FieldState::Full;
                    mWateredCount = 0;
                } else {
                    mState = FieldState::Grown;
                    resetDecayTimer();
                }
                if (mGrowingEmitter) mGrowingEmitter->stop();
            }
            break;

        case FieldState::Full:
            mAnimation.update(dt);
//            mSprite.setTextureRect({128, 0, 32, 32});
            if (mGrowingEmitter) mGrowingEmitter->stop();
            if (!mFullEmitter) {
                std::unique_ptr<EmitterNode> particles(new EmitterNode(Particle::Default));
                particles->setPosition(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
                mFullEmitter = particles.get();
                attachChild(std::move(particles));
            }
            break;
    }
}

void FieldNode::resetDecayTimer()
{
    mDecayTimer = sf::seconds(DECAY_TIME);
}

void FieldNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mState == FieldState::Full) {
        target.draw(mAnimation, states);
    } else {
        target.draw(mSprite, states);
    }
}

unsigned int FieldNode::getCategory() const
{
    return Category::Field;
}
