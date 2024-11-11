#include "Scene_Play.h"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

Scene_Play::Scene_Play(std::shared_ptr<GameEngine> gameEngine, const std::string levelPath) : Scene(gameEngine), m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");

    // TODO register all other game play actionts.

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game.assets().getFont("Tech"));

    loadLevel(levelPath);
}

Vec2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // TODO: using the x a y grid position and an entity
    //  returns a vec2 indicating there the center position of the entity should be placed  

    return Vec2f(0.0, 0.0);
}

void Scene_Play::loadLevel(const std::string& filename)
{
    m_entityManager = EntityManager();

    spawnPlayer();

    auto brick = m_entityManager.addEntity("tile");
    brick->add<CAnimation>(m_game.assets().getAnimation("Brick"), true);
    brick->add<CTransform>(Vec2f(96, 480));
}


void Scene_Play::spawnPlayer()
{
    auto player = m_entityManager.addEntity("player");
    player->add<CAnimation>(m_game.assets().getAniamtion("Stand"), true);
    player->add<CTransform>(Vec2f(224, 352));
    player->add<CBoundingBox>(Vec2f(48, 48));
    player->add<CState>("stand");
    player->add<CInput>();

    // TODO: add remaining compoments to player
    // be sure to destroy the player if you are respawning 
}

