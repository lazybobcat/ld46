#include <controllers/playercontroller.hpp>
#include <events/commandqueue.hpp>
#include <iostream>
#include "../../player_entity.hpp"

PlayerController::PlayerController()
{
    // Default keybindings
    mKeyBinding[sf::Keyboard::Left] = MoveLeft;
    mKeyBinding[sf::Keyboard::Q] = MoveLeft;
    mKeyBinding[sf::Keyboard::A] = MoveLeft;

    mKeyBinding[sf::Keyboard::Right] = MoveRight;
    mKeyBinding[sf::Keyboard::D] = MoveRight;
    mKeyBinding[sf::Keyboard::E] = MoveRight;

    mKeyBinding[sf::Keyboard::Up] = MoveUp;
    mKeyBinding[sf::Keyboard::Z] = MoveUp;
    mKeyBinding[sf::Keyboard::W] = MoveUp;

    mKeyBinding[sf::Keyboard::Down] = MoveDown;
    mKeyBinding[sf::Keyboard::S] = MoveDown;

    mKeyBinding[sf::Keyboard::Space] = Water;

    // initialize actions
    initializeActions();
}

void PlayerController::handleEvent(const sf::Event &event, CommandQueue &commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        // Check if pressed key appears in key binding, trigger command if so
        auto found = mKeyBinding.find(event.key.code);
        if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
            commands.push(mActionBinding[found->second]);
    }
}

void PlayerController::handleRealtimeInput(CommandQueue &commands)
{
    for(auto pair : mKeyBinding)
    {
        if(sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
        {
            commands.push(mActionBinding[pair.second]);
        }
    }
}

void PlayerController::assignKey(Action action, sf::Keyboard::Key key)
{
    // Remove all keys that already map to action
    for(auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
    {
        if (itr->second == action)
            mKeyBinding.erase(itr++);
        else
            ++itr;
    }

    // Insert new binding
    mKeyBinding[key] = action;
}

sf::Keyboard::Key PlayerController::getAssignedKey(Action action) const
{
    for(auto pair : mKeyBinding)
    {
        if (pair.second == action)
            return pair.first;
    }

    return sf::Keyboard::Unknown;
}

void PlayerController::initializeActions()
{
    mActionBinding[MoveLeft].action = derivedAction<PlayerEntity>([](PlayerEntity& player, sf::Time dt) {
        if (!player.onCooldown()) player.move(-player.speed * dt.asSeconds(), 0);
    });
    mActionBinding[MoveLeft].category = Category::Player;

    mActionBinding[MoveRight].action = derivedAction<PlayerEntity>([](PlayerEntity& player, sf::Time dt) {
        if (!player.onCooldown()) player.move(player.speed * dt.asSeconds(), 0);
    });
    mActionBinding[MoveRight].category = Category::Player;

    mActionBinding[MoveDown].action = derivedAction<PlayerEntity>([](PlayerEntity& player, sf::Time dt) {
        if (!player.onCooldown()) player.move(0, player.speed * dt.asSeconds());
    });
    mActionBinding[MoveDown].category = Category::Player;

    mActionBinding[MoveUp].action = derivedAction<PlayerEntity>([](PlayerEntity& player, sf::Time dt) {
        if (!player.onCooldown()) player.move(0, -player.speed * dt.asSeconds());
    });
    mActionBinding[MoveUp].category = Category::Player;

    mActionBinding[Water].action = derivedAction<PlayerEntity>([](PlayerEntity& player, sf::Time) {
        player.water();
    });
    mActionBinding[Water].category = Category::Player;

    // ...
}

bool PlayerController::isRealtimeAction(Action action)
{
    switch (action)
    {
        case MoveLeft:
        case MoveRight:
        case MoveDown:
        case MoveUp:
            return true;

        default:
            return false;
    }
}

