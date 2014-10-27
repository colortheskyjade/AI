#include "puzzle.h"
#include <cstdlib>
#include <set>
#include <map>

using namespace std;

Puzzle::Puzzle(int r, int c, int min, int max) 
    : r(r), c(c), min(min), max(max) {
  srand(time(NULL));

  bestScore = 0;
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      bestPuzzle[i][j] = 0;
    }
  }
}

Puzzle::~Puzzle() {};

/**
 * Finds the best puzzle and prints it out.
 */
void Puzzle::findBestPuzzle() {
  timer.StartTimer();
  double estimatedTime = 0;

  while(timer.GetElapsedTime() + (estimatedTime * 1.1) < 57.0) {
    // keep searching if we're approximately under the time threshhold

    // create a random puzzle
    for(int i = 0; i < r; i++) {
      for(int j = 0; j < c; j++) {
        curPuzzle[i][j] = rand() % (max - min + 1) + min;
      }
    }

    int solution, black, white, forward, backward;
    bool unique;
    
    // let's find the heuristic for it
    int curBest = calcMetrics(curPuzzle, solution, unique, black, white, 
      forward, backward);

    // this will store the best candidate per hill climbing step
    int bestCandidate[10][10];
    bool updateBest = false;

    // while there is still some hill climbing to do
    do { 
      // find all possible neighbors
      for(int x = 1; x <= (max - min); x++) {
        for(int i = 0; i < r; i++) {
          for(int j = 0; j < c; j++) {
            // we make a new puzzle
            int newPuzzle[10][10];

            // copy the current puzzle to the new puzzle
            std::copy(&curPuzzle[0][0], &curPuzzle[0][0]+100, &newPuzzle[0][0]);

            // we change one entry in this puzzle
            newPuzzle[i][j] = ((curPuzzle[i][j] + x) % (max - min + 1)) + min;
            int curSol, curBl, curWh, curFor, curBack;
            bool curUnique;

            // and calculate the heuristic of the candidate neighbor
            int candidate = calcMetrics(newPuzzle, curSol, curUnique, curBl, 
              curWh, curFor, curBack);

            // if the candidate heuristic is better, copy everything over
            if (canditate > curBest) {
              curBest = candidate;
              updateBest = true;
              std::copy(&newPuzzle[0][0], &newPuzzle[0][0]+100, 
                &bestCandidate[0][0]);
              solution = curSol; black = curBl; white = curWh; forward = curFor;
              backward = curBack; unique = curUnique;
            }
          }
        }
      }

      if (updateBest) {
        // if we're updating the best, copy the best over to be the current
        std::copy(&bestCandidate[0][0], &bestCandidate[0][0]+100, 
          &curPuzzle[0][0]);
      }
    } while (updateBest);

    // after we've found a local maxima, update the best if necessary
    if (curBest > bestScore) {
      bestScore = curBest;
      std::copy(&curPuzzle[0][0], &curPuzzle[0][0]+100, &bestPuzzle[0][0]);
    }

    if(estimatedTime == 0) {
      estimatedTime = timer.GetElapsedTime();
    }
  }
}

/**
 * Calculates the metrics for a given puzzle configuration.
 * @param  puzzle   Puzzle configuration
 * @param  solution Shortest solution path length
 * @param  unique   If the path is unique
 * @param  black    # black holes
 * @param  white    # white holes
 * @param  forward  # forced forwards
 * @param  backward # forced backwards
 * @return          heuristic given all the above
 */
int Puzzle::calcMetrics(int puzzle[][10], int & solution, bool & unique, 
    int & black, int & white, int & forward, int & backward) {
  map<int, Node> nodes;

  // create the map of nodes we're going to use
  initNodes(puzzle, nodes);

  // find all the reaching nodes
  set<int> reachable = forwardBFS(puzzle, nodes);

  // find all the reachable nodes
  set<int> reaching = backwardBFS(puzzle, nodes);

  // if reachable intersect reaching is not empty, then we have a solution
}

void Puzzle::initNodes(int puzzle[][10], map<int, Node>& nodes){
  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++) {
      nodes.insert(Pair(i*100 + j, temp);
    }
  }

  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++) {
      Node temp = nodes[i*100 + j];
      int val = puzzle[i][j];

      // north
      if (i - val >= 0) {
        // we add that node to the list of forward pointers for our node
        temp.n.insert((i - val) * 100 + j);
        // we add our node to a list of backwards pointers for that node
        nodes[(i - val) * 100 + j].p.insert(i * 100 + j);
      }

      // south
      if (i + val < r) {
        temp.n.insert((i + val) * 100 + j);
        nodes[(i + val) * 100 + j].p.insert(i * 100 + j);
      }

      // west
      if (j - val >= 0) {
        temp.n.insert(i * 100 + (j - val));
        nodes[i * 100 + (j - val)].p.insert(i * 100 + j);
      }

      // east
      if (j + val < c) {
        temp.n.insert(i * 100 + (j + val));
        nodes[i * 100 + (j + val)].p.insert(i * 100 + j);
      }
    }
  }
}