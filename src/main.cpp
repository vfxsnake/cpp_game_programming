#include <SFML/Graphics.hpp>

#include "Game.h"


// void ReadConfig(const std::string& configFile, 
//                 int &windowWidth, 
//                 int &windowHeight,
//                 std::string &fontPath,
//                 int &fontSize,
//                 int &fontColorR,
//                 int &fontColorG,
//                 int &fontColorB,
//                 std::vector<Circle> &circles,
//                 std::vector<Rectangle> &rectangles 
//                 )
// {
//     std::ifstream fileIn(configFile);
//     std::string objectType;
//     while (fileIn >> objectType)
//     {
//         if (objectType == "Window")
//         {
//             fileIn >> windowWidth >> windowHeight;
//         }

//         if (objectType == "Font")
//         {
//             fileIn >> fontPath
//                    >> fontSize
//                    >> fontColorR
//                    >> fontColorG
//                    >> fontColorB;
//         } 

//         if (objectType == "Circle")
//         {
//             std::string name;
//             sf::Vector2f position;
//             sf::Vector2f velocity;
//             int iColor[3];
//             float radius;
//             fileIn >> name
//                    >> position.x 
//                    >> position.y 
//                    >> velocity.x 
//                    >> velocity.y
//                    >> iColor[0] 
//                    >> iColor[1] 
//                    >> iColor[2]
//                    >> radius;
//             sf::Color color{uint8_t(iColor[0]), uint8_t(iColor[1]), uint8_t(iColor[2])};
//             Circle c(name, radius, 32, position, velocity, color);
//             circles.push_back(c); 
//         }

//         if (objectType == "Rectangle")
//         {
//             std::string name;
//             sf::Vector2f position;
//             sf::Vector2f velocity;
//             int iColor[3];
//             sf::Vector2f size;

//             fileIn >> name
//                    >> position.x
//                    >> position.y
//                    >> velocity.x
//                    >> velocity.y
//                    >> iColor[0] 
//                    >> iColor[1] 
//                    >> iColor[2]
//                    >> size.x
//                    >> size.y;
//             sf::Color color{uint8_t(iColor[0]), uint8_t(iColor[1]), uint8_t(iColor[2])};
//             Rectangle r(name, size, position, velocity, color);
//             rectangles.push_back(r);
//         }
//     }

// }


int main(int argc, char* argv[])
{
        Game g("config.txt");
        g.run();
}