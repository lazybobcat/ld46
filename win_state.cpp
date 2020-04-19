//
// Created by loic on 19/04/2020.
//

#include <utils.hpp>
#include "win_state.hpp"

WinState::WinState(StateStack& stack, State::Context context) : State(stack, context)
{
    mBackground.setTexture(context.textures->get(Textures::MenuBackground));
    mBackground.setScale({4.f, 4.f});

    mText.setFont(context.fonts->get(Fonts::Title));
    mText.setString("Congratulations! You won!");
    mText.setCharacterSize(75);
    mText.setFillColor({244, 156, 174});
    mText.setOutlineColor({49, 34, 44});
    mText.setOutlineThickness(1.f);
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize().x / 2.f, 80.f);

    mDescription.setFont(context.fonts->get(Fonts::Main));
    mDescription.setString("Oo It must have taken you so much time oO");
    mDescription.setCharacterSize(25);
    mDescription.setFillColor({237, 237, 237});
    mDescription.setOutlineColor({49, 34, 44});
    mDescription.setOutlineThickness(1.f);
    centerOrigin(mDescription);
    mDescription.setPosition(context.window->getView().getSize().x / 2.f, 200.f);

    mGhost.setTexture(context.textures->get(Textures::Player));
    mGhost.setAnimationNumber(1);
    mGhost.setFrameSize(sf::Vector2i(32,32));
    mGhost.setNumFrames(10);
    mGhost.setDuration(sf::seconds(1.25f));
    mGhost.setRepeating(true);
    mGhost.setScale(sf::Vector2f(6.f, 6.f));
    mGhost.setPosition(100, 300);
}

void WinState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackground);
    window.draw(mText);
    window.draw(mDescription);
    window.draw(mGhost);
}

bool WinState::update(sf::Time dt)
{
    mGhost.update(dt);
    return false;
}

bool WinState::handleEvent(const sf::Event& event)
{
    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased || event.type == sf::Event::JoystickButtonReleased) {
        requestStackClear();
    }

    return false;
}
