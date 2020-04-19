//
// Created by loic on 18/04/2020.
//

#ifndef LD46_PAUSE_STATE_HPP
#define LD46_PAUSE_STATE_HPP

#include <states/state.hpp>
#include <GUI/container.hpp>

class PauseState : public State
{
public:
    PauseState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

private:
    sf::Text mText;
    sf::Sprite mBackground;
    Animation mGhost;
    GUI::Container mContainer;
};


#endif //LD46_PAUSE_STATE_HPP
