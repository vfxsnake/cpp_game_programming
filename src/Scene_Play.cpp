#include "Scene_Play.h"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"
#include "Scene_Menu.h"
#include <imgui-SFML.h>
#include <imgui.h>
#include <fstream>

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string levelPath) 
        : Scene(gameEngine), m_levelPath(levelPath)
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

    // TODO register all other game play actions.

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Tech"));
    loadLevel(levelPath);
}

Vec2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    float tile_size = 64;
    float window_y_size = m_game->window().getSize().y;
    Vec2f animation_size = entity->get<CAnimation>().animation.getSize();
    return Vec2f((gridX * tile_size) + (animation_size.x / 2.0f), 
                 window_y_size - (gridY * tile_size) - (animation_size.y / 2));
}

void Scene_Play::loadLevel(const std::string& filename)
{
    // reseting entity Manager
    m_entityManager = EntityManager();

    // loading from file
    std::ifstream file(filename);
        if (!file)
        {
            std::cerr << "unable to load Level path \n" ;
        }
        std::string str;
        while (file >> str)
        {
            std::cout << "file current string: " << str << "\n";
            if (str == "Tile")
            {
                std::string name;
                float grid_x, grid_y;
                file >> name >> grid_x >> grid_y;
                auto entity = m_entityManager.addEntity("Tile");

                entity->add<CAnimation>(m_game->assets().getAnimation(name), true);
                entity->add<CTransform>(gridToMidPixel(grid_x, grid_y, entity));
            }

            else if (str == "Dec")
            {
                std::string name;
                float grid_x, grid_y;
                file >> name >> grid_x >> grid_y;
                auto entity = m_entityManager.addEntity("Dec");
                entity->add<CAnimation>(m_game->assets().getAnimation(name), true);
                entity->add<CTransform>(gridToMidPixel(grid_x, grid_y, entity));
            }

            else if (str == "Player")
            {
                
                file >> m_playerConfig.X 
                     >> m_playerConfig.Y
                     >> m_playerConfig.CX
                     >> m_playerConfig.CY
                     >> m_playerConfig.SPEED
                     >> m_playerConfig.JUMP
                     >> m_playerConfig.MAX_SPEED
                     >> m_playerConfig.GRAVITY
                     >> m_playerConfig.WEAPON
                     ;
            }
            else
            {
                std::cerr << "Unknown Element type: " << str << "\n";
            }
        }
    
    spawnPlayer();
}


void Scene_Play::spawnPlayer()
{
    m_player = m_entityManager.addEntity("Player");
    m_player->add<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    m_player->add<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player));
    m_player->add<CBoundingBox>(Vec2f(m_playerConfig.CX, m_playerConfig.CY));
    m_player->add<CState>("stand");
    m_player->add<CInput>();
    // TODO: add remaining compoments to player
    // be sure to destroy the player if you are respawning 
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
     // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

void Scene_Play::update()
{
    m_entityManager.update();

    // // TODO: implement pause functionality

    sMovement();
    SLifespan();
    sCollision();
    sAnimation();
    sGUI();
    sRender();
}

void Scene_Play::sMovement()
{
    // TODO: Implement player movement/jumping based on its CInput component
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/righ
}

void Scene_Play::SLifespan()
{
    // TODO: Check lifespan of entities the have them, and destroy them if the go over
}

void Scene_Play::sCollision()
{
// REMEMBER: SFML's (0,0) position is in the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will hava a y value GREATER than it
    //           Also, something ABOVE something else will hava a y value LESS than it

    // TODO: Implement Physics::GetOverlap() function, use it inside this function

    // TODO: Implement bullet/tile collisions
    //       Destroy the tile if it has a Brick animation
    // TODO: Implement player/tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is currently on the ground or in the air. This will be
    //       used by the Animation system
    // TODO: Check to see if the player has fallen down a hole (y > height())
    // TODO: Don't let the player walk off the left side of the map

}

void Scene_Play::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_COLLISION")
        {
            m_drawCollision = !m_drawCollision;
        }
        else if (action.name() == "TOGGLE_GRID")
        {
            m_drawGrid = !m_drawGrid;
        }
        else if (action.name() == "PAUSE")
        {
            setPaused(!m_paused); // this comes from the base class
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
        else if (action.name() == "JUMP")
        {
            // m_player jump
        } 
    }
    else if (action.type() == "END")
    {

    }
}

void Scene_Play::sAnimation()
{
    // TODO: Complete the Animation class code first
}

void Scene_Play::onEnd()
{
    // TODO: when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
    m_game->changeScene( "MENU", std::make_shared<Scene_Menu>(m_game),true);
}

void Scene_Play::sRender()
{
    if(!m_paused)
    {
        m_game->window().clear(sf::Color(100, 100, 255));
    }

    else
    {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    auto &pPos = m_player->get<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);
    
    // draw sprite textures
    if (m_drawTextures)
    {
        for (const auto& e : m_entityManager.getEntities())
        {
            auto& transform = e->get<CTransform>();
            if (e->has<CAnimation>())
            {
                auto& animation = e->get<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
        }
    }

    // draw collision bounding boxes as rectangular shapes
    if (m_drawCollision)
    {
        for (const auto& e : m_entityManager.getEntities())
        {
            if (e->has<CBoundingBox>())
            {
                auto& box = e->get<CBoundingBox>();
                auto& transform = e->get<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2(box.halfSize.x , box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0,0,0,0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    // draw the game tile grid for debugging
    if (m_drawGrid)
    {
        float leftX = m_game->window().getView().getCenter().x - width() / 2.0;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        // draw vertical lines
        for (float x= nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2f(x, 0), Vec2f(x, height()));
        }
        // draw horizontal lines
        for (float y = 0; y < height(); y += m_gridSize.y)
        {
            drawLine(Vec2f(leftX, height() - y), Vec2f(rightX, height() - y));

            // draw coordinates
            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + ", " + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }
}

void Scene_Play::sGUI()
{

    ImGui::Begin("Geometry Wars");
     ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("tabs", tab_bar_flags))
    {
       if (ImGui::BeginTabItem("Systems"))
        {
            ImGui::EndTabItem();
        } 

    if (ImGui::BeginTabItem("Entity Manager"))
        {
            if (ImGui::CollapsingHeader("Entities"))
            {
                for (auto& [tag, entityVec] : m_entityManager.getEntityMap())
                {
                    ImGui::Indent();
                    if(ImGui::CollapsingHeader(tag.c_str()))
                    {
                        for (auto entity : entityVec)
                        {
                            ImGui::Indent();
                            std::string button_label = "D##" + std::to_string(entity->id());
                            if(ImGui::Button(button_label.c_str()))
                            {
                                entity->destroy();
                            }
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

            ImGui::EndTabItem();
        } 

        ImGui::EndTabBar();
    }
    ImGui::End();
}
