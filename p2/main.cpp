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
#include "puzzle.h"

using namespace std;

void GeneratePuzzle(int nRows, int nColumns, int minVal, int maxVal)
{
	Puzzle p(nRows, nColumns, minVal, maxVal);	
	cout << endl; // formatting
	p.findBestPuzzle();

	// DEBUG
	// int arr[10][10] =	 {2, 5, 6, 5, 4, 2, 6, 2, 2, 6,
	// 				1, 1, 3, 1, 2, 2, 2, 7, 1, 3,
	// 				8, 7, 4, 6, 2, 2, 2, 7, 9, 5,
	// 				5, 5, 7, 6, 3, 4, 5, 3, 9, 9,
	// 				1, 4, 4, 2, 8, 7, 2, 4, 6, 1,
	// 				2, 1, 2, 5, 8, 3, 8, 4, 3, 4,
	// 				2, 3, 6, 2, 2, 8, 4, 1, 6, 6,
	// 				1, 9, 4, 4, 2, 3, 2, 2, 7, 1,
	// 				6, 1, 6, 5, 1, 4, 5, 5, 8, 5,
	// 				4, 7, 6, 3, 4, 8, 4, 5, 2, 0};
	// int sol, black, white, forward, backward;
	// bool unique;
	// p.calcMetrics(arr, sol, unique, black, white, forward, backward);
	// cout << sol << " " << black << " " << white << " " << forward << " " << backward << endl;
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
