#pragma once
#include <string>
#include <vector>
#include "Actor.h"

using namespace std;

class Scene
{
public:
    int id;
    int duration;
    string location;
    vector<Actor> actors;
};