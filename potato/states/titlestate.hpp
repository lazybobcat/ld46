#ifndef TITLESTATE_H
#define TITLESTATE_H

#include <SFML/Graphics.hpp>

#include <states/state.hpp>
#include <resources/animation.hpp>
#include <GUI/container.hpp>

class TitleState : public State
{
public:
    TitleState(StateStack& stack, Context context);
    virtual void        draw();
    virtual bool        update(sf::Time dt);
    virtual bool        handleEvent(const sf::Event &event);

private:
    sf::Text mTitle;
    sf::Text mDescription;
    sf::Text mKeys1;
    sf::Text mKeys2;
    sf::Text mTip;
    sf::Text mContinue;
    sf::Sprite mBackground;
    Animation mGhost;
};

#endif // TITLESTATE_H
