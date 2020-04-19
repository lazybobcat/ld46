#include <world.hpp>
#include <iostream>
#include <resources/particle.hpp>
#include <entities/particlenode.hpp>
#include <cmath>
#include <algorithm>
#include "../field_node.hpp"
#include "../player_entity.hpp"
#include "../well_node.hpp"

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
    mWorldView(window.getDefaultView()),
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
//    mWorldView.setViewport({0, 0.05f, 1.f, 0.9f});
}


void World::loadTextures()
{

}

void World::buildScene()
{
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
    std::unique_ptr<PlayerEntity> player(new PlayerEntity(mTextures));
    player->setOrigin(SPRITE_WIDTH/2, SPRITE_WIDTH/2);
    player->setPosition(6*SPRITE_WIDTH - SPRITE_WIDTH/2, 4*SPRITE_WIDTH - SPRITE_WIDTH/2);
    mPlayerEntity = player.get();
    mSceneLayers[Foreground]->attachChild(std::move(player));

    // Ennemies
    // ...
}

void World::update(sf::Time dt)
{
    // Game logic here


    // Forward commands to scene
    while(!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }

    // Set the listener position
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
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}
