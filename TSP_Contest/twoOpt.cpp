/***********************************************************
 * Author: Michael S. Lewis
 * Date: 6/6/2017
 * CS325, Spring 2017
 * Description: Implementation file for 2-Opt Algorithm.
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include "twoOpt.hpp"


/* Converts input file coordinates into adjacency matrix using a 2D array. Based on example provided at
 http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems */
int** makeAdjMatrix(FILE *readFile, int *totalVertices) {

	// Initializes size of array
	int ARRAY_SIZE = 128;

	// Arrays store x and y vertices, respectively
	int *x_Vertices;
	int *y_Vertices;

	// ARRAY_SIZE determines memory allocation for x_Vertices and y_Vertices
	x_Vertices = (int *)malloc(sizeof(int) * ARRAY_SIZE);
	y_Vertices = (int *)malloc(sizeof(int) * ARRAY_SIZE);
	// Counts index of vertex arrays
	int index = 0;

	// Doubles size of array as needed while input file continues to provide expected input
	while (fscanf(readFile, "%d %d %d", totalVertices, &(x_Vertices[index]), &(y_Vertices[index])) == 3) {

		// Increments index of vertex arrays
		++index;

		// Reallocates to resize vertex arrays to double previous size as needed
		if (index > ARRAY_SIZE - 1) {
			ARRAY_SIZE = ARRAY_SIZE * 2;
			x_Vertices = (int *)realloc(x_Vertices, sizeof(int) * ARRAY_SIZE);
			y_Vertices = (int *)realloc(y_Vertices, sizeof(int) * ARRAY_SIZE);
		}
	}

	// Track totalVertices based on vertex array indices
	*totalVertices = index;

	// Build adjMatrix as product of x_Vertices and y_Vertices arrays
	// Rows are int **
	int **adjMatrix = (int **)malloc(*totalVertices * sizeof(int *));
	int i;
	for (i = 0; i < *totalVertices; i++) {
		// Columns are int *
		adjMatrix[i] = (int *)malloc(*totalVertices * sizeof(int));
	}

	// Complete adjMatrix using input file values
	for (i = 0; i < *totalVertices; i++) {
        int j;
		for (j = 0; j < *totalVertices; j++) {
			adjMatrix[i][j] = distForm(x_Vertices[i], y_Vertices[i],
			 x_Vertices[j], y_Vertices[j]);
		}
	}

	// Free memory from allocated arrays
	free(x_Vertices);
	free(y_Vertices);

	return adjMatrix;
}

/* Frees memory allocated for adjMatrix */
void deallocate(int **adjMatrix) {
	free(adjMatrix);
}

/* Calculates distance between each city in tour, returns rounded value */
int distForm(int x1, int y1, int x2, int y2) {
	double xDist = x2 - x1;
	double yDist = y2 - y1;
	double distance = sqrt(xDist * xDist + yDist * yDist);
	return (int) round(distance);
}

/* Iterates through all cities from beginning, in order */
int naiveTour(int **adjMatrix, int *tour, int totalVertices) {

	// Iterate through all cities, beginning at city 0
	tour[0] = 0;
	int cityNumber = 1;

	while (cityNumber < totalVertices) {
		tour[cityNumber] = cityNumber;
		cityNumber = cityNumber + 1;
	}

	int tourLength = tourDistance(adjMatrix, tour, totalVertices);

	return tourLength;
}

/* Opens outputFile, writes output, and closes */
void writeToFile(char *fnameOutput, int *tour, int totalVertices, int tourLength) {
	// Attempt to open outputFile
	FILE *outputFile = fopen(fnameOutput, "w");
	if (!outputFile) {
		fprintf(stderr, "\nERROR: Could not open output file: '%s'\n.", fnameOutput);
		exit(-1);
	}

    // Prints tour length to outputFile
	fprintf(outputFile, "%d\n", tourLength);
	int i;
	for (i = 0; i < totalVertices; i++) {
		fprintf(outputFile, "%d\n", tour[i]);
	}

	// Indicates if error occurs when closing outputFile
	if (fclose(outputFile)){
		fprintf(stderr, "\nERROR: Output file failed to close.\n");
		exit(-1);
	}
}

/* Searches through all vertices for pairs of edges to swap if better length. Based on example provided at
 http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems */
void twoOptSearch(int **adjMatrix, int *tour, int *tourLength, char *fnameOutput, int totalVertices) {

	bool better = true;
	int prevLength = *tourLength;

	while (better) {
		better = false;
		bool searchComplete = false;
        int i;
		for (i = 1; i < totalVertices - 1 && !searchComplete; i++) {
            int j;
			for (j = i + 1; j < totalVertices && !searchComplete; j++) {
				prevLength = *tourLength;
				swapEfficient(adjMatrix, tour, tourLength, totalVertices, i, j);
				if (*tourLength < prevLength) {
				 	// New length is better than previous
				 	better = true;
				 	// Evaluate as complete to write to file
				 	searchComplete = true;
                }
			}
		}
	}
}

/* Calculates tourLength */
int tourDistance(int **adjMatrix, int *tour, int totalVertices) {
	int tourLength = 0;

	// Distance from current (tour[i]) to next (tour[i+1]) is added to tourLength
	int i;
	for (i = 0; i < totalVertices - 1; i++) {
		tourLength += adjMatrix[tour[i]][tour[i+1]];
	}

	// Distance from last city (tour[end]) to beginning city (tour[0]) is added to tourLength
	tourLength += adjMatrix[tour[totalVertices - 1]][tour[0]];

	return tourLength;
}

/* Builds new tours to swap vertexA and vertex in the route order, in an attempt to find a more efficient route. Based on
example provided at http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems */
void twoOptSwap(int *newTour, int *tour, int totalVertices, int vertexA, int vertexB) {

	int nodeMin = MIN(vertexA, vertexB);
	int nodeMax = MAX(vertexA, vertexB);
	int indexNew = 0;

	/* Builds newTour as result of tour[0] through tour[nodeMin - 1] */
	int i;
	for (i = 0; i < nodeMin; i++) {
		newTour[indexNew] = tour[i];
		indexNew++;
	}

    /* Builds newTour as result of tour[nodeMax], descending through tour[nodeMin] */
	for (i = nodeMax; i >= nodeMin; i--) {
		newTour[indexNew] = tour[i];
		indexNew++;
	}

    /* Builds newTour as result of tour[nodeMax + 1] through tour[totalVertices] */
	for (i = nodeMax + 1; i < totalVertices; i++) {
		newTour[indexNew] = tour[i];
		indexNew++;
	}
}

/* Performs swap of vertexA and vertexB in route order, if it produces a more optimal result. Based on example provided at
 http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems */
void swapEfficient(int **adjMatrix, int *tour, int *tourLength,
int totalVertices, int vertexA, int vertexB) {

	int nodeMin = MIN(vertexA, vertexB);
	int nodeMax = MAX(vertexA, vertexB);
	int indexNew = 0;

	int lengthRemoved = 0;
	int lengthAdded = 0;

	int newTour[totalVertices];

    /* Replaces removed distances with added distances */
	if (nodeMax + 1 < totalVertices) {
		lengthRemoved = adjMatrix[tour[nodeMin-1]][tour[nodeMin]] +
		adjMatrix[tour[nodeMax]][tour[nodeMax + 1]];

		lengthAdded = adjMatrix[tour[nodeMin-1]][tour[nodeMax]] +
		adjMatrix[tour[nodeMin]][tour[nodeMax + 1]];
	/* Handles condition in which tour wraps around to beginning city */
	} else {
		lengthRemoved = adjMatrix[tour[nodeMin-1]][tour[nodeMin]] +
		adjMatrix[tour[nodeMax]][tour[0]];

		lengthAdded = adjMatrix[tour[nodeMin-1]][tour[nodeMax]] +
		adjMatrix[tour[nodeMin]][tour[0]];
	}

	int tourDiff = lengthRemoved - lengthAdded;

	/* Recalculates tour if shorter length is found */
	if (tourDiff > 0) {
        /* Builds newTour as result of tour[0] through tour[nodeMin - 1] */
        int i;
		for (i = 0; i < nodeMin; i++) {
			newTour[indexNew] = tour[i];
			indexNew++;
		}

        /* Builds newTour as result of tour[nodeMax], descending through tour[nodeMin] */
		for (i = nodeMax; i >= nodeMin; i--) {
			newTour[indexNew] = tour[i];
			indexNew++;
		}

        /* Builds newTour as result of tour[nodeMax + 1] through tour[totalVertices] */
		for (i = nodeMax + 1; i < totalVertices; i++) {
			newTour[indexNew] = tour[i];
			indexNew++;
		}

		/* Replaces tour with newTour */
		memcpy(tour, newTour, sizeof(int) * totalVertices);

		*tourLength = *tourLength - tourDiff;
	}

	return;
}

/* Starts at a random city and finds the nearest city (shortest edge between current
 and any unvisited city). Sets current city to closest neighbor, marks visited, and
 repeats until all cities are visited. Sequence of visited cities represents the path.
 */
int nearestNeighborTour(int **adjMatrix, int *tour, int totalVertices) {

	// Initializes array to store currentTour
	int currentTour[totalVertices];
	int currentTourLength = 0;
	int bestTourLength = INT_MAX;
	int cityZero;

	// New cityVisited array stores cities visited
	int cityVisited[totalVertices];
	// New startsTested array stores cities tested as candidates for beginning of tour
	int startsTested[totalVertices];

	int cityNumber = 0;
	int prevCity = 0;

	int nearestDist = INT_MAX;
	int nearestCity = totalVertices;
	int i;
	// Fill startsTested array for each vertex
	for (i = 0; i < totalVertices; i++) {
		startsTested[i] = 0;
	}

    // Match cities in cityVisited to corresponding vertices from startsTested
    for (i = 0; i < totalVertices; i++) {
        cityVisited[i] = 0;
		}

		// Update nearestDist and nearestCity
		nearestDist = INT_MAX;
		nearestCity = totalVertices;

		// Sets start city to random value within vertex range
		cityZero = findRandom(0, totalVertices - 1);
		while (startsTested[cityZero]) {
			cityZero = findRandom(0, totalVertices - 1);
		}

		// Add start city to startsTested array
		startsTested[cityZero] = 1;
		// Account for start city as visited in tour
		cityVisited[cityZero] = 1;

		currentTour[0] = cityZero;
		currentTourLength = 0;

		cityNumber = 1;

        // Determine if starting at tour's previous city improves result
		while (cityNumber < totalVertices) {
			prevCity = currentTour[cityNumber - 1];
            int k;
			for (k = 0; k < totalVertices; k++) {
				if (!cityVisited[k] && adjMatrix[prevCity][k] < nearestDist) {
					nearestDist = adjMatrix[prevCity][k];
					nearestCity = k;
				}
			}

			// Add nearest city as visited in tour
			currentTour[cityNumber] = nearestCity;
			currentTourLength += nearestDist;
			cityVisited[nearestCity] = 1;
			cityNumber += 1;

            // Update nearestDist and nearestCity
			nearestDist = INT_MAX;
			nearestCity = totalVertices;
		}

		// Account for return distance from last city back to starting city
		currentTourLength += adjMatrix[currentTour[0]][currentTour[totalVertices-1]];

		// If the current tour is an improvement, it overwrites the bestTourLength
		if (currentTourLength < bestTourLength) {
			bestTourLength = currentTourLength;
			memcpy(tour, currentTour, sizeof(int) * totalVertices);
		}

	return bestTourLength;
}

/* Builds the optimized tour by tracking cities visited and nearestCity to update tourLength. Based on example provided
at http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems */
int seededTour(int **adjMatrix, int *tour, int totalVertices, int cityZero) {

	// Initializes tourLength to 0
	int tourLength = 0;

	// New cityVisited array stores cities visited
	int cityVisited[totalVertices];

	int cityNumber = 0;
	int prevCity = 0;

    // Updates nearestDist and nearestCity
	int nearestDist = INT_MAX;
	int nearestCity = totalVertices;

    // Initializes cities in cityVisited array to 0
	int i;
	for (i = 0; i < totalVertices; i++) {
		cityVisited[i] = 0;
	}

    // Accounts for start city as visited in tour
	cityVisited[cityZero] = 1;
	tour[0] = cityZero;

	cityNumber = 1;
    // Determines if starting at tour's previous city improves result
	while (cityNumber < totalVertices) {
		prevCity = tour[cityNumber - 1];
        int k;
		for (k = 0; k < totalVertices; k++) {
			if (!cityVisited[k] && adjMatrix[prevCity][k] < nearestDist) {
				nearestDist = adjMatrix[prevCity][k];
				nearestCity = k;
			}
		}

		// Adds nearest city to tour
		tour[cityNumber] = nearestCity;
		tourLength = tourLength + nearestDist;
		cityVisited[nearestCity] = 1;
		cityNumber = cityNumber + 1;

        // Updates nearestDist and nearestCity
		nearestDist = INT_MAX;
		nearestCity = totalVertices;
	}

    // Adds return distance from last city back to starting city to tourLength
	tourLength += adjMatrix[tour[0]][tour[totalVertices-1]];

	return tourLength;
}

/* Generates random value within range of maxVal - minVal. */
unsigned int findRandom(unsigned int minVal, unsigned int maxVal) {

    int random;
    const unsigned int range = 1 + maxVal - minVal;
    const unsigned int bins = RAND_MAX / range;
    const unsigned int limit = bins * range;
    do
    {
        random = rand();
    } while (random >= limit);

    return minVal + (random / bins);
}
