#pragma once
#include <string>
#include <vector>

using namespace std;

class Actor
{
public:
    int id;
    string name;
    vector<int> availableDays;
};