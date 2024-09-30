#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"


int main(int argc, char* argv[])
{
    // create a new window of W,H
    const int windowWidth = 1280; // read from config 
    const int windowHeight = 720; // read from config
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Game Window");
    window.setFramerateLimit(60);
    
    // intitialize ImgGui
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    /*
    // in case of need this will scale the entirely imgui widget.
    ImGui::GetStyle().ScaleAllSizes(2.0f); 
    ImGui::GetIO().FontGlobalScale = 2.0; 
    */

    // temp color rgb range 0,1
    float c[3] = {0.0f, 1.0f, 1.0f};

    // temp shape 
    float circleRadius = 50;
    int circleSegments = 32;
    float circleSpeedX = 1.0f;
    float circleSpeedY = 0.5;
    bool drawCircle = true;
    bool drawText = true;

    sf::CircleShape circle(circleRadius, circleSegments);
    circle.setPosition(10.0f, 10.0f);


    sf::Font myFont;

    if (!myFont.loadFromFile("fonts/tech.ttf"))
    {
        std::cerr << "could not load font!\n";
        exit(-1);
    }

    sf::Text text("sample Text", myFont, 24);
    text.setPosition(0, windowHeight - (float)text.getCharacterSize());
    char displayString[255] = "sample Text";

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

            //example catching the event:
            if (event.key.code == sf::Keyboard::X)
            {
                circleSpeedX *= -1.0f;
            }
        }

        ///////// imgui updates ////////////
        ImGui::SFML::Update(window, deltaClock.restart());
        
        // draw the ui
        ImGui::Begin("Window title");
        ImGui::Text("Window Text!");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 64);
        ImGui::ColorEdit3("Color Circle", c);
        ImGui::InputText("Text", displayString, 255);
        if(ImGui::Button("set Text"))
        {
            text.setString(displayString);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Circle"))
        {
            circle.setPosition(0.0f,0.0f);
        }
        ImGui::End();

        // update shapes
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);
        circle.setFillColor(sf::Color(sf::Uint8(c[0]*255), sf::Uint8(c[1]*255), sf::Uint8(c[2]*255)));
        circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);

        window.clear();
        if (drawCircle)
        {
            window.draw(circle);
        }
        if (drawText)
        {
            window.draw(text);
        }

        ImGui::SFML::Render(window);
        window.display();
    } 
    
    return 0;
}