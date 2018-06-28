/**************************************************************
 * Author: Patrick Mullaney (originally nearestNeighbor.hpp,
 *         modified by Michael Lewis for naive.hpp)
 * Date: 6/5/2017
 * CS325, Spring 2017
 * Description: Header file for Naive algorithm
 * implementation of TSP (modeled after wikipedia description).
 *************************************************************/

#ifndef naive_hpp
#define naive_hpp

#include <stdio.h>
#include "structCity.hpp"
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "structTour.h"

using std::vector;

int distanceForm(struct city, struct city);
struct tour naive(vector<struct city>);

#endif /* naive_hpp */
