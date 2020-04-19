//
// Created by loic on 18/04/2020.
//

#include <utils.hpp>
#include <GUI/button.hpp>
#include "pause_state.hpp"

PauseState::PauseState(StateStack& stack, State::Context context) : State(stack, context)
{
    mBackground.setTexture(context.textures->get(Textures::MenuBackground));
    mBackground.setScale({4.f, 4.f});

    mText.setFont(context.fonts->get(Fonts::Title));
    mText.setString("Game paused!");
    mText.setCharacterSize(75);
    mText.setFillColor({244, 156, 174});
    mText.setOutlineColor({49, 34, 44});
    mText.setOutlineThickness(1.f);
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize().x / 2.f, 80.f);

    mGhost.setTexture(context.textures->get(Textures::Player));
    mGhost.setAnimationNumber(1);
    mGhost.setFrameSize(sf::Vector2i(32,32));
    mGhost.setNumFrames(10);
    mGhost.setDuration(sf::seconds(1.25f));
    mGhost.setRepeating(true);
    mGhost.setScale(sf::Vector2f(6.f, 6.f));
    mGhost.setPosition(100, 300);

    // GUI
    mContainer.setSoundPlayer(context.sounds);
    mContainer.setPosition((context.window->getView().getSize().x - 200) / 2.f, 300.f);

    GUI::Button::Ptr button_resume = GUI::Button::Ptr(new GUI::Button(*context.fonts, *context.textures));
    button_resume->setCallback([this]() {
        requestStackPop();
    });
    button_resume->setText("Resume");
    mContainer.pack(std::move(button_resume));

    GUI::Button::Ptr button_help = GUI::Button::Ptr(new GUI::Button(*context.fonts, *context.textures));
    button_help->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Title);
    });
    button_help->setText("Help");
    button_help->move(0, 75.f);
    mContainer.pack(std::move(button_help));

    GUI::Button::Ptr button_restart = GUI::Button::Ptr(new GUI::Button(*context.fonts, *context.textures));
    button_restart->setCallback([this]() {
        requestStackClear();
        requestStackPush(States::Game);
    });
    button_restart->setText("Restart");
    button_restart->move(0, 150.f);
    mContainer.pack(std::move(button_restart));

    GUI::Button::Ptr button_quit = GUI::Button::Ptr(new GUI::Button(*context.fonts, *context.textures));
    button_quit->setCallback([this]() {
        requestStackClear();
    });
    button_quit->setText("Quit");
    button_quit->move(0, 225.f);
    mContainer.pack(std::move(button_quit));
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackground);
    window.draw(mText);
    window.draw(mContainer);
    window.draw(mGhost);
}

bool PauseState::update(sf::Time dt)
{
    mGhost.update(dt);
    mContainer.update(dt);

    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
        requestStackPop();
    } else if (event.type == sf::Event::JoystickButtonReleased) {
        if (event.joystickButton.button == 1 || event.joystickButton.button == 7) {
            requestStackPop();
        }
    }

    mContainer.handleEvent(event);

    return false;
}
