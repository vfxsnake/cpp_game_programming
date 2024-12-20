#pragma once
#include <map>
#include <memory>

#include <SFML/Graphics.hpp> 
#include "Assets.hpp"

// forward declarations
class Scene;


using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;


class GameEngine : public std::enable_shared_from_this<GameEngine>
{
private:
    sf::RenderWindow m_window;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;
    sf::Clock m_deltaClock;

    void init(const std::string& path);
    void update();
    void sUserInput();
    std::shared_ptr<Scene> currentScene();
    
public:

    GameEngine(const std::string& path);
    
    void changeScene(const std::string &path, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
    
    void quit();
    
    void run();
    
    sf::RenderWindow& window();

    const Assets& assets() const;

    bool isRunning();
};