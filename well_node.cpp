//
// Created by loic on 18/04/2020.
//

#include <iostream>
#include "well_node.hpp"
#include "player_entity.hpp"

WellNode::WellNode(TextureHolder& textures)
{
    mSprite.setTexture(textures.get(Textures::Tiles));
    mSprite.setTextureRect({0, 64, 32, 32});
    mSprite.setScale({4.f, 4.f});

    std::unique_ptr<EmitterNode> particles(new EmitterNode(Particle::Water));
    particles->setPosition(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
    particles->setEmissionRate(1.5f);
    attachChild(std::move(particles));
}

void WellNode::onWaterEvent(sf::Vector2f position, PlayerEntity& player)
{
    auto rect = getBoundingRect();
    if (rect.contains(position)) {
        player.fill(10);
        player.playFillSound();
    }
}

unsigned int WellNode::getCategory() const
{
    return Category::Well;
}

void WellNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void WellNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
