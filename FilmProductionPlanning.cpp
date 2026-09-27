#include <iostream>
#include "Scene.h"

int main()
{
    Scene scene1;

    scene1.id = 1;
    scene1.duration = 3;
    scene1.location = "Palace";

    std::cout << "Scene: " << scene1.id << "\n";
    std::cout << "Duration: " << scene1.duration << " hours\n";
    std::cout << "Location: " << scene1.location << "\n";

    return 0;
}