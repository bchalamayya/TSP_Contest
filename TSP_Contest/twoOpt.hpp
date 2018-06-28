/***********************************************************
 * Author: Michael S. Lewis
 * Date: 6/6/2017
 * CS325, Spring 2017
 * Description: Header file for 2-Opt Algorithm.
 ***********************************************************/
#ifndef twoOpt_hpp
#define twoOpt_hpp

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int** makeAdjMatrix(FILE *readFile, int *totalVertices);
void deallocate(int **adjMatrix);
int distForm(int x1, int y1, int x2, int y2);
int naiveTour(int **adjMatrix, int *tour, int totalVertices);
int nearestNeighborTour(int **adjMatrix, int *tour, int totalVertices);
int seededTour(int **adjMatrix, int *tour, int totalVertices, int cityZero);
void writeToFile(char *fnameOutput, int *tour, int totalVertices, int tourLength);
void twoOptSearch(int **adjMatrix, int *tour, int *tourLength, char *fnameOutput, int totalVertices);
int tourDistance(int **adjMatrix, int *tour, int totalVertices);
void twoOptSwap(int *newTour, int *tour, int totalVertices, int vertexA, int vertexB);
void swapEfficient(int **adjMatrix, int *tour, int *tourLength, int totalVertices, int vertexA, int vertexB);
unsigned int findRandom(unsigned int minVal, unsigned int maxVal);

#endif /* twoOpt_hpp */
