//
// Created by loic on 19/04/2020.
//

#ifndef LD46_WIN_STATE_HPP
#define LD46_WIN_STATE_HPP


#include <states/state.hpp>
#include <resources/animation.hpp>

class WinState : public State
{
public:
    WinState(StateStack& stack, Context context);
    virtual void        draw();
    virtual bool        update(sf::Time dt);
    virtual bool        handleEvent(const sf::Event &event);
private:
    sf::Text mText;
    sf::Text mDescription;
    sf::Sprite mBackground;
    Animation mGhost;
};


#endif //LD46_WIN_STATE_HPP
