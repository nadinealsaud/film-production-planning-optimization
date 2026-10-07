#pragma once
#include "Schedule.h"
#include <vector>

using namespace std;

class Scheduler
{
public:
    bool backtrack(
        Schedule& schedule,
        const vector<Scene>& scenes,
        const vector<Location>& locations,
        int numberOfDays)
    {
        if (schedule.assignments.size() == scenes.size())
        {
            return true;
        }

        const Scene& scene = scenes[schedule.assignments.size()];

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
                    if (backtrack(schedule, scenes, locations, numberOfDays))
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