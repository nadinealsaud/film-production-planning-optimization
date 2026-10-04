#pragma once
#include <vector>
#include <string>
#include "Scene.h"
#include "Location.h"

using namespace std;

class Schedule
{
public:
    struct Assignment
    {
        Scene scene;
        int day;
        Location location;
    };

    vector<Assignment> assignments;

    bool isValid()
    {
        const int maxDailyHours = 8;

        vector<int> dailyHours(100, 0);

        for (Assignment& assignment : assignments)
        {
            int day = assignment.day;

            bool locationAvailable = false;

            for (int availableDay : assignment.location.availableDays)
            {
                if (availableDay == day)
                {
                    locationAvailable = true;
                    break;
                }
            }

            if (!locationAvailable)
            {
                return false;
            }

            for (Actor& actor : assignment.scene.actors)
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
                    return false;
                }
            }

            dailyHours[day] += assignment.scene.duration;

            if (dailyHours[day] > maxDailyHours)
            {
                return false;
            }
        }

        return true;
    }
};