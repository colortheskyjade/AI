#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <set>
#include <map>
#include "Timer.h"
#include "node.h"

class Puzzle {
  public:
    Puzzle(int r, int c, int min, int max);
    ~Puzzle();

    void findBestPuzzle();

    // helper functions
    int calcMetrics(int[][10], int&, bool&, int&, int&, int&, int&);
    void initNodes(int[][10], std::map<int, Node>&);
    std::set<int> forwardBFS(int[][10], std::map<int, Node>&, int&, bool&);
    std::set<int> backwardBFS(int[][10], std::map<int, Node>&);

    // data members
    Timer timer;
    int bestPuzzle[10][10];
    int curPuzzle[10][10];
    int r, c, min, max, bestScore;
};

#endif