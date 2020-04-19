#ifndef WORLD_H
#define WORLD_H

#include <array>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <resources/resourceholder.hpp>
#include <resources/resourceidentifiers.hpp>
#include <resources/musicplayer.hpp>
#include <resources/soundplayer.hpp>
#include <resources/scriptplayer.hpp>
#include <controllers/playercontroller.hpp>
#include <events/category.hpp>
#include <events/commandqueue.hpp>
#include <entities/spritenode.hpp>
#include "../bloom_effect.hpp"
#include "../player_entity.hpp"
#include "../points_node.hpp"

class World : public sf::NonCopyable
{
public:
    World(sf::RenderWindow& window, TextureHolder &textures, FontHolder &fonts, MusicPlayer &music, SoundPlayer &sounds, ScriptPlayer &scripts, PlayerController &player);

    void        update(sf::Time dt);
    void        draw();

    bool        won() const { return mGameWon; }

    CommandQueue&   getCommandQueue();

private:
    void        loadTextures();
    void        buildScene();

private:
    enum Layer
    {
        Background,
        Foreground,
        Ui,
        LayerCount
    };

private:
    sf::RenderWindow&                   mWindow;
    sf::View                            mWorldView;
    sf::View                            mUiView;
    TextureHolder&                      mTextures;
    FontHolder&                         mFonts;
    MusicPlayer&                        mMusic;
    SoundPlayer&                        mSounds;
    ScriptPlayer&                       mScripts;
    PlayerController&                   mPlayer;

    PlayerEntity*                       mPlayerEntity;
    PointsNode*                         mPointsNode;

    sf::RenderTexture                   mSceneTexture;
    BloomEffect                         mBloomEffect;

    SceneNode                           mSceneGraph;
    std::array<SceneNode*,LayerCount>   mSceneLayers;
    CommandQueue                        mCommandQueue;

    // UI
    unsigned int                        mWaterCanCapacity = 10;
    sf::Sprite                          mWaterCan;
    sf::Sprite                          mWaterCanBackground;
    sf::Sprite                          mStar;
    sf::Text                            mPoints;

    bool                                mGameWon = false;
};

#endif // WORLD_H
