#include <iostream>
#include <vector>
#include <chrono>
#include "Scene.h"
#include "Actor.h"
#include "Location.h"
#include "Schedule.h"
#include "Scheduler.h"

using namespace std;

Scene createScene(int id, int duration, const vector<Actor>& actors)
{
    Scene scene;
    scene.id = id;
    scene.duration = duration;
    scene.actors = actors;

    return scene;
}


int main()
{
    Actor actor1;
    actor1.id = 1;
    actor1.name = "Anna";
    actor1.availableDays = { 1, 2, 4 };

    Actor actor2;
    actor2.id = 2;
    actor2.name = "Mark";
    actor2.availableDays = { 1, 2, 3, 4 };

    Actor actor3;
    actor3.id = 3;
    actor3.name = "Sara";
    actor3.availableDays = { 2, 3, 4 };

    vector<Scene> scenes;
    scenes.push_back(createScene(1, 2, { actor1 }));
    scenes.push_back(createScene(2, 3, { actor1, actor2 }));
    scenes.push_back(createScene(3, 1, { actor2 }));
    scenes.push_back(createScene(4, 4, { actor1, actor3 }));
    scenes.push_back(createScene(5, 2, { actor3 }));
    scenes.push_back(createScene(6, 3, { actor1, actor2 }));
    scenes.push_back(createScene(7, 2, { actor2, actor3 }));
    scenes.push_back(createScene(8, 1, { actor1, actor3 }));


    Location location1;
    location1.id = 1;
    location1.name = "Palace";
    location1.availableDays = { 1, 3, 4 };

    Location location2;
    location2.id = 2;
    location2.name = "Studio";
    location2.availableDays = { 2, 3 };

    vector<Location> locations;
    locations.push_back(location1);
    locations.push_back(location2);

    Schedule schedule1;

    Scheduler scheduler;

    auto start = chrono::high_resolution_clock::now();

    bool foundSchedule =
        scheduler.backtrack(schedule1, scenes, locations, 4);

    auto end = chrono::high_resolution_clock::now();

    auto runtime =
        chrono::duration_cast<chrono::microseconds>(end - start).count();

    if (foundSchedule)
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

    cout << "Runtime: " << runtime << " microseconds.\n";

    return 0;
}