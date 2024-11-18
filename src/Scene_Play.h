#pragma once

#include <map>
#include <memory>

#include "Scene.h"
#include "EntityManager.hpp"

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

protected:

    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = true;
    const Vec2f m_gridSize = {64, 64};
    sf::Text m_gridText;

    void init(const std::string& levelPath);
    
    Vec2f gridToMidPixel(float x, float y, std::shared_ptr<Entity> entity);

    void loadLevel(const std::string& filename);
    
    void spawnPlayer();
    
    void spawnBullet(std::shared_ptr<Entity> entity);

    void sMovement();
    
    void SLifespan();
    
    void sCollision();

    void sAnimation();
    
    void sRender() override;
    
    void sDoAction(const Action& action) override;
    
    void onEnd() override;


public:
    
    Scene_Play(GameEngine* gameEngine, const std::string levelPath);
    
    void update();
    
    std::shared_ptr<Entity> player();

};