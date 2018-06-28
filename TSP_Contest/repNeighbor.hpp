/***********************************************************
 * Author: Patrick Mullaney
 * Date: 6/2/2017
 * CS325, Spring 2017
 * Description: Header file for repetitive nearest Neighbor
 * algorithm for TSP (modeled after wikipedia description).
 ***********************************************************/

#ifndef repNeighbor_hpp
#define repNeighbor_hpp

#include <stdio.h>
#include "structCity.hpp"
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "structTour.h"
#include "nearestNeighbor.hpp"
#include <iostream>

using std::cout;
using std::endl;

struct tour repNeighbor(vector<struct city>, int, int**);
struct tour repNearest(vector<struct city>);
// Resets all cities to unvisited.
void resetCities(vector<struct city>);

// Functions to create DP table for distance calculations.
int** distanceInit(vector <struct city>);
int distanceHelper(struct city a, struct city b, int** t);
void repCleanup(int**, int);

#endif /* repNeighbor_hpp */
