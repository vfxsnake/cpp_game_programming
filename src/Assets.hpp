#pragma once

#include<SFML/Graphics.hpp>
#include "Animation.hpp"

#include <cassert>
#include <iostream>
#include <fstream>

class Assets
{
private:
    std::map<std::string, sf::Texture> m_textureMap;
    std::map<std::string, Animation> m_animationMap;
    std::map<std::string, sf::Font> m_fontMap;

    void addTexture(const std::string& textureName, const std::string& path, bool smooth=true)
    {
        m_textureMap[textureName] = sf::Texture();
        if (!m_textureMap[textureName].loadFromFile(path))
        {
            std::cerr << "Could not load texture file: " << path << "\n" ;
            m_textureMap.erase(textureName);
        }

        else
        {
            m_textureMap[textureName].setSmooth(smooth);
            std::cout << "Loaded Texture" << path << "\n";
        }
    }

    void addAnimation(const std::string& animationName, const std::string& textureName, size_t frames, size_t speed)
    {
        m_animationMap[animationName] = Animation(animationName, textureName, frames, speed);
    }

    void addFont(const std::string& fontName, const std::string& path)
    {
        m_fontMap[fontName] = sf::Font();
        if (!m_fontMap[fontName].loadFromFile(path))
        {
            std::cerr << "Could not load font file: " << path << "\n";
            m_fontMap.erase(fontName);
        }

        else
        {
            std::cout << "Loaded Font: " << path << "\n";
        }
    }

public:
    Assets() = default;

    void loadFromFile(const std::string& path)
    {
        std::ifstream file(path);
        std::string str;
        while (file.good())
        {
            if (str == "Texture")
            {
                std::string name, path;
                file >> name >> path;
                addTexture(name, path);
            }

            else if (str == "Animation")
            {
                std::string name, texture;
                size_t frames, speed;
                file >> name >> texture >> frames >> speed;
                addAnimation(name, texture, frames, speed);
            }

            else if (str == "font")
            {
                std:: string name, path;
                file >> name >> path;
                addFont(name, path);
            }
            else
            {
                std::cerr << "Unknown Asset type: " << str << "\n";
            }
        }
    }

    const sf::Texture& getTexture(const std::string& textureName) const
    {
        assert(m_textureMap.find(textureName) != m_textureMap.end());
        return m_textureMap.at(textureName);
    }

    const Animation& getAnimation(std::string& animationName)
    {
        assert(m_animationMap.find(animationName) != m_animationMap.end());
        return m_animationMap.at(animationName);
    }

    const sf::Font& getFont(const std::string& fontName) const
    {
        assert(m_fontMap.find(fontName) != m_fontMap.end());
        return m_fontMap.at(fontName);

    }

    const std::map<std::string, sf::Texture>& getTextures()
    {
        return m_textureMap;
    }

    const std::map<std::string, Animation>& geAnimations() const
    {
        return m_animationMap;
    }
};