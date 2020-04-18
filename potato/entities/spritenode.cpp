#include <entities/spritenode.hpp>

SpriteNode::SpriteNode(const sf::Texture &texture) :
    mSprite(texture)
{
    mSprite.setScale({4.f, 4.f});
}

SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect& rect) :
    mSprite(texture, rect)
{
    mSprite.setScale({4.f, 4.f});
}

void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
