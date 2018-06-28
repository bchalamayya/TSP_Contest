/******************************************************************
 * Author: Patrick Mullaney
 * Date: 6/1/2017
 * CS325, Spring 2017
 * Description: Header file for struct tour for TSP, which contains 
 * vector of path through cities and distance, length of the tour.
 ******************************************************************/

#ifndef structTour_h
#define structTour_h
#include <vector>
#include "structCity.hpp"

using std::vector;

struct tour{
    vector<struct city> path;
    int distance;
};

#endif /* structTour_h */
