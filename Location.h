#pragma once
#include <string>
#include <vector>

using namespace std;

class Location
{
public:
    int id;
    string name;
    vector<int> availableDays;
};