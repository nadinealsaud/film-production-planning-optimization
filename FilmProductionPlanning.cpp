#include <iostream>
#include "Scene.h"
#include "Actor.h"
#include "Location.h"
#include "Schedule.h"

using namespace std;

int main()
{
    Scene scene1;

    scene1.id = 1;
    scene1.duration = 3;
    scene1.location = "Palace";

    Actor actor1;

    actor1.id = 1;
    actor1.name = "Anna";
    actor1.availableDays = { 1, 2, 4 };

    scene1.actors.push_back(actor1);

    Location location1;

    location1.id = 1;
    location1.name = "Palace";
    location1.availableDays = { 1, 3, 4 };

    cout << "Scene: " << scene1.id << "\n";
    cout << "Duration: " << scene1.duration << " hours\n";
    cout << "Location: " << scene1.location << "\n";

    cout << "Actor: " << actor1.name << "\n";
    cout << "Available days: ";

    for (int day : actor1.availableDays)
    {
        cout << day << " ";
    }

    cout << "\n";

    cout << "Location: " << location1.name << "\n";
    cout << "Available days: ";

    for (int day : location1.availableDays)
    {
        cout << day << " ";
    }

    cout << "\n";

    Schedule schedule1;

    Schedule::Assignment assignment1;

    assignment1.scene = scene1;
    assignment1.day = 1;
    assignment1.location = location1;

    schedule1.assignments.push_back(assignment1);

    if (schedule1.isValid())
    {
        cout << "Schedule is valid.\n";
    }
    else
    {
        cout << "Schedule is invalid.\n";
    }

    return 0;
}