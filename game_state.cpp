//
// Created by loic on 18/04/2020.
//

#include "game_state.hpp"

GameState::GameState(StateStack& stack, State::Context context) :
State(stack, context),
mPlayer(*context.player),
mWorld(*context.window, *context.textures, *context.fonts, *context.music, *context.sounds, *context.scripts, mPlayer),
mIsGameOver(false)
{

}

void GameState::draw()
{
//    sf::RenderWindow& window = *getContext().window;

    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    mWorld.update(dt);

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                if(!mIsGameOver) {
                    requestStackPush(States::Pause);
                } else {
                    requestStackPop();
                }
                break;

            default:break;
        }
    }
    else if(event.type == sf::Event::LostFocus)
    {
        if(!mIsGameOver) requestStackPush(States::Pause);
    }

    return true;
}
