/**************************************************************
 * Author: Patrick Mullaney
 * Date: 6/1/2017
 * CS325, Spring 2017
 * Description: Header file for nearest Neighbor algorithm 
 * implementation of TSP (modeled after wikipedia description).
 *************************************************************/

#ifndef nearestNeighbor_hpp
#define nearestNeighbor_hpp

#include <stdio.h>
#include "structCity.hpp"
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "structTour.h"

using std::vector;

int nearestInt(struct city, struct city);
struct tour nearestNeighbor(vector<struct city>);

#endif /* nearestNeighbor_hpp */
