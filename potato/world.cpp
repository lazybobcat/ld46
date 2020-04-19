#include <world.hpp>
#include <iostream>
#include <resources/particle.hpp>
#include <entities/particlenode.hpp>
#include <cmath>
#include <algorithm>
#include "../field_node.hpp"
#include "../player_entity.hpp"
#include "../well_node.hpp"
#include "utils.hpp"

enum class TT
{
    Field = 0,
    Path = 1,
    Well = 2,
};

// 18 x 9
TT TileMap[] = {
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field,
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path,  TT::Field, TT::Field, TT::Field, TT::Field,
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Path, TT::Path, TT::Path,  TT::Field, TT::Field, TT::Field, TT::Field,
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field,
    TT::Field, TT::Path, TT::Path, TT::Path, TT::Path, TT::Well,  TT::Path, TT::Path, TT::Path, TT::Path, TT::Path, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field,
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Path, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Path, TT::Path, TT::Path, TT::Field,
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field,
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Path, TT::Field, TT::Field, TT::Field, TT::Field,
    TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field, TT::Field,
};

constexpr float SPRITE_WIDTH = 128.f;
constexpr float MAP_WIDTH = 18 * SPRITE_WIDTH;
constexpr float MAP_HEIGHT = 9 * SPRITE_WIDTH;
constexpr int SCREEN_WIDTH  = 1152;
constexpr int SCREEN_HEIGHT = SCREEN_WIDTH * 10 / 16;



/**********************************************************/


World::World(sf::RenderWindow& window, TextureHolder &textures, FontHolder &fonts, MusicPlayer &music, SoundPlayer &sounds, ScriptPlayer &scripts, PlayerController &player) :
    mWindow(window),
    mUiView(window.getDefaultView()),
    mTextures(textures),
    mFonts(fonts),
    mMusic(music),
    mSounds(sounds),
    mScripts(scripts),
    mPlayer(player),
    mPlayerEntity(nullptr),
    mSceneGraph(),
    mSceneLayers()
{
    loadTextures();
    buildScene();

    // Other things here, like setting the view center on the player, scores, etc...
    std::unique_ptr<SoundNode> sound(new SoundNode(sounds));
    mSceneGraph.attachChild(std::move(sound));

    mWaterCan.setTexture(textures.get(Textures::WaterCan));
    mWaterCan.setOrigin(0, 13);
    mWaterCan.setScale({6.f, 6.f});
    mWaterCan.setPosition(SCREEN_WIDTH - 30*6, 100);

    mWaterCanBackground.setTexture(textures.get(Textures::WaterCan));
    mWaterCanBackground.setOrigin(0, 13);
    mWaterCanBackground.setScale({6.f, 6.f});
    mWaterCanBackground.setColor({0, 0, 0, 128});
    mWaterCanBackground.setPosition(SCREEN_WIDTH - 30*6, 100);

    mStar.setTexture(textures.get(Textures::Star));
    mStar.setScale({4.f, 4.f});
    mStar.setPosition(50, 50);

    mPoints.setFont(fonts.get(Fonts::Main));
    mPoints.setCharacterSize(20);
    mPoints.setFillColor({237, 237, 237});
    mPoints.setOutlineColor({49, 34, 44});
    mPoints.setPosition(100, 60);
}


void World::loadTextures()
{

}

void World::buildScene()
{
    mWorldView.setSize(mWindow.getSize().x, mWindow.getSize().y);
    mWorldView.setCenter(mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f);
    mSceneTexture.create(mWindow.getSize().x, mWindow.getSize().y);

    // Initialize layers
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Tiled map
    for (int y = 0; y < 9; ++y) {
        for (int x = 0; x < 18; ++x) {
            switch (TileMap[y * 18 + x])
            {
                case TT::Field:
                {
                    std::unique_ptr<FieldNode> tile(new FieldNode(mTextures));
                    tile->setPosition(x * SPRITE_WIDTH, y * SPRITE_WIDTH);
                    mSceneLayers[Foreground]->attachChild(std::move(tile));
                    break;
                }
                case TT::Path:
                {
                    std::unique_ptr<SpriteNode> tile(new SpriteNode(mTextures.get(Textures::Tiles), {0, 32, 32, 32}));
                    tile->setPosition(x * SPRITE_WIDTH, y * SPRITE_WIDTH);
                    mSceneLayers[Foreground]->attachChild(std::move(tile));
                    break;
                }
                case TT::Well:
                {
                    std::unique_ptr<WellNode> tile(new WellNode(mTextures));
                    tile->setPosition(x * SPRITE_WIDTH, y * SPRITE_WIDTH);
                    mSceneLayers[Foreground]->attachChild(std::move(tile));
                    break;
                }
            }
        }
    }

    // Add particle node to the scene
    {
        std::unique_ptr<ParticleNode> pnode(new ParticleNode(Particle::Default, mTextures));
        pnode->addAffector([](Particle &p, sf::Time dt) {
            p.position.x += 2 * std::cos(p.position.y * 0.05f);
            p.position.y -= 150.f * dt.asSeconds();
        });
        mSceneLayers[Foreground]->attachChild(std::move(pnode));
    }
    {
        std::unique_ptr<ParticleNode> pnode(new ParticleNode(Particle::Water, mTextures));
        pnode->addAffector([](Particle &p, sf::Time dt) {
            p.position.x += 2.f * std::cos(p.direction);
            p.position.y += p.speed * dt.asSeconds();
            p.speed += 75.f * 3 * dt.asSeconds();
        });
        mSceneLayers[Foreground]->attachChild(std::move(pnode));
    }
    {
        std::unique_ptr<ParticleNode> pnode(new ParticleNode(Particle::Growing, mTextures));
        pnode->addAffector([](Particle &p, sf::Time dt) {
            p.position.x += 2 * std::cos(p.position.y * 0.05f);
            p.position.y -= 150.f * dt.asSeconds();
        });
        mSceneLayers[Foreground]->attachChild(std::move(pnode));
    }

    // Player
    std::unique_ptr<PlayerEntity> player(new PlayerEntity(mTextures, mWaterCanCapacity));
    player->setOrigin(SPRITE_WIDTH/2, SPRITE_WIDTH/2);
    player->setPosition(6*SPRITE_WIDTH - SPRITE_WIDTH/2, 4*SPRITE_WIDTH - SPRITE_WIDTH/2);
    mPlayerEntity = player.get();
    mSceneLayers[Foreground]->attachChild(std::move(player));

    // Points
    std::unique_ptr<PointsNode> points(new PointsNode());
    mPointsNode = points.get();
    mSceneLayers[Foreground]->attachChild(std::move(points));
}

void World::update(sf::Time dt)
{
    // Game logic here
    int crop = (int)std::ceil(13 - (int)(mWaterCanCapacity)/10.f * 13);
    mWaterCan.setTextureRect({0, crop, 21, 13 - crop});
    mWaterCan.setPosition(SCREEN_WIDTH - 30*6, 100.f + 6*crop);

    if (mPointsNode->poll()) {
        mPoints.setString(toString(mPointsNode->points()) + "/130");
        if (mPointsNode->points() >= 130) {
            mGameWon = true;
        }
    }

    // Forward commands to scene
    while(!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }

    float halfSpriteW = SPRITE_WIDTH / 2;

    if (mPlayerEntity->getPosition().x < halfSpriteW) {
        mPlayerEntity->setPosition(halfSpriteW, mPlayerEntity->getPosition().y);
    } else if (mPlayerEntity->getPosition().x > MAP_WIDTH - halfSpriteW) {
        mPlayerEntity->setPosition(MAP_WIDTH - halfSpriteW, mPlayerEntity->getPosition().y);
    }
    if (mPlayerEntity->getPosition().y < halfSpriteW) {
        mPlayerEntity->setPosition(mPlayerEntity->getPosition().x, halfSpriteW);
    } else if (mPlayerEntity->getPosition().y > MAP_HEIGHT - halfSpriteW) {
        mPlayerEntity->setPosition(mPlayerEntity->getPosition().x, MAP_HEIGHT - halfSpriteW);
    }

    auto playerPos = mPlayerEntity->getPosition();
    float cameraX = std::min(std::max(SCREEN_WIDTH/2.f, playerPos.x), MAP_WIDTH-SCREEN_WIDTH/2);
    float cameraY = std::min(std::max(SCREEN_HEIGHT / 2.f, playerPos.y), MAP_HEIGHT - SCREEN_HEIGHT / 2);
    mWorldView.setCenter(cameraX, cameraY);

    // Set the listener position
    mSounds.setListenerPosition(mPlayerEntity->getWorldPosition());
    mSounds.removeStoppedSounds();

    mSceneGraph.update(dt, mCommandQueue);
}

void World::draw()
{
    if(PostEffect::isSupported())
    {
        mSceneTexture.clear();
        mSceneTexture.setView(mWorldView);
        mSceneTexture.draw(*mSceneLayers[Background]);
        mSceneTexture.draw(*mSceneLayers[Foreground]);
        mBloomEffect.apply(mSceneTexture, mWindow);
        mWindow.draw(*mSceneLayers[Ui]);
    }
    else
    {
        mWindow.setView(mWorldView);
        mWindow.draw(mSceneGraph);
    }

    mWindow.setView(mUiView);
    mWindow.draw(mWaterCanBackground);
    mWindow.draw(mWaterCan);
    mWindow.draw(mStar);
    mWindow.draw(mPoints);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}
