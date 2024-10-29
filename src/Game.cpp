#include "Game.h"

#include <iostream>
#include <fstream>

Game::Game(const std::string &config)
{
    init(config);
}

void Game::init(const std::string &path)
{
    int width, height, maxFPS;
    bool fullScreenMode;
    std::string fontPath;
    int fontColor[3];

    std::ifstream fileIn(path);
    std::string objectType;
    while (fileIn >> objectType)
    {
        if (objectType == "Window")
        {
            fileIn >> width
                   >> height
                   >> maxFPS
                   >> fullScreenMode;
        }

        if (objectType == "Font")
        {
            fileIn >> fontPath
                   >> fontColor[1]
                   >> fontColor[2]
                   >> fontColor[3]; 
        }

        if (objectType == "Player")
        {
            fileIn >> m_playerConfig.SR
                   >> m_playerConfig.CR
                   >> m_playerConfig.S
                   >> m_playerConfig.FR       
                   >> m_playerConfig.FG       
                   >> m_playerConfig.FB
                   >> m_playerConfig.OR       
                   >> m_playerConfig.OG       
                   >> m_playerConfig.OB
                   >> m_playerConfig.OT       
                   >> m_playerConfig.V;       
        }

        if (objectType == "Enemy")
        {
            fileIn >> m_enemyConfig.SR
                   >> m_enemyConfig.CR
                   >> m_enemyConfig.SMin
                   >> m_enemyConfig.SMax
                   >> m_enemyConfig.OR
                   >> m_enemyConfig.OG
                   >> m_enemyConfig.OB
                   >> m_enemyConfig.OT
                   >> m_enemyConfig.VMIN
                   >> m_enemyConfig.VMAX
                   >> m_enemyConfig.L
                   >> m_enemyConfig.SI;
        }
        
        if (objectType == "Bullet")
        {
            fileIn >> m_bulletConfig.SR
                   >> m_bulletConfig.CR
                   >> m_bulletConfig.S
                   >> m_bulletConfig.FR
                   >> m_bulletConfig.FG
                   >> m_bulletConfig.FB
                   >> m_bulletConfig.OR
                   >> m_bulletConfig.OG
                   >> m_bulletConfig.OB
                   >> m_bulletConfig.OT
                   >> m_bulletConfig.V
                   >> m_bulletConfig.L;
        }

    }
    
    // window parameters
    m_window.create(sf::VideoMode(width, height), "Assignment 2");
    m_window.setFramerateLimit(maxFPS);

    ImGui::SFML::Init(m_window);

    // imgui scale by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    // global random seed.
    srand(time(NULL));
    spawnPlayer();
}

std::shared_ptr<Entity> Game::player()
{
    auto& players = m_entities.getEntities("player");
    assert(players.size() == 1);
    return players.front();
}

void Game::run()
{
    // TODO: add pause functionality in here, render system should be kept alive.
    while(m_running)
    {
        // update the entity manager
        m_entities.update();

        // imgui update
        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        sEnemySpawner();
        sMovement();
        sCollision();
        sUserInput();
        sGUI();
        sRender();

        m_currentFrame++;
    }
}


void Game::spawnPlayer()
{
    // TODO: Finish adding all properties fo the player with the correct values 
    // getting the player form the entity manager.
    Vec2 win_size = m_window.getSize();

    auto entity = m_entities.addEntity("player");

    // adding transform component to the player
    entity->add<CTransform>(Vec2f(win_size.x / 2.0f, win_size.y / 2.0f), Vec2f(1.0f, 1.0f), 0.0f);

    entity->add<CShape>(
            m_playerConfig.SR, 
            m_playerConfig.V, 
            sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), 
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
            m_playerConfig.OT
        );

    entity->add<CInput>();
}


void Game::spawnEnemy()
{
    // TODO: enemies should be spawned with in the boundaries of the screen and withe the config settings.

    Vec2 win_size = m_window.getSize();
    float max_spawn_threshold_position_x = win_size.x - m_enemyConfig.SR;
    float max_spawn_threshold_position_y = win_size.y - m_enemyConfig.SR;

    float rand_position_x = m_enemyConfig.SR + (rand() % static_cast<int>(max_spawn_threshold_position_x - m_enemyConfig.SR + 1));
    float rand_position_y = m_enemyConfig.SR + (rand() % static_cast<int>(max_spawn_threshold_position_y - m_enemyConfig.SR + 1) );

    int number_of_points = m_enemyConfig.VMIN + (rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1));
    float rotation = rand() % 45;
    float velocity_x = (rand() % 100) -50;
    float velocity_y = (rand() % 100) -50;
    float velocity_scale = m_enemyConfig.SMin + (rand() % static_cast<int>(m_enemyConfig.SMax - m_enemyConfig.SMin + 1));
    auto enemy = m_entities.addEntity("enemy");
    Vec2f velocity = Vec2(velocity_x, velocity_y).normalize() * velocity_scale;
    enemy->add<CTransform>(Vec2f(rand_position_x, rand_position_y), velocity, rotation);
    enemy->add<CShape>( 
                m_enemyConfig.SR,
                number_of_points,
                sf::Color(0, 0, 0),
                sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
                m_enemyConfig.OT
            );
    // record the last frame an enemy was spawned.
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies from the location of the input enemy e
    //  number of small enemies should be the number of sides of the input enemy
    //  use the color from the input enemy
    //  score fo the small enemies should be double of the shape score.

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &target)
{
    Vec2f position = entity->get<CTransform>().pos;
    Vec2f direction = (target - position).normalize();
    auto bullet = m_entities.addEntity("bullet");
    bullet->add<CTransform>(
                        position + direction * entity->get<CShape>().circle.getRadius(), //position
                        direction * m_bulletConfig.S, // velocity
                        0.0f  //rotation
                    );
    bullet->add<CShape>( 
                m_bulletConfig.SR,
                m_bulletConfig.V,
                sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
                m_bulletConfig.OT
            );
    bullet->add<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    // TODO: implement you own component here
}

void Game::sMovement()
{
    Vec2 win_size = m_window.getSize();
    for (auto entity : m_entities.getEntities())
    {
        // player movement logic
        auto& transform = entity->get<CTransform>();
        auto& circle =  entity->get<CShape>().circle;
        if (entity->tag() == "player")
        {
            unsigned int window_right_boundary = win_size.x - circle.getRadius() - circle.getOutlineThickness();
            unsigned int window_low_boundary = win_size.y - circle.getRadius() - circle.getOutlineThickness();
            auto& transform = player()->get<CTransform>();
            auto& player_input = player()->get<CInput>();
            if (player_input.left && transform.pos.x > (circle.getRadius() + circle.getOutlineThickness()))
            {
                transform.velocity.x = -1;
            }
            else if (player_input.right && transform.pos.x < window_right_boundary)
            {
                transform.velocity.x = 1;
            }
            else 
            {
                transform.velocity.x = 0;
            }

            if (player_input.up && transform.pos.y > (circle.getRadius() + circle.getOutlineThickness()))
            {
                transform.velocity.y = -1;
            }
            else if (player_input.down && transform.pos.y < window_low_boundary)
            {
                transform.velocity.y = 1;
            }
            else
            {
                transform.velocity.y = 0;
            }

            if (transform.velocity.magnitude() != 0)
            {  
                transform.velocity = transform.velocity.normalize()  * m_playerConfig.S;
            }
        }

        else
        {
            if (transform.pos.x <= circle.getRadius() + circle.getOutlineThickness()||
                transform.pos.x >= win_size.x - circle.getRadius() - circle.getOutlineThickness())
            {
                transform.velocity.x *= -1;
            }

            if (transform.pos.y <= circle.getRadius() + circle.getOutlineThickness()||
                transform.pos.y >= win_size.y - circle.getRadius() - circle.getOutlineThickness())
            {
                transform.velocity.y *= -1;
            }

        }

        transform.pos += transform.velocity;
    }
}

void Game::sLifespan()
{
    // TODO: implement lifespan functionality.
    // for all entities if no valid life spawn skip it,
    //                  if lifespan > 0 subtract 1
    //                  cage alpha channel accordingly
    //                  if lifespan has ended destroy the instance.
}

void Game::sCollision()
{
    // implement collisions
    for (auto b : m_entities.getEntities("bullet"))
    {
        for (auto e : m_entities.getEntities("enemy"))
        {
            
        }

        for (auto e : m_entities.getEntities("smallEnemies"))
        {

        }
    }
}

void Game::sEnemySpawner()
{
    // TODO: implement enemy spawning 
    if ((m_currentFrame - m_lastEnemySpawnTime) % m_enemyConfig.SI == 0)
    {
        spawnEnemy();
    }

}

void Game::sGUI()
{
    ImGui::Begin("Geometry Wars");
    ImGui::Text("stuff goes here");
    ImGui::End();
}

void Game::sRender()
{
    // TODO: change the code bellow to draw all entities

    m_window.clear();
    for (auto entity : m_entities.getEntities())
    {
        entity->get<CShape>().circle.setPosition(entity->get<CTransform>().pos);
        entity->get<CTransform>().angle += 1.0f;
        entity->get<CShape>().circle.setRotation(entity->get<CTransform>().angle);
        m_window.draw(entity->get<CShape>().circle);
    }

    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::sUserInput()
{

    // TODO: handle user input.
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(m_window, event);
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            auto& c_input = player()->get<CInput>();
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                // TODO: set players input component "up" to true
                c_input.up = true;
                break;
            case sf::Keyboard::S:
                c_input.down = true;
                break;
            case sf::Keyboard::A:
                c_input.left = true;
                break;
            case sf::Keyboard::D:
                c_input.right = true;
                
            default:
                break;
            }

        }

        if (event.type == sf::Event::KeyReleased)
        {
            auto& c_input = player()->get<CInput>();
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                c_input.up = false;
                break;
            case sf::Keyboard::S:
                c_input.down = false;
                break;
            case sf::Keyboard::A:
                c_input.left = false;
                break;
            case sf::Keyboard::D:
                c_input.right = false;
            default:
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            // this line overrides functionality when clicking in the gui
            if (ImGui::GetIO().WantCaptureMouse)
            {
                continue;
            }

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "left Mouse button clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << "\n";
                spawnBullet(player(), Vec2f(event.mouseButton.x, event.mouseButton.y));
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "right Mouse Button clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")"  <<"\n";
                spawnSpecialWeapon(player());
            }


        }

    }
    
}