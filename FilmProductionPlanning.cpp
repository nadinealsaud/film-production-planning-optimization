#include <iostream>
#include <vector>
#include <chrono>
#include "SimulatedAnnealing.cpp"
#include "Scene.h"
#include "Actor.h"
#include "Location.h"
#include "Schedule.h"
#include "Scheduler.h"
#include <cstdlib>
#include <ctime>

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
    srand(static_cast<unsigned int>(time(nullptr)));

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
    scenes.push_back(createScene(9, 2, { actor1, actor2 }));
    scenes.push_back(createScene(10, 1, { actor2, actor3 }));
    scenes.push_back(createScene(11, 2, { actor1 }));
    scenes.push_back(createScene(12, 1, { actor2, actor3 }));
    scenes.push_back(createScene(13, 3, { actor1, actor2 }));
    scenes.push_back(createScene(14, 2, { actor3 }));
    scenes.push_back(createScene(15, 1, { actor1, actor3 }));

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
        scheduler.backtrack(schedule1, scenes, locations, 4, false);

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

    int backtrackingRentalDays = 0;

    for (const Schedule::Assignment& assignment : schedule1.assignments)
    {
        bool alreadyCounted = false;

        for (const Schedule::Assignment& other : schedule1.assignments)
        {
            if (other.location.id == assignment.location.id &&
                other.day == assignment.day &&
                other.scene.id < assignment.scene.id)
            {
                alreadyCounted = true;
                break;
            }
        }

        if (!alreadyCounted)
            backtrackingRentalDays++;
    }

    cout << "Backtracking rental days: "
        << backtrackingRentalDays << "\n";

    SimulatedAnnealing annealing;

    auto saStart = chrono::high_resolution_clock::now();

    Schedule saResult = annealing.run(
        schedule1, scenes, locations, 4);

    auto saEnd = chrono::high_resolution_clock::now();

    auto saRuntime =
        chrono::duration_cast<chrono::microseconds>(
            saEnd - saStart).count();

    cout << "\nSimulated Annealing Result:\n";
    cout << "Rental days: "
        << annealing.getRentalDays(saResult) << "\n";

    if (foundSchedule)
    {
        cout << "Schedule valid: "
            << (saResult.isValid() ? "Yes" : "No") << "\n";

        cout << "Runtime: " << saRuntime
            << " microseconds.\n";

        cout << "Assignments: "
            << saResult.assignments.size() << "\n";

        for (const Schedule::Assignment& assignment :
            saResult.assignments)
        {
            cout << "Scene " << assignment.scene.id
                << " is scheduled on day " << assignment.day
                << " at " << assignment.location.name << ".\n";
        }
    }
    else
    {
        cout << "Skipped: backtracking did not find a starting schedule.\n";
    }

    cout << "\n--- Benchmark: 20 runs ---\n";

    long long totalBacktrackingTime = 0;
    long long totalAnnealingTime = 0;
    long long totalBacktrackingRentalDays = 0;
    long long totalAnnealingRentalDays = 0;

    for (int i = 0; i < 20; i++)
    {
        Schedule testSchedule;
        auto startBT = chrono::high_resolution_clock::now();

        bool testFound = scheduler.backtrack(
            testSchedule, scenes, locations, 4, false);

        if (!testFound)
            continue;

        auto endBT = chrono::high_resolution_clock::now();
        totalBacktrackingTime +=
            chrono::duration_cast<chrono::microseconds>(
                endBT - startBT).count();
        int runRentalDays = 0;

        for (const Schedule::Assignment& assignment : testSchedule.assignments)
        {
            bool counted = false;

            for (const Schedule::Assignment& other : testSchedule.assignments)
            {
                if (other.location.id == assignment.location.id &&
                    other.day == assignment.day &&
                    other.scene.id < assignment.scene.id)
                {
                    counted = true;
                    break;
                }
            }

            if (!counted)
                runRentalDays++;
        }

        totalBacktrackingRentalDays += runRentalDays;

        auto startSA = chrono::high_resolution_clock::now();
        Schedule testSA = annealing.run(testSchedule, scenes, locations, 4);
        auto endSA = chrono::high_resolution_clock::now();

        totalAnnealingRentalDays += annealing.getRentalDays(testSA);

        totalAnnealingTime +=
            chrono::duration_cast<chrono::microseconds>(
                endSA - startSA).count();
    }

    cout << "Average backtracking runtime: "
        << totalBacktrackingTime / 20 << " microseconds\n";

    cout << "Average simulated annealing runtime: "
        << totalAnnealingTime / 20 << " microseconds\n";

    cout << "Average backtracking rental days: "
        << totalBacktrackingRentalDays / 20.0 << "\n";

    cout << "Average simulated annealing rental days: "
        << totalAnnealingRentalDays / 20.0 << "\n";
    return 0;
}