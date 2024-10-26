#include <SFML/Graphics.hpp>
#include <fstream>

#include "Game.h"


int main(int argc, char* argv[])
{
        Game g("config.txt");
        g.run();
}