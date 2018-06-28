/***********************************************************
 * Author: Michael S. Lewis (based on nearestNeighbor.cpp
 *         file provided by Patrick Mullaney for Group 17)
 * Date: 6/5/2017
 * CS325, Spring 2017
 * Description: Implementation file for Naive
 * algorithm for TSP.
 ***********************************************************/

#include "naive.hpp"

using std::cout;
using std::endl;

/*
 Start at cityNum 0 as current, i as next.  Find distance, set current to next, increment i.
 Visits all cities in order, then returns to starting city.
 */
struct tour naive(vector<struct city> cities){

    vector<struct city> path;

      /* Start at city 0 */
    int index = 0;
    struct city current = cities[index];
    path.push_back(current);

    int distance = 0;
    int tempDist = 0;
    struct city next;
    int i;
    for(i = 1; i < cities.size(); i++)
    {
        /* Calculate distance. */
        tempDist = distanceForm(current, cities[i]);
        /* Update next */
        next = cities[i];

        /* Assign current city to next and push to path.*/
        current = cities[i];
        distance += tempDist;
        path.push_back(current);
        /* For trip back to start */
        if (i == (cities.size() - 1))
        {
            next = cities[0];
            /* Calculate distance. */
            tempDist = distanceForm(cities[i], cities[0]);
            distance += tempDist;
        }
    }

    struct tour t;
    t.distance = distance;
    t.path = path;
    return t;
};

/*
  Distance formula to calculate distance from city a to city b.
*/
int distanceForm(struct city a, struct city b){

    int near;
    double n = 0.0;
    double x1 = a.x;
    double x2 = b.x;
    double y1 = a.y;
    double y2 = b.y;

    // Calculate distance.
    n = sqrt(pow((x1 - x2),2.0) + pow((y1 - y2),2.0));
    // Round up if rem >= .5 else round down.
    int temp = floor(n);
    double rem = n - temp;
    if (rem >= 0.5)
        near = ceil(n);
    else
        near = floor(n);

    return near;
}
