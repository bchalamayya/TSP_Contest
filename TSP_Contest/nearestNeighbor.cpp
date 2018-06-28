/***********************************************************
 * Author: Patrick Mullaney
 * Date: 6/1/2017
 * CS325, Spring 2017
 * Description: Implementation file for nearest Neighbor 
 * algorithm for TSP (modeled after wikipedia description).
 ***********************************************************/

#include "nearestNeighbor.hpp"

using std::cout;
using std::endl;

/* Psuedocode description based on wikipedia:
 Start at a random city, find the nearest city (shortest edge between current
 and any unvisited city).  Set current city to closest neighbor, mark visited, repeat
 until all cities are visited.  Sequence of visited cities represents the path.
 */
struct tour nearestNeighbor(vector<struct city> cities){
    
    vector<struct city> path;
    
      /* Start at random city */
    int index = rand()%cities.size();
    struct city current = cities[index];
    current.visited = true;
    cities[index].visited = true;
    path.push_back(current);
    
    int distance = 0;
   
    while(path.size() < cities.size())
    {
        /* Initialize distance to arbitrarily large number.*/
        int nearest = 9999999;
        struct city next;
        
        for(int i = 0; i < cities.size(); i++)
        {
            /* If city is unvsited, calculate disance */
            if((cities[i].visited == false) && (current.cityNum != cities[i].cityNum))
            {
                /* Calculate distance. */
                int d = nearestInt(current, cities[i]);
                /* If distance to next is the nearest city, update next */
                if(d < nearest)
                {
                    nearest = d;
                    next = cities[i];
                }
            }
        }
        /* After iterating through unvisited cities, assign current city to next and push to path.*/
        current = cities[next.cityNum];
        cities[next.cityNum].visited = true;
        distance += nearest;
        path.push_back(current);
    }
    
    /* After path run, add distance from last city back to first. */
    distance += nearestInt(current, cities[index]);
    
    struct tour t;
    t.distance = distance;
    t.path = path;
    return t;
};

/* Psuedocode description based on wikipedia:
  Start at a random city, find the nearest city (shortest edge between current
 and any unvisited city).  Set current city to closest neighbor, mark visited, repeat
 until all cities are visited.  Sequence of visited cities represents the path.
*/
int nearestInt(struct city a, struct city b){
    
    int near;
   /* Original calculation
    float n = 0.0;
    float x1 = a.x;
    float x2 = b.x;
    float y1 = a.y;
    float y2 = b.y;
    
    // Calculate distance.
    n = sqrt(pow((x1 - x2),2.0) + pow((y1-y2),2.0));
      Round up to nearest int.
     n = round(n);
     near = n; */
    
    /* Amended distance calculation */
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
    
    if(rem >= 0.5)
    {
        near = ceil(n);
    }
    else
        near = floor(n);
    
    return near;
}