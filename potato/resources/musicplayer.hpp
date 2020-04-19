#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <map>
#include <string>

#include <resources/resourceholder.hpp>
#include <resources/resourceidentifiers.hpp>

class MusicPlayer : private sf::NonCopyable
{
public:
    MusicPlayer();

    void        play(Musics::ID song);
    void        stop();
    void        pause(bool flag = true);
    bool        isPaused() const;
    void        setVolume(float volume);

private:
    sf::Music   mMusic;
    std::map<Musics::ID,std::string>    mFilenames;
    float       mVolume;
    bool        mPaused = false;
};

#endif // MUSICPLAYER_H
