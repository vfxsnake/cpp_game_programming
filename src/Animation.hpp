#pragma once

#include <string>

class Animation
{
private:
    std::string m_name;
    std::string m_textureName;
    size_t m_frames = 0;
    size_t m_speed = 0;
    int m_currentFrame=0;

public:
    Animation() = default;
    
    Animation(const std::string& name, const std::string textureName, size_t frames, size_t speed ): 
            m_name(name), m_textureName(textureName), m_frames(frames), m_speed(speed)  {}

    void update()
    {
        m_currentFrame++;
        // TODO: calculate the correct frame of the animation based on the current frame
        // set the texture rectangle properly
    }

    bool hasEnded() const
    {
        // detect when aniamtion has ended (last frame was played) and return true.
        return false;
    }

    const std::string& getName() const
    {
        return m_name;
    }

};