//
// Created by loic on 18/04/2020.
//

#include <utils.hpp>
#include "pause_state.hpp"

PauseState::PauseState(StateStack& stack, State::Context context) : State(stack, context)
{
    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setString("Game paused! Press ESC to quit the game.");
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mText);
}

bool PauseState::update(sf::Time dt)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                requestStackClear();
                break;

            default:
                requestStackPop();
                break;
        }
    }

    return false;
}
