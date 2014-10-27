/*
NAME: Ruyan Chen
STUDENT ID: 2890440150
*/

#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include "Timer.h"
#include "node.h"

using namespace std;

int getHeuristic(nodeArray);

void GeneratePuzzle(int nRows, int nColumns, int minVal, int maxVal)
{
	Puzzle p(nRows, nColumns, minVal, maxVal);	
	p.findBestPuzzle();
}

int main(int argc, char **argv)
{
	// Process the arguments
	if (argc != 5)
	{
		printf("Please specify the number of rows and columns and the minimum and maximum values for each cell (requires 4 parameters)\n");
		exit(0);
	}
	int nRows = atoi(argv[1]);
	int nColumns = atoi(argv[2]);
	int minVal = atoi(argv[3]);
	int maxVal = atoi(argv[4]);

	// Start the timer
	Timer t;
	t.StartTimer();
  	
	// Generate the puzzle
	printf("Generating a %dx%d puzzle with values in range [%d-%d]\n", nRows, nColumns, minVal, maxVal);
	
	GeneratePuzzle(nRows, nColumns, minVal, maxVal);
		
	// Print the elapsed time
    printf("Total time: %.6lf seconds\n", t.GetElapsedTime());

	return 0;
}