/*******************************************************************
 * Author: Patrick Mullaney
 * Date: 6/2/2017
 * CS325, Spring 2017
 * Description: Implementation file for repetitive nearest Neighbor
 * algorithm for TSP (modeled after wikipedia description).
 ******************************************************************/

#include "repNeighbor.hpp"
/* Resets all cities to unvisited. */
void resetCities(vector<struct city> cities)
{
    for(int i = 0; i < cities.size(); i++)
    {
        cities[i].visited = false;
    }
}
/* Runs repetitive nearest neighbor algorithm. */
struct tour repNearest(vector<struct city> cities)
{
    vector<struct tour> tours;
    struct tour t;
    
    /* Obtain DP table */
    int** table = distanceInit(cities);
    
    /* Calculate tour based on nearest neighbor algorithm for each city, 
     for larger inputs, will not run nearest neighbor on all cities, but rather 
     a sample to reduce runtime. */
    if(cities.size() < 2000)
    {
        for(int i = 0; i < cities.size(); i++)
        {
            t = repNeighbor(cities, i, table);
            tours.push_back(t);
        }
    }
    else
    {
        /* If input is larger, run nearest neighbor ona  sample percentage */
        int s = (cities.size()/200);
        /* Start index at a random number between 0 and 9 */
        int r = rand()%10;
        for(int i = r; i < cities.size(); i = i+s)
        {
            t = repNeighbor(cities, i, table);
            tours.push_back(t);
        }
    }
    
    /* Take the shortest path as optimal path */
    int shortest = 99999999;
    
    for(int i = 0; i < tours.size(); i++)
    {
        if(tours[i].distance < shortest)
        {
            shortest = tours[i].distance;
            t = tours[i];
        }
    }
    
    repCleanup(table, cities.size());
    return t;
}

struct tour repNeighbor(vector<struct city> cities, int index, int** table){
    
    vector<struct city> path;
    
    /* Start at input city */
    struct city current = cities[index];
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
                int d = distanceHelper(current, cities[i], table);
                
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
    resetCities(cities);
    return t;
};
int** distanceInit(vector <struct city> cities)
{
    int size = cities.size();
    
    int **table = new int*[size];
    for(int i = 0; i < size; i++)
    {
        table[i] = new int[size];
    }
    /* Initialize to arbitrarily large # */
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            table[i][j] = 999999;
        }
    }
    
    return table;
}
/* Returns distance between two cities */
int distanceHelper(struct city a, struct city b, int** table){
    
    int current, next;
    current = a.cityNum;
    next = b.cityNum;
    int near;
    
    /* If not yet calculated, calculate and update table */
    if(table[current][next] == 999999)
    {
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
        
        table[current][next] = near;
    }
    else /* Else return distance value from table */
        near = table[current][next];
    
    return near;
}
/* To free DP table. */
void repCleanup(int** table, int size)
{
    for(int i = 0; i < size; i++)
    {
        delete table[i];
    }
    delete[] table;
}