#include <resources/soundplayer.hpp>

#include <cmath>
#include <utils.hpp>

namespace
{
    const float ListenerZ = 300.f;
    const float Attenuation = 8.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

/////////////////////////////////////////

SoundPlayer::SoundPlayer() :
    mSoundBuffers(),
    mSounds()
{
    // Fill buffers with sounds
    mSoundBuffers.load(Sounds::Water, "assets/sounds/water.ogg");
    mSoundBuffers.load(Sounds::FieldUpgrade, "assets/sounds/upgrade.ogg");
    mSoundBuffers.load(Sounds::FieldUpgradeFinal, "assets/sounds/upgrade-final.ogg");
    mSoundBuffers.load(Sounds::FieldDecay, "assets/sounds/decay.ogg");
    mSoundBuffers.load(Sounds::Fill, "assets/sounds/fill.ogg");
    mSoundBuffers.load(Sounds::Select, "assets/sounds/select.ogg");
    mSoundBuffers.load(Sounds::Validate, "assets/sounds/validate.ogg");
}

void SoundPlayer::play(Sounds::ID sound)
{
    play(sound, getListenerPosition());
}

void SoundPlayer::play(Sounds::ID effect, sf::Vector2f position)
{
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();

    sound.setBuffer(mSoundBuffers.get(effect));
    sound.setPosition(position.x, position.y, 0.f);
    sound.setAttenuation(Attenuation);
    sound.setMinDistance(MinDistance3D);
    sound.setVolume(50.f);
    sound.setPitch(randomFloat(0.6f, 1.5f));

    sound.play();
}


void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
    });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    return sf::Vector2f(sf::Listener::getPosition().x, sf::Listener::getPosition().y);
}
