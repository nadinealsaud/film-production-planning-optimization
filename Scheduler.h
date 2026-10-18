#pragma once
#include "Schedule.h"
#include <vector>
#include <limits>

using namespace std;

class Scheduler
{
private:
    int countAvailableOptions(
        const Scene& scene,
        const vector<Location>& locations,
        int numberOfDays)
    {
        int count = 0;

        for (int day = 1; day <= numberOfDays; day++)
        {
            for (const Location& location : locations)
            {
                bool locationAvailable = false;

                for (int availableDay : location.availableDays)
                {
                    if (availableDay == day)
                    {
                        locationAvailable = true;
                        break;
                    }
                }

                if (!locationAvailable)
                {
                    continue;
                }

                bool allActorsAvailable = true;

                for (const Actor& actor : scene.actors)
                {
                    bool actorAvailable = false;

                    for (int availableDay : actor.availableDays)
                    {
                        if (availableDay == day)
                        {
                            actorAvailable = true;
                            break;
                        }
                    }

                    if (!actorAvailable)
                    {
                        allActorsAvailable = false;
                        break;
                    }
                }

                if (allActorsAvailable)
                {
                    count++;
                }
            }
        }

        return count;
    }

public:
    bool backtrack(
        Schedule& schedule,
        const vector<Scene>& scenes,
        const vector<Location>& locations,
        int numberOfDays,
        bool useHeuristic = true)
    {
        if (schedule.assignments.size() == scenes.size())
        {
            return true;
        }

        int selectedIndex = -1;
        int fewestOptions = numeric_limits<int>::max();

        for (int i = 0; i < static_cast<int>(scenes.size()); i++)
        {
            bool alreadyAssigned = false;

            for (const Schedule::Assignment& assignment : schedule.assignments)
            {
                if (assignment.scene.id == scenes[i].id)
                {
                    alreadyAssigned = true;
                    break;
                }
            }

            if (alreadyAssigned)
            {
                continue;
            }

            if (!useHeuristic)
            {
                selectedIndex = i;
                break;
            }

            int options = countAvailableOptions(
                scenes[i], locations, numberOfDays);

            if (options < fewestOptions)
            {
                fewestOptions = options;
                selectedIndex = i;
            }
        }

        if (selectedIndex == -1 ||
            (useHeuristic && fewestOptions == 0))
        {
            return false;
        }

        const Scene& scene = scenes[selectedIndex];

        for (int day = 1; day <= numberOfDays; day++)
        {
            for (const Location& location : locations)
            {
                Schedule::Assignment assignment;
                assignment.scene = scene;
                assignment.day = day;
                assignment.location = location;

                schedule.assignments.push_back(assignment);

                if (schedule.isValid())
                {
                    if (backtrack(
                        schedule, scenes, locations,
                        numberOfDays, useHeuristic))
                    {
                        return true;
                    }
                }

                schedule.assignments.pop_back();
            }
        }

        return false;
    }
};
