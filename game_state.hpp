//
// Created by loic on 18/04/2020.
//

#ifndef LD46_GAME_STATE_HPP
#define LD46_GAME_STATE_HPP

#include <states/state.hpp>
#include <world.hpp>
#include <controllers/playercontroller.hpp>


class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

private:
    PlayerController&   mPlayer;
    World               mWorld;

    bool                mIsGameOver;
    sf::Time            mGameOverTimer;

    MusicPlayer*        mMusicPlayer = nullptr;

};


#endif //LD46_GAME_STATE_HPP
