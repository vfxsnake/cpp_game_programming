#include <memory>
#include "Scene_Menu.h"
#include "Scene_play.h"
#include "Assets.hpp"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

Scene_Menu::Scene_Menu(std::shared_ptr<GameEngine> gameEngine) 
        : Scene(gameEngine) // this is why it uses explicit
{
    init();
}

void Scene_Menu::init()
{
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "PLAY");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_title = "Mega Mario";
    int titleSize = 30;
    m_menuText.setString(m_title);
    m_menuText.setFont(m_game->assets().getFont("Mario"));
    m_menuText.setCharacterSize(titleSize);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(
            float(m_game->window().getSize().x / 2.0f - float(titleSize * m_title.length() + 1)) / 2.0f,
            float(titleSize * 3)
            );


    m_menuStrings.push_back("level  1");
    m_menuStrings.push_back("level  2");
    m_menuStrings.push_back("level  3");

    m_levelPaths.push_back("level1.txt");
    m_levelPaths.push_back("level2.txt");
    m_levelPaths.push_back("level3.txt");

    for (int i = 0; i < m_menuStrings.size(); i++)
    {
        sf::Text text(m_menuStrings[i], m_game->assets().getFont("Megaman"), 26);
        if (m_selectedMenuIndex != i)
        {
            text.setFillColor(sf::Color::Black);
        }
        else
        {
            text.setFillColor(sf::Color::White);
        }

        text.setPosition(
            float(m_game->window().getSize().x) * 0.5f - float(26 * (m_menuStrings[i].length() + 1)) * 0.5f,
            m_menuText.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1) 
        );
        m_menuItems.push_back(text);
    }

}

void Scene_Menu::update()
{
    m_entityManager.update();
    // sRender(); // ?? should this be here??
}

void Scene_Menu::onEnd()
{
    m_game->quit();
}

void Scene_Menu::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (m_selectedMenuIndex > 0)
            {
                m_selectedMenuIndex--;
            }
            else
            {
                m_selectedMenuIndex = m_menuStrings.size() -1;
            }
        }

        if (action.type() == "DOWN")
        {
            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
        }

        else if (action.name() == "PLAY")
        {
            m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
    
        }

        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}

void Scene_Menu::sRender()
{
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear(sf::Color(100, 100, 255));

    m_menuText.setCharacterSize(48);
    m_menuText.setString(m_title);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(sf::Vector2f(10,10));
    m_game->window().draw(m_menuText);

    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::Black : sf::Color::White);
        m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
        m_game->window().draw(m_menuText);
    }

    m_menuText.setCharacterSize(20);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setString("up: w   down: s    play:  d  ");
}