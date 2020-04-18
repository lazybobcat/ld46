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
, mSprite(textures.get(Textures::Player))
, mEmitter(nullptr)
, mCooldown(sf::seconds(0))
{
    mSprite.setScale({4.f, 4.f});

    std::unique_ptr<EmitterNode> particles(new EmitterNode(Particle::Water));
    particles->setPosition(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height - 5);
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
    if (mHasToWater && mCooldown == sf::Time::Zero) {
        sf::Vector2f position = getWorldPosition();
        position += {mSprite.getTexture()->getSize().x * 2.f, mSprite.getTexture()->getSize().y * 2.f};

        if (mWaterCapacity > 0) {
            Command command;
            command.category = Category::Field;
            command.action = derivedAction<FieldNode>([=](FieldNode& field, sf::Time) {
                field.onWaterEvent(position, *this);
            });
            commands.push(command);

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

void PlayerEntity::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);


    sf::FloatRect bound(mSprite.getLocalBounds());
    sf::RectangleShape shape(sf::Vector2f(bound.width * 4, bound.height * 4));
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color(255,255,255, 0));
    shape.setPosition(bound.left, bound.top);
    target.draw(shape, states);
}
