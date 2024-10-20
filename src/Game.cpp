#include "Game.h"

#include <iostream>

Game::Game(const std::string &config)
{
    init(config);
}

void Game::init(const std::string &path)
{
    // TODO: read in config file here. 

    // window parameters

    m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
    m_window.setFramerateLimit(60);

    ImGui::SFML::Init(m_window);

    // imgui scale by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

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
    auto entity = m_entities.addEntity("player");

    // adding transform component to the player
    entity->add<CTransform>(Vec2(200.f, 200.0f), Vec2f(1.0f, 1.0f), 0.0f);

    entity->add<CShape>(32.0f, 8, sf::Color(10,10,10), sf::Color(255, 0, 0), 4.0f);

    entity->add<CInput>();

}


void Game::spawnEnemy()
{
    // TODO: enemies should be spawned with in the boundaries of the screen and withe the config settings.

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
    // TODO: implement bullet spawn.
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    // TODO: implement you own component here
}

void Game::sMovement()
{
    // TODO: implement all entity movements in this function
    // ** sample movement speed update
    auto& transform = player()->get<CTransform>();
    transform.pos += transform.velocity;
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
    player()->get<CShape>().circle.setPosition(player()->get<CTransform>().pos);
    player()->get<CTransform>().angle += 1.0f;
    player()->get<CShape>().circle.setRotation(player()->get<CTransform>().angle);

    m_window.draw(player()->get<CShape>().circle);

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
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                std::cout << "W key pressed\n";
                // TODO: set players input component "up" to true
                break;
            
            default:
                break;
            }

        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                std::cout << "W key released\n";
                // TODO: set players input component "up" to false
                break;
            
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
                // calling spawn bullet here
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "right Mouse Button clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")"  <<"\n";
                // call spawn Special Weapon here.
            }


        }

    }
    
}