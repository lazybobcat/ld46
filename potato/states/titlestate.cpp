#include <states/titlestate.hpp>
#include <utils.hpp>

TitleState::TitleState(StateStack &stack, Context context) :
    State(stack, context)
{
    mBackground.setTexture(context.textures->get(Textures::TitleBackground));
    mBackground.setScale({4.f, 4.f});

    mTitle.setFont(context.fonts->get(Fonts::Title));
    mTitle.setString("Garden Keeper");
    mTitle.setCharacterSize(75);
    mTitle.setFillColor({244, 156, 174});
    mTitle.setOutlineColor({49, 34, 44});
    mTitle.setOutlineThickness(1.f);
    centerOrigin(mTitle);
    mTitle.setPosition(context.window->getView().getSize().x / 2.f, 80.f);

    mDescription.setFont(context.fonts->get(Fonts::Main));
    mDescription.setString("Keep the garden alive by watering it!\n\nIf you water a plot enough, it will last forever and earn you a point.\nBut if you don't water a plot enough, it will start to decay.\nIf your watering can is empty, fill it up at the well!");
    mDescription.setCharacterSize(18);
    mDescription.setFillColor({237, 237, 237});
    mDescription.setPosition(115, 245);

    mKeys1.setFont(context.fonts->get(Fonts::Main));
    mKeys1.setString("Use your keyboard or your Xbox 360 controller to move around.");
    mKeys1.setCharacterSize(18);
    mKeys1.setFillColor({237, 237, 237});
    mKeys1.setPosition(320, 380);

    mKeys2.setFont(context.fonts->get(Fonts::Main));
    mKeys2.setString("Use keyboard's 'SPACE' or controller's 'A' to water a plot\nor fill up your watering can.");
    mKeys2.setCharacterSize(18);
    mKeys2.setFillColor({237, 237, 237});
    mKeys2.setPosition(320, 480);

    mTip.setFont(context.fonts->get(Fonts::Main));
    mTip.setString("PRO TIP: A growing plot has green stars flying around. If it doesn't, you probably\nshould water it.");
    mTip.setCharacterSize(18);
    mTip.setFillColor({237, 237, 237});
    mTip.setPosition(115, 580);

    mContinue.setFont(context.fonts->get(Fonts::Main));
    mContinue.setString("Press 'SPACE' or 'A' to play!");
    mContinue.setCharacterSize(22);
    mContinue.setFillColor({237, 237, 237});
    centerOrigin(mContinue);
    mContinue.setPosition(context.window->getView().getSize().x / 2.f, 640);

    mGhost.setTexture(context.textures->get(Textures::Player));
    mGhost.setAnimationNumber(1);
    mGhost.setFrameSize(sf::Vector2i(32,32));
    mGhost.setNumFrames(10);
    mGhost.setDuration(sf::seconds(1.25f));
    mGhost.setRepeating(true);
    mGhost.setScale(sf::Vector2f(-4.f, 4.f));
    mGhost.setPosition(1100, 420);
}


void TitleState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(mBackground);
    window.draw(mTitle);
    window.draw(mGhost);
    window.draw(mDescription);
    window.draw(mKeys1);
    window.draw(mKeys2);
    window.draw(mTip);
    window.draw(mContinue);
}

bool TitleState::update(sf::Time dt)
{
    mGhost.update(dt);

    return false;
}

bool TitleState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
    {
        requestStackPop();
    }
    else if (event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == 0)
    {
        requestStackPop();
    }

    return false;
}
