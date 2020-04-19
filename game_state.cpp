//
// Created by loic on 18/04/2020.
//

#include "game_state.hpp"

GameState::GameState(StateStack& stack, State::Context context) :
State(stack, context),
mPlayer(*context.player),
mWorld(*context.window, *context.textures, *context.fonts, *context.music, *context.sounds, *context.scripts, mPlayer),
mIsGameOver(false),
mMusicPlayer(context.music)
{
    mMusicPlayer->play(Musics::Main);
}

void GameState::draw()
{
//    sf::RenderWindow& window = *getContext().window;

    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    if (mMusicPlayer->isPaused()) mMusicPlayer->pause(false);

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    mWorld.update(dt);

    if (mWorld.won()) {
        requestStackPush(States::Win);
    }

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased) {
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                if(!mIsGameOver) {
                    mMusicPlayer->pause(true);
                    requestStackPush(States::Pause);
                } else {
                    requestStackPop();
                }
                break;

            default:break;
        }
    }  else if (event.type == sf::Event::JoystickButtonReleased) {
        if (event.joystickButton.button == 7) {
            if(!mIsGameOver) {
                mMusicPlayer->pause(true);
                requestStackPush(States::Pause);
            }
        }
    } else if(event.type == sf::Event::LostFocus) {
        if(!mIsGameOver) {
            mMusicPlayer->pause(true);
            requestStackPush(States::Pause);
        }
    }

    return true;
}
