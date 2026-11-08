#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include "Schedule.h"
#include "Scene.h"
#include "Location.h"

using namespace std;

class SimulatedAnnealing
{
private:
    int getRentalDaysInternal(const Schedule& schedule)
    {
        int rentalDays = 0;

        for (const auto& assignment : schedule.assignments)
        {
            bool counted = false;

            for (const auto& other : schedule.assignments)
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
                rentalDays++;
        }

        return rentalDays;
    }

    double calculateCost(Schedule& schedule)
    {
        return static_cast<double>(getRentalDaysInternal(schedule)) +
            (schedule.isValid() ? 0.0 : 1000.0);
    }

public:
    int getRentalDays(const Schedule& schedule)
    {
        return getRentalDaysInternal(schedule);
    }

    Schedule run(
        const Schedule& initialSchedule,
        const vector<Scene>& scenes,
        const vector<Location>& locations,
        int numberOfDays,
        double coolingRate = 0.999,
        int maxIterations = 1000)
    {
        if (initialSchedule.assignments.empty() ||
            locations.empty() || numberOfDays <= 0)
            return initialSchedule;


        Schedule current = initialSchedule;
        Schedule best = current;

        double temperature = 100.0;
        int iteration = 0;

        while (temperature > 0.1 && iteration < maxIterations)
        {
            Schedule neighbor = current;

            int index = rand() %
                static_cast<int>(neighbor.assignments.size());

            neighbor.assignments[index].day =
                1 + rand() % numberOfDays;

            neighbor.assignments[index].location =
                locations[rand() % locations.size()];

            double currentCost = calculateCost(current);
            double neighborCost = calculateCost(neighbor);

            if (neighborCost < currentCost ||
                static_cast<double>(rand()) / RAND_MAX <
                exp((currentCost - neighborCost) / temperature))
            {
                current = neighbor;
            }

            if (calculateCost(current) < calculateCost(best))
                best = current;

            temperature *= coolingRate;
            iteration++;
        }

        return best;
    }
};
