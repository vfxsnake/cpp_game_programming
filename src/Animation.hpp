#pragma once

#include <string>
#include "Vec2.hpp"

class Animation
{
private:
    sf::Sprite m_sprite;
    size_t m_frameCount = 1;
    size_t m_currentFrame = 0;
    size_t m_speed = 0;
    Vec2f m_size = Vec2f(1, 1);
    std::string m_name = "none";


public:
    Animation() = default;

    Animation(const std::string &name, const sf::Texture &t) :
            Animation(name, t, 1, 0){ }
    
    Animation(const std::string& name, const sf::Texture &t, size_t frameCount, size_t speed ): 
            m_name(name), m_sprite(t), m_frameCount(frameCount), m_speed(speed), m_currentFrame(0)  
    { 
        m_size = Vec2f((float)t.getSize().x / frameCount, (float)t.getSize().y);
        m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
        m_sprite.setTextureRect(sf::IntRect((int)floor(float(m_currentFrame) * m_size.x), 0, (int)m_size.x, (int)m_size.y));
    }

    void update()
    {
        if (m_speed > 0 && m_frameCount > 1)
        {
            int update_rate = (m_currentFrame / m_speed) % m_frameCount;
            m_sprite.setTextureRect(sf::IntRect((int)floor(float(update_rate) * m_size.x), 0, (int)m_size.x, (int)m_size.y));
            m_currentFrame++;
        }
    }

    bool hasEnded() const
    {
        if (m_currentFrame > m_frameCount)
        {
            return true;
        }
        return false;
    }

    const std::string& getName() const
    {
        return m_name;
    }

    const Vec2f &getSize() const
    {
        return m_size;
    }

    sf::Sprite &getSprite()
    {
        return m_sprite;
    } 
    
};