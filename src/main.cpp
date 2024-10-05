#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"


struct Circle
{
    /* circle parameters */
    Circle(std::string name,
           float radius, 
           int segments, 
           sf::Vector2f position,
           sf::Vector2f velocity,
           sf::Color color)
    {
        Name = name;
        Shape.setRadius(radius);
        Shape.setPointCount(segments);
        Shape.setPosition(position);
        Shape.setFillColor(color);
        Velocity = velocity;
        Draw = true;
    }

    sf::CircleShape Shape;
    std::string Name;
    sf::Vector2f Velocity;
    bool Draw;
};

struct Rectangle
{
    /* rectangle parameters */
    Rectangle(std::string name,
              sf::Vector2f size, 
              sf::Vector2f position,
              sf::Vector2f velocity,
              sf::Color color
              )
    {
        Name = name;
        Shape.setSize(size);
        Shape.setPosition(position);
        Shape.setFillColor(color);
        Velocity = velocity;
        Draw = true;
    }
    
    sf::RectangleShape Shape;
    std::string Name;
    sf::Vector2f Velocity;
    bool Draw;

};



void ReadConfig(const std::string& configFile, 
                int &windowWidth, 
                int &windowHeight,
                std::string &fontPath,
                int &fontSize,
                int &fontColorR,
                int &fontColorG,
                int &fontColorB,
                std::vector<Circle> &circles,
                std::vector<Rectangle> &rectangles 
                )
{
    std::ifstream fileIn(configFile);
    std::string objectType;
    while (fileIn >> objectType)
    {
        if (objectType == "Window")
        {
            fileIn >> windowWidth >> windowHeight;
        }

        if (objectType == "Font")
        {
            fileIn >> fontPath
                   >> fontSize
                   >> fontColorR
                   >> fontColorG
                   >> fontColorB;
        } 

        if (objectType == "Circle")
        {
            std::string name;
            sf::Vector2f position;
            sf::Vector2f velocity;
            int iColor[3];
            float radius;
            fileIn >> name
                   >> position.x 
                   >> position.y 
                   >> velocity.x 
                   >> velocity.y
                   >> iColor[0] 
                   >> iColor[1] 
                   >> iColor[2]
                   >> radius;
            sf::Color color{uint8_t(iColor[0]), uint8_t(iColor[1]), uint8_t(iColor[2])};
            Circle c(name, radius, 32, position, velocity, color);
            circles.push_back(c); 
        }

        if (objectType == "Rectangle")
        {
            std::string name;
            sf::Vector2f position;
            sf::Vector2f velocity;
            int iColor[3];
            sf::Vector2f size;

            fileIn >> name
                   >> position.x
                   >> position.y
                   >> velocity.x
                   >> velocity.y
                   >> iColor[0] 
                   >> iColor[1] 
                   >> iColor[2]
                   >> size.x
                   >> size.y;
            sf::Color color{uint8_t(iColor[0]), uint8_t(iColor[1]), uint8_t(iColor[2])};
            Rectangle r(name, size, position, velocity, color);
            rectangles.push_back(r);
        }
    }

}


int main(int argc, char* argv[])
{
    int width, height;
    std::string fontPath;
    int fontSize;
    int fontColor[3];
    std::vector<Circle> circles;
    std::vector<Rectangle> rectangles;
    ReadConfig("config.txt", width, height, fontPath, fontSize, fontColor[0], fontColor[1], fontColor[2], circles, rectangles);

    // create a new window of W,H
    const int windowWidth = width; // read from config 
    const int windowHeight = height; // read from config
    
    // creating the global font
    sf::Font myFont;
    if (!myFont.loadFromFile(fontPath))
    {
        std::cerr << "could not load font!\n";
        exit(-1);
    }


    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Game Window");
    window.setFramerateLimit(60);
    
    // initialize ImgGui
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    /*
    // in case of need this will scale the entirely imgui widget.
    ImGui::GetStyle().ScaleAllSizes(2.0f); 
    ImGui::GetIO().FontGlobalScale = 2.0; 
    */

    // main loop
    while (window.isOpen())
    {
        /////////// events processing
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code: " << event.key.code << "\n";
            }
        }

        ///////// imgui updates ////////////
        ImGui::SFML::Update(window, deltaClock.restart());
        
        // draw the ui
        ImGui::ShowDemoWindow();
        ImGui::Begin("Window title");
        ImGui::Text("Contro Window");
        // ImGui::Checkbox("Draw Circle", &circle.Draw);
        // ImGui::SameLine();
        // ImGui::Checkbox("Draw Text", &drawText);
        // ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
        // ImGui::SliderInt("Sides", &circleSegments, 3, 64);
        // ImGui::ColorEdit3("Color Circle", c);
        // ImGui::InputText("Text", displayString, 255);
        // if(ImGui::Button("set Text"))
        // {
        //     text.setString(displayString);
        // }
        // ImGui::SameLine();
        // if (ImGui::Button("Reset Circle"))
        // {
        //     circle.Shape.setPosition(0.0f,0.0f);
        // }
        ImGui::End();

        // update shape values
        // circle.Shape.setPointCount(circleSegments);
        // circle.Shape.setRadius(circleRadius);
        // circle.Shape.setFillColor(sf::Color(sf::Uint8(c[0]*255), sf::Uint8(c[1]*255), sf::Uint8(c[2]*255)));
        // circle.Shape.setPosition(circle.Shape.getPosition().x + circle.Velocity.x, circle.Shape.getPosition().y + circle.Velocity.y);


        window.clear();
    
        for (Circle& c : circles)
        {
            // update velocity if positionis out of bounce
            if (c.Shape.getPosition().x < 0 || c.Shape.getPosition().x + (c.Shape.getRadius() * 2) > windowWidth)
            {
                c.Velocity.x *= -1.0f;
            }
            if (c.Shape.getPosition().y < 0 || c.Shape.getPosition().y + (c.Shape.getRadius() * 2) > windowHeight)
            {
                c.Velocity.y *= -1.0f;
            }

            c.Shape.setPosition(c.Shape.getPosition() + c.Velocity);
            sf::Text text(c.Name, myFont, fontSize);
            sf::Vector2f textCenter = text.getGlobalBounds().getSize() / 2.0f;
            text.setPosition(c.Shape.getPosition().x + c.Shape.getRadius() - textCenter.x, 
                             c.Shape.getPosition().y + c.Shape.getRadius() - textCenter.y );
            char displayString[255] = "sample Text";
            window.draw(c.Shape);
            window.draw(text);
        }


        for (Rectangle& r : rectangles)
        {
            // update velocity if positionis out of bounce
            if (r.Shape.getPosition().x < 0 || r.Shape.getPosition().x + r.Shape.getSize().x > windowWidth)
            {
                r.Velocity.x *= -1.0f;
            }
            if (r.Shape.getPosition().y < 0 || r.Shape.getPosition().y + r.Shape.getSize().y > windowHeight)
            {
                r.Velocity.y *= -1.0f;
            }
            r.Shape.setPosition(r.Shape.getPosition() + r.Velocity);
            sf::Text text(r.Name, myFont, fontSize);
            sf::Vector2f textCenter = text.getGlobalBounds().getSize() / 2.0f;
            text.setPosition(r.Shape.getPosition().x + (r.Shape.getSize().x / 2.0f) - textCenter.x, 
                             r.Shape.getPosition().y + (r.Shape.getSize().y / 2.0f) - textCenter.y - fontSize * 0.25 );
            window.draw(r.Shape);
            window.draw(text);
        }
        // if (drawText)
        // {
        //     window.draw(text);
        // }

        ImGui::SFML::Render(window);
        window.display();
    } 
    
    return 0;
}