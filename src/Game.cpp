#define _USE_MATH_DEFINES
#include "Game.h"

#include <cmath>
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

    m_spawn_interval = m_enemyConfig.SI;

    if (!m_font.loadFromFile(fontPath))
    {
        std::cout << "unable to load font file" << fontPath << "\n";
        return;
    }

    ImGui::SFML::Init(m_window);

    // imgui scale by 2
    // ImGui::GetStyle().ScaleAllSizes(2.0f);
    // ImGui::GetIO().FontGlobalScale = 2.0f;

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
        sLifespan();
        sSpecialMove();
        sUserInput();
        sGUI();
        sRender();

        m_currentFrame++;
    }
}


void Game::spawnPlayer()
{
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
    entity->add<CSpecialMove>();
}


void Game::spawnEnemy()
{
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
    enemy->add<CScore>(number_of_points * 100);
    // record the last frame an enemy was spawned.
    m_lastEnemySpawnTime = m_currentFrame;
}


void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    if (e->tag() != "enemy")
    {
        return;
    }
    auto& transform =  e->get<CTransform>();
    auto circle = e->get<CShape>().circle;
    auto number_of_points = circle.getPointCount();  
    float angle = (M_PI * 2) / static_cast<float>(number_of_points);
    for (int i=0; i < number_of_points; i++)
    {
        auto small_enemy = m_entities.addEntity("smallEnemy");
        Vec2f velocity = Vec2f(std::sin(angle * i), std::cos(angle * i)) * transform.velocity.magnitude() / 2.0f;
        small_enemy->add<CTransform>(transform.pos, velocity, 0);
        small_enemy->add<CShape>(
                            circle.getRadius() / 2.0f,
                            number_of_points,
                            circle.getFillColor(),
                            circle.getOutlineColor(),
                            circle.getOutlineThickness()
                        );
        small_enemy->add<CLifespan>(60);
        small_enemy->add<CScore>(e->get<CScore>().score * 2);
    }

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
    auto& special_move = player()->get<CSpecialMove>();
    if (! special_move.charged)
        return;

    Vec2f position = entity->get<CTransform>().pos;
    auto weapon = m_entities.addEntity("special");
    weapon->add<CTransform>(
                        position, //position
                        Vec2f(0,0), // velocity
                        0.0f  //rotation
                    );
    weapon->add<CShape>( 
                m_bulletConfig.SR,
                m_bulletConfig.V,
                sf::Color(0, 0, 0, 255),
                sf::Color(255, 255, 255, 255),
                5
            );
    weapon->add<CLifespan>(m_bulletConfig.L/3.0f);
    special_move.energy = 0;
    special_move.charged = false;
}


void Game::sMovement()
{
    if (!s_movement_active)
    {
        return;
    }

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
    if (!s_lifespan_active)
    {
        return;
    }

    for (auto entity : m_entities.getEntities())
    {
        auto& life_span = entity->get<CLifespan>();
        if (!life_span.exists)
        {
            continue;
        }
        
        float alpha_color = 255 * (life_span.remaining / static_cast<float>(life_span.lifespan));
        if (alpha_color > 0.0f)
        {
            auto& circle = entity->get<CShape>().circle;
            sf::Color fill_color = circle.getFillColor();
            sf::Color out_line_color = circle.getOutlineColor();

            fill_color.a = alpha_color;
            out_line_color.a = alpha_color;
            circle.setFillColor(fill_color);
            circle.setOutlineColor(out_line_color);
        }

        else
        {
            entity->destroy();
        }

        life_span.remaining -= 1;
    }
}


void Game::sCollision()
{
    if (!s_collision_active)
    {
        return;
    }
    // bullet-enemy collisions
    for (auto bullet : m_entities.getEntities("bullet"))
    {
        auto bullet_transform = bullet->get<CTransform>();
        auto bullet_circle = bullet->get<CShape>().circle;

        for (auto enemy : m_entities.getEntities())
        {
            if (enemy->tag() == "player" || enemy->tag() == "bullet")
            {
                continue;
            }
            auto enemy_transform = enemy->get<CTransform>();
            auto enemy_circle = enemy->get<CShape>().circle;

            
            if (enemy_transform.pos.dist(bullet_transform.pos) < (enemy_circle.getRadius() + bullet_circle.getRadius()))
            {
                enemy->destroy();
                m_score += enemy->get<CScore>().score;
       
                bullet->destroy();
                spawnSmallEnemies(enemy);
            } 
        }
    }

    for (auto bullet : m_entities.getEntities("special"))
    {
        auto bullet_transform = bullet->get<CTransform>();
        auto bullet_circle = bullet->get<CShape>().circle;

        for (auto enemy : m_entities.getEntities("enemy"))
        {
            auto enemy_transform = enemy->get<CTransform>();
            auto enemy_circle = enemy->get<CShape>().circle;

            
            if (enemy_transform.pos.dist(bullet_transform.pos) < (enemy_circle.getRadius() + bullet_circle.getRadius()))
            {
                enemy->destroy();
                m_score += enemy->get<CScore>().score;
            } 
        }
    }

    for (auto enemy : m_entities.getEntities())
    {
        if (enemy->tag() == "player" || enemy->tag() == "bullet" || enemy->tag() == "special")
            {
                continue;
            }
        auto player_transform = player()->get<CTransform>();
        auto player_circle = player()->get<CShape>().circle;

        auto enemy_transform = enemy->get<CTransform>();
        auto enemy_circle = enemy->get<CShape>().circle;

        if (player_transform.pos.dist(enemy_transform.pos) < (player_circle.getRadius() + enemy_circle.getRadius()))
        {
            enemy->destroy();
            m_score += enemy->get<CScore>().score;
            spawnSmallEnemies(enemy);

            player()->get<CTransform>().pos = Vec2f(static_cast<float>(m_window.getSize().x) / 2.0f,  static_cast<float>(m_window.getSize().y) /2.0f) ;
        }
    }

}


void Game::sEnemySpawner()
{
    if (!s_spawning_active)
    {
        return;
    }

    if ((m_currentFrame - m_lastEnemySpawnTime) % m_spawn_interval == 0)
    {
        spawnEnemy();
    }

}


void Game::sGUI()
{
    // ImGui::ShowDemoWindow();
    ImGui::Begin("Geometry Wars");
     ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("tabs", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("System"))
        {
            bool test;
            ImGui::Checkbox("Movement System", &s_movement_active);
            ImGui::Checkbox("Lifespan System", &s_lifespan_active);
            ImGui::Checkbox("Collision system", &s_collision_active);
            ImGui::Checkbox("Spawning system", &s_spawning_active);
            ImGui::SliderInt("spawning interval", &m_spawn_interval, 5, 100);
            if (ImGui::Button("Manual Spawn"))
            {
                spawnEnemy();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Entity Manager"))
        {
            if (ImGui::CollapsingHeader("Entities"))
            {
                for (auto& [tag, entityVec] : m_entities.getEntityMap())
                {
                    ImGui::Indent();
                    if(ImGui::CollapsingHeader(tag.c_str()))
                    {
                        for (auto entity : entityVec)
                        {
                            ImGui::Indent();
                            std::string button_label = "D##" + std::to_string(entity->id());
                            sf::Color color = entity->get<CShape>().circle.getOutlineColor();
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color.r, color.g, color.b, color.a));
                            if(ImGui::Button(button_label.c_str()))
                            {
                                entity->destroy();
                            }
                            ImGui::PopStyleColor();
                            ImGui::SameLine();
                            ImGui::Text(entity->tag().c_str());
                            std::string  position = "(" + std::to_string(entity->get<CTransform>().pos.x) + ", " 
                                                        + std::to_string(entity->get<CTransform>().pos.y) + ")";
                            ImGui::SameLine();
                            ImGui::Text(position.c_str());
                            ImGui::Unindent();
                        }

                    }
                    ImGui::Unindent();
                }
            }

            if (ImGui::CollapsingHeader("All Entities", ImGuiTreeNodeFlags_None))
            {
                ImGui::Indent();
                for (auto entity : m_entities.getEntities())
                {
                    ImGui::Indent();
                    std::string button_label = "D##" + std::to_string(entity->id());
                    sf::Color color = entity->get<CShape>().circle.getOutlineColor();
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color.r, color.g, color.b, color.a));
                    if(ImGui::Button(button_label.c_str()))
                    {
                        entity->destroy();
                    }
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::Text(entity->tag().c_str());
                    std::string  position = "(" + std::to_string(entity->get<CTransform>().pos.x) + ", " 
                                                + std::to_string(entity->get<CTransform>().pos.y) + ")";
                    ImGui::SameLine();
                    ImGui::Text(position.c_str());
                    ImGui::Unindent();
                }
                ImGui::Unindent();
            }
            
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}


void Game::sRender()
{
    m_window.clear();
    for (auto entity : m_entities.getEntities())
    {
        entity->get<CShape>().circle.setPosition(entity->get<CTransform>().pos);
        entity->get<CTransform>().angle += 1.0f;
        entity->get<CShape>().circle.setRotation(entity->get<CTransform>().angle);
        m_window.draw(entity->get<CShape>().circle);
    }

    m_text.setFont(m_font);
    m_special_weapon.setFont(m_font);
    std::string score = "score: " + std::to_string(m_score);
    m_text.setString(score);
    m_window.draw(m_text);
    if (display_special_weapon)
    {
        m_special_weapon.setString("omni blast ready");
        m_special_weapon.setPosition(0.0, 25);
        m_window.draw(m_special_weapon);
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


void Game::sSpecialMove()
{
    for (auto& entity : m_entities.getEntities("special"))
    {
        auto& circle = entity->get<CShape>().circle;
        auto radius = circle.getRadius();
        circle.setRadius(radius + 10);
        circle.setOrigin(radius, radius); 
    }

    auto& special_move = player()->get<CSpecialMove>();
    special_move.energy += 1;
    if (special_move.energy > 120)
    {
        special_move.charged = true;
        display_special_weapon = true;
    }
    else
    {
        display_special_weapon = false;
    }
}