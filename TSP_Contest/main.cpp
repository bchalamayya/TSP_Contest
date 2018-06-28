/***********************************************************
 * Author: Michael S. Lewis (based on original main.cpp
 *         file provided by Patrick Mullaney for Group 17)
 * Date: 5/29/2017
 * CS325, Spring 2017
 * Description: Main driver for program for TSP.
 ************************************************/
#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <ctime>
#include "structCity.hpp"
#include "nearestNeighbor.hpp"
#include "repNeighbor.hpp"
#include "naive.hpp"
#include "twoOpt.hpp"

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;


int main(int argc, const char * argv[])
{
    /* Seed rand */
    srand(time(NULL));

  /*  std::ifstream inputFile;
   std::ofstream outputFile; attempting to fix output in flip */

    /* Will take arguments from command line or default to example 1. */
    const char* fileName;

    std::ofstream outputFile;
    string tour = ".tour";
    if(argc > 1)
    {
        fileName = argv[1];
        string fileN(fileName);
       // outputFile.open(fileN + tour);
        tour = fileN+tour;
    }
    else
        tour = fileName+tour;
    char fnameOutput[127];
    strcpy(fnameOutput, argv[1]);
    strcat(fnameOutput, ".tour");

    std::ifstream inputFile(fileName);
    string readFile(fileName);
    //inputFile.open(fileName);


    string line;
    string number;
    int tourLength;
    vector<struct city> cities;
    int inputSize;
    int **adjMatrix = 0;
    int totalVertices = 0;
    if(inputFile.is_open())
    {
        // Read input line by line.
        while(getline(inputFile, line))
        {
            // Get city info.
            std::stringstream stream(line);
            vector<int> numbers((std::istream_iterator<int>(stream)),
                                (std::istream_iterator<int>()));

            // Assign to city and add to vector.
            struct city c;
            c.cityNum = numbers[0];
            c.x = numbers[1];
            c.y = numbers[2];
            c.visited = false;
            cities.push_back(c);
            inputSize = cities.size();
        }
        FILE *readFile = fopen(argv[1], "r");
        adjMatrix = makeAdjMatrix(readFile, &totalVertices);
        inputFile.close();

        int best_tourLength = INT_MAX;
        int tour_order[totalVertices];
        int best_tour[totalVertices];

        /* Two Opt */
        if (inputSize <= 500)
        {
           /* double diff;
            clock_t start, end; */

            /* 2-Opt algorithm */
            cout << "Testing twoOpt....." << endl;
           /* start = clock(); */
            int i;
            for(i = 0; i < totalVertices; i++){
                tourLength = seededTour(adjMatrix, tour_order, totalVertices, i);
                twoOptSearch(adjMatrix, tour_order, &tourLength, fnameOutput, totalVertices);

                if(tourLength < best_tourLength) {
                    best_tourLength = tourLength;
                    memcpy(best_tour, tour_order, sizeof(int) * totalVertices);
                    writeToFile(fnameOutput, best_tour, totalVertices, best_tourLength);
                }
            }
           /* end = clock();
            diff = (end-start);
            diff = diff/CLOCKS_PER_SEC;
            // diff = diff * 1000; */
            cout << "Length of tour: " << best_tourLength << endl;
           /* cout.precision(10);
            cout << "Time nearest (m/s): " << std::fixed << diff << endl; */

           /* cout << "Testing repetitive Neighbor....." << endl;
            start = clock();
            struct tour r = repNearest(cities);
            end = clock();
            diff = (end-start);
            diff = diff/CLOCKS_PER_SEC;
            int repTourLength = r.distance;
            cout << "Length of repetitive: " << repTourLength << endl;
            cout << "Time of repetitive: " << diff << endl; */

            // Output to file
            outputFile.open(tour);
            if(outputFile.is_open())
            {
                writeToFile(fnameOutput, best_tour, totalVertices, best_tourLength);
            }
            else
                cout << "File did not open for write" << endl;
            deallocate(adjMatrix);
        }
        else
        {
           /* double diff;
            clock_t start, end; */

           /* Nearest neighbor algorithm */
            cout << "Testing repNearest....." << endl;
           /* start = clock(); */
            struct tour t = repNearest(cities);
           /* end = clock();
            diff = (end-start);
            diff = diff/CLOCKS_PER_SEC;
            // diff = diff * 1000; */
            tourLength = t.distance;
            cout << "Length of tour: " << tourLength << endl;
           /* cout.precision(10);
            cout << "Time nearest (m/s): " << std::fixed << diff << endl; */

           /* cout << "Testing repetitive Neighbor....." << endl;
            start = clock();
            struct tour r = repNearest(cities);
            end = clock();
            diff = (end-start);
            diff = diff/CLOCKS_PER_SEC;
            int repTourLength = r.distance;
            cout << "Length of repetitive: " << repTourLength << endl;
            cout << "Time of repetitive: " << diff << endl; */

            // Output to file
            outputFile.open(tour);
            if(outputFile.is_open())
            {
                outputFile << tourLength << "\n";
                for(int i = 0; i < t.path.size(); i++)
                {
                    outputFile << t.path[i].cityNum << "\n";
                    //outputFile.flush();
                }
                outputFile.flush();
                outputFile.close();
            }
            else
            cout << "File did not open for write" << endl;
        }
    }
    else
        cout << "File not found." << endl;

    return 0;
}

