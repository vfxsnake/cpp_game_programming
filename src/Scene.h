#pragma once

#include <memory>
#include <map>


// forward declaration
class GameEngine;
class Action;
class EntityManager;

template <typename T>
class Vec2;  // vector class forward declaration and type defined with using.
using Vec2f = Vec2<float>;


using ActionMap = std::map<int, std::string>;


class Scene
{
protected:
    std::shared_ptr<GameEngine> m_game;
    EntityManager m_entityManager;
    ActionMap m_actionMap;
    bool m_pause = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

    virtual void onEnd() = 0;
    void setPaused(bool paused);

public:
    
    Scene() =  default;
    
    explicit Scene(std::shared_ptr<GameEngine> gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    virtual void doAction(const Action &action);
    void simulate(size_t frames);
    void registerAction(int inputKey, const std::string &actionName);

    
    [[nodiscard]] size_t width() const;
    [[nodiscard]] size_t height() const;
    [[nodiscard]] size_t currentFrame() const;
    [[nodiscard]] bool hasEnded() const;
    [[nodiscard]] const ActionMap &getActionMap() const;
    void drawLine(const Vec2f &p1, const Vec2f &p2);

};
