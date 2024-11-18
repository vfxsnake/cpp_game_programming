#include "Scene_Play.h"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

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
    // TODO: This function takes in a grid (x,y) position and an Entity
    //       Return a vec2 indicating where the CENTER position of the Entity should be
    //       You must use the Entity's Animation size to position it correctly
    //       The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    //       The bottom-left corner of the Animation should aligh with the bottom left of the grid cellced  

    return Vec2f(0.0, 0.0);
}

void Scene_Play::loadLevel(const std::string& filename)
{
    m_entityManager = EntityManager();

    spawnPlayer();

    auto brick = m_entityManager.addEntity("tile");
    brick->add<CAnimation>(m_game->assets().getAnimation("Brick"), true);
    brick->add<CTransform>(Vec2f(96, 480));

    if (brick->get<CAnimation>().animation.getName() == "Brick")
    {
        std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    }

    auto block = m_entityManager.addEntity("tile");
    block->add<CAnimation>(m_game->assets().getAnimation("Ground"), true);
    block->add<CTransform>(Vec2f(128, 480));
    block->add<CBoundingBox>(m_game->assets().getAnimation("Ground").getSize());




    auto question = m_entityManager.addEntity("tile");
    question->add<CAnimation>(m_game->assets().getAnimation("Ground"), true);
    question->add<CTransform>(Vec2f(450, 480));


    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    //       Components are returned as references
    //       If you do not specify a reference variable type, it will COPY the component
    //       Here is an example:
    //
    //       This will COPY the transform into the variable 'transform1' - it is INCORRECT
    //       Any changes you make to transform1 will not be changed inside the entity
    //       auto transform1 = entity->get<CTransform>()
    //
    //       This will REFERENCE the transform with the variable 'transform2' - it is CORRECT
    //       Now any changes you make to transform2 will be changed inside the entity
    //       auto& transform2 = entity->get<CTransform>()
}


void Scene_Play::spawnPlayer()
{
    auto player = m_entityManager.addEntity("player");
    player->add<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    player->add<CTransform>(Vec2f(224, 352));
    player->add<CBoundingBox>(Vec2f(48, 48));
    player->add<CState>("stand");
    player->add<CInput>();
    // TODO: add remaining compoments to player
    // be sure to destroy the player if you are respawning 

    std::cout << "end spawning player \n";
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
    // m_game->changeScene( "MENU", std::make_shared<Scene_Menu>(m_game));
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

    // temporary draw of player
    auto &pPos = m_player->get<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    // sf::View view = m_game->window().getView();
    // view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    // m_game->window().setView(view);

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