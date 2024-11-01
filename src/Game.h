#pragma once

#include "EntityManager.hpp"
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"


struct PlayerConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S;};
struct EnemyConfig{int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMin, SMax;};
struct BulletConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S;};


class Game
{
private:
    sf::RenderWindow m_window;  // the window we will draw to.
    EntityManager m_entities;  //factory of entities 
    sf::Font m_font;  // the font we will use to draw.
    sf::Text m_text;  // the score text to be drawn to the screen.
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    sf::Clock m_deltaClock;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    int m_spawn_interval = 60;
    bool m_paused = false;
    bool m_running = true;

    bool s_movement_active = true;
    bool s_lifespan_active = true;
    bool s_collision_active = true;
    bool s_spawning_active = true;


    void init(const std::string &config);
    void setPaused(bool paused);

    void sMovement();
    void sUserInput();
    void sLifespan();
    void sRender();
    void sGUI();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

    std::shared_ptr<Entity> player();

public:
    Game(const std::string & config);
    void run();

};