#include "potato/application.hpp"
#include <iostream>

using namespace std;

int main()
{
    const unsigned int width = 1152, height = width * 10/16;
    try {
        Application game(width, height, "LD46 - Garden Keeper");
        game.run();
    } catch(std::exception& e) {
        std::cerr << "Game ended unexpectedely : " << std::endl;
        std::cerr << e.what() << std::endl;
    }
    return 0;
}