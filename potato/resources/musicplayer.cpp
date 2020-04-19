#include <resources/musicplayer.hpp>

MusicPlayer::MusicPlayer() :
    mMusic(),
    mFilenames(),
    mVolume(100.f)
{
    // Fill mFilenames table here :
    mFilenames[Musics::Main] = "assets/musics/ld46.flac";
}

void MusicPlayer::play(Musics::ID song)
{
    std::string filename = mFilenames[song];

    if (!mMusic.openFromFile(filename))
        throw std::runtime_error("Music " + filename + " could not be loaded.");

    mMusic.setVolume(mVolume);
    mMusic.setLoop(true);
    mMusic.play();
}

void MusicPlayer::stop()
{
    mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
    mVolume = volume;
    mMusic.setVolume(volume);
}

void MusicPlayer::pause(bool flag)
{
    if (flag) {
        mMusic.pause();
        mPaused = true;
    } else {
        mMusic.play();
        mPaused = false;
    }
}

bool MusicPlayer::isPaused() const
{
    return mPaused;
}
