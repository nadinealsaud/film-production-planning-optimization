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
    int minAnnealingRentalDays = 999999;
    int maxAnnealingRentalDays = 0;
    int successfulRuns = 0;

    for (int i = 0; i < 20; i++)
    {
        Schedule testSchedule;
        auto startBT = chrono::high_resolution_clock::now();

        bool testFound = scheduler.backtrack(
            testSchedule, scenes, locations, 4, false);

        if (!testFound)
            continue;
        successfulRuns++;

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

        int saRentalDays = annealing.getRentalDays(testSA);

        totalAnnealingRentalDays += saRentalDays;

        if (saRentalDays < minAnnealingRentalDays)
            minAnnealingRentalDays = saRentalDays;

        if (saRentalDays > maxAnnealingRentalDays)
            maxAnnealingRentalDays = saRentalDays;

        totalAnnealingTime +=
            chrono::duration_cast<chrono::microseconds>(
                endSA - startSA).count();
    }

    cout << "Successful benchmark runs: " << successfulRuns << "\n";

    if (successfulRuns > 0)
    {
        cout << "Average backtracking runtime: "
            << totalBacktrackingTime / successfulRuns
            << " microseconds\n";

        cout << "Average simulated annealing runtime: "
            << totalAnnealingTime / successfulRuns
            << " microseconds\n";

        cout << "Average backtracking rental days: "
            << totalBacktrackingRentalDays /
            static_cast<double>(successfulRuns) << "\n";

        cout << "Average simulated annealing rental days: "
            << totalAnnealingRentalDays /
            static_cast<double>(successfulRuns) << "\n";

        cout << "Minimum simulated annealing rental days: "
            << minAnnealingRentalDays << "\n";

        cout << "Maximum simulated annealing rental days: "
            << maxAnnealingRentalDays << "\n";
    }
    cout << "\n--- Cooling Rate Experiment ---\n";

    vector<double> coolingRates = { 0.99, 0.995, 0.999 };

    for (double rate : coolingRates)
    {
        long long totalRuntime = 0;
        long long totalRentalDays = 0;

        int minRentalDays = 999999;
        int maxRentalDays = 0;
        int validRuns = 0;

        for (int i = 0; i < 20; i++)
        {
            auto startSA = chrono::high_resolution_clock::now();

            Schedule result = annealing.run(
                schedule1, scenes, locations, 4, rate, 500);

            auto endSA = chrono::high_resolution_clock::now();

            long long runtime =
                chrono::duration_cast<chrono::microseconds>(
                    endSA - startSA).count();

            totalRuntime += runtime;

            if (result.isValid())
            {
                validRuns++;

                int rentalDays = annealing.getRentalDays(result);

                totalRentalDays += rentalDays;

                if (rentalDays < minRentalDays)
                    minRentalDays = rentalDays;

                if (rentalDays > maxRentalDays)
                    maxRentalDays = rentalDays;
            }
        }

        cout << "\nCooling rate: " << rate << "\n";
        cout << "Valid runs: " << validRuns << "/20\n";
        cout << "Average runtime: "
            << totalRuntime / 20 << " microseconds\n";

        if (validRuns > 0)
        {
            cout << "Average rental days: "
                << totalRentalDays / static_cast<double>(validRuns)
                << "\n";

            cout << "Minimum rental days: "
                << minRentalDays << "\n";

            cout << "Maximum rental days: "
                << maxRentalDays << "\n";
        }
    }
    cout << "\n--- Exhaustive Search Experiment ---\n";

    vector<Scene> smallScenes(
        scenes.begin(), scenes.begin() + 5);

    auto exhaustiveStart = chrono::high_resolution_clock::now();

    Schedule optimalSchedule = scheduler.exhaustiveSearch(
        smallScenes, locations, 4);

    auto exhaustiveEnd = chrono::high_resolution_clock::now();

    auto exhaustiveRuntime =
        chrono::duration_cast<chrono::microseconds>(
            exhaustiveEnd - exhaustiveStart).count();

    cout << "Number of scenes: " << smallScenes.size() << "\n";
    cout << "Valid schedules explored: "
        << scheduler.getValidScheduleCount() << "\n";

    cout << "Minimum rental days found: "
        << scheduler.getBestRentalDays() << "\n";

    cout << "Schedule valid: "
        << (optimalSchedule.isValid() ? "Yes" : "No") << "\n";

    cout << "Runtime: " << exhaustiveRuntime
        << " microseconds\n";
    cout << "\n--- Five-Scene Algorithm Comparison ---\n";

    Schedule smallBTSchedule;

    bool smallBTFound = scheduler.backtrack(
        smallBTSchedule, smallScenes, locations, 4, false);

    cout << "Backtracking found schedule: "
        << (smallBTFound ? "Yes" : "No") << "\n";

    if (smallBTFound)
    {
        cout << "Backtracking rental days: "
            << annealing.getRentalDays(smallBTSchedule) << "\n";
    }

    Schedule smallSAResult;

    if (smallBTFound)
    {
        smallSAResult = annealing.run(
            smallBTSchedule, smallScenes, locations, 4);

        cout << "Simulated Annealing valid: "
            << (smallSAResult.isValid() ? "Yes" : "No") << "\n";

        cout << "Simulated Annealing rental days: "
            << annealing.getRentalDays(smallSAResult) << "\n";
    }

    cout << "Exhaustive search minimum rental days: "
        << scheduler.getBestRentalDays() << "\n";
    cout << "\n--- Simulated Annealing Improvement Test ---\n";

    Schedule inefficientSchedule;

    Schedule::Assignment a1;
    a1.scene = smallScenes[0];
    a1.day = 1;
    a1.location = locations[0];
    inefficientSchedule.assignments.push_back(a1);

    Schedule::Assignment a2;
    a2.scene = smallScenes[1];
    a2.day = 2;
    a2.location = locations[1];
    inefficientSchedule.assignments.push_back(a2);

    Schedule::Assignment a3;
    a3.scene = smallScenes[2];
    a3.day = 3;
    a3.location = locations[0];
    inefficientSchedule.assignments.push_back(a3);

    Schedule::Assignment a4;
    a4.scene = smallScenes[3];
    a4.day = 4;
    a4.location = locations[0];
    inefficientSchedule.assignments.push_back(a4);

    Schedule::Assignment a5;
    a5.scene = smallScenes[4];
    a5.day = 3;
    a5.location = locations[1];
    inefficientSchedule.assignments.push_back(a5);

    cout << "Starting schedule valid: "
        << (inefficientSchedule.isValid() ? "Yes" : "No") << "\n";

    cout << "Starting rental days: "
        << annealing.getRentalDays(inefficientSchedule) << "\n";

    Schedule improvedSchedule = annealing.run(
        inefficientSchedule, smallScenes, locations, 4);

    cout << "Improved schedule valid: "
        << (improvedSchedule.isValid() ? "Yes" : "No") << "\n";

    cout << "Rental days after Simulated Annealing: "
        << annealing.getRentalDays(improvedSchedule) << "\n";

    cout << "Exhaustive search minimum: "
        << scheduler.getBestRentalDays() << "\n";
    cout << "\n--- Repeated Improvement Experiment: 20 Runs ---\n";

    int validImprovementRuns = 0;
    int successfulImprovementRuns = 0;
    int optimalRuns = 0;

    long long totalImprovementRuntime = 0;
    long long totalImprovementRentalDays = 0;

    for (int i = 0; i < 20; i++)
    {
        auto start = chrono::high_resolution_clock::now();

        Schedule result = annealing.run(
            inefficientSchedule, smallScenes, locations, 4);

        auto end = chrono::high_resolution_clock::now();

        totalImprovementRuntime +=
            chrono::duration_cast<chrono::microseconds>(
                end - start).count();

        if (result.isValid())
        {
            validImprovementRuns++;

            int rentalDays = annealing.getRentalDays(result);

            totalImprovementRentalDays += rentalDays;

            if (rentalDays < 5)
                successfulImprovementRuns++;

            if (rentalDays == scheduler.getBestRentalDays())
                optimalRuns++;
        }
    }

    cout << "Valid runs: " << validImprovementRuns << "/20\n";

    cout << "Runs that improved on the starting schedule: "
        << successfulImprovementRuns << "/20\n";

    cout << "Runs that reached the optimal result: "
        << optimalRuns << "/20\n";

    if (validImprovementRuns > 0)
    {
        cout << "Average rental days: "
            << totalImprovementRentalDays /
            static_cast<double>(validImprovementRuns) << "\n";

        cout << "Average runtime: "
            << totalImprovementRuntime /
            validImprovementRuns << " microseconds\n";
    }
    return 0;
}