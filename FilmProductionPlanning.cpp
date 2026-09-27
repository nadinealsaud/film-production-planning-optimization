#include <iostream>
#include "Scene.h"

using namespace std;

int main()
{
    Scene scene1;

    scene1.id = 1;
    scene1.duration = 3;
    scene1.location = "Palace";

    cout << "Scene: " << scene1.id << "\n";
    cout << "Duration: " << scene1.duration << " hours\n";
    cout << "Location: " << scene1.location << "\n";

    return 0;
}