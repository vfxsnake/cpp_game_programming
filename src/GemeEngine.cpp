#include <iostream>
#include <utility>

#include "GameEngine.h"
#include "Action.hpp"
#include "Scene.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"
#include <imgui.h>
#include <imgui-SFML.h>


GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

void GameEngine::init(const std::string& path)
{
    m_assets.loadFromFile(path);
    m_window.create(sf::VideoMode(1280, 768), "Definitely not Mario");
    m_window.setFramerateLimit(60);

    ImGui::SFML::Init(m_window);

    // ImGui::GetStyle().ScaleAllSizes(2.0f);
    // ImGui::GetIO().FontGlobalScale = 2.0f;
    changeScene("Menu", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
    return m_running && m_window.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while (isRunning())
    {
        sUserInput();
        update();
        m_window.display();
    }
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(m_window, event);
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::X)
            {
                std::cout << "screenshot save to " << "test.png" << "\n";
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "screenshot saved to " << "test.png \n";
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) 
            {
                std::cout << event.key.code << "action not in the action map \n";
                continue;
            } 
            const std::string actionType = (event.type == sf::Event::EventType::KeyPressed ? "START" : "END");
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
}

void GameEngine::changeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = scene;
}

void GameEngine::quit()
{
    m_running = false;
    m_window.close();
}

void GameEngine::update()
{
    currentScene()->update();
}

const Assets& GameEngine::assets() const
{
    return m_assets;
}