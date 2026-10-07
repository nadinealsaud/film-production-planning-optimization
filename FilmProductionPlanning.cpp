#include <iostream>
#include <vector>
#include "Scene.h"
#include "Actor.h"
#include "Location.h"
#include "Schedule.h"
#include "Scheduler.h"

using namespace std;

int main()
{
    Scene scene1;
    scene1.id = 1;
    scene1.duration = 3;
    scene1.location = "Palace";

    Scene scene2;
    scene2.id = 2;
    scene2.duration = 6;
    scene2.location = "Palace";

    Actor actor1;
    actor1.id = 1;
    actor1.name = "Anna";
    actor1.availableDays = { 1, 2, 4 };

    scene1.actors.push_back(actor1);
    scene2.actors.push_back(actor1);

    Location location1;
    location1.id = 1;
    location1.name = "Palace";
    location1.availableDays = { 1, 3, 4 };

    vector<Scene> scenes;
    scenes.push_back(scene1);
    scenes.push_back(scene2);

    vector<Location> locations;
    locations.push_back(location1);

    Schedule schedule1;

    Scheduler scheduler;

    if (scheduler.backtrack(schedule1, scenes, locations, 4))
    {
        cout << "Backtracking found a valid schedule.\n";

        for (const Schedule::Assignment& assignment : schedule1.assignments)
        {
            cout << "Scene " << assignment.scene.id;
            cout << " is scheduled on day ";
            cout << assignment.day;
            cout << " at ";
            cout << assignment.location.name << ".\n";
        }
    }
    else
    {
        cout << "No valid schedule found.\n";
    }

    return 0;
}