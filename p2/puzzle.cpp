#include "puzzle.h"
#include <cstdlib>
#include <set>
#include <map>
#include <queue>
#include <algorithm> 
#include <iostream>

using namespace std;

Puzzle::Puzzle(int r, int c, int min, int max) 
    : r(r), c(c), min(min), max(max) {
  srand(time(NULL));

  // there's no point in trying to deal with values
  // greater than the larger dimension of the array
  // e.g. (0,0) can at most move to (r,0) or (0,c)
  if (max > std::max(r, c)) {
    max = std::max(r, c) - 1;
  }

  bestScore = -100000;
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      bestPuzzle[i][j] = 0;
    }
  }
}

Puzzle::~Puzzle() {}

/**
 * Finds the best puzzle and prints it out.
 */
void Puzzle::findBestPuzzle() {
  timer.StartTimer();
  double estimatedTime = 0;

  while(timer.GetElapsedTime() + estimatedTime < 58.0) {
    // keep searching if we're approximately under the time threshhold

    // create a random puzzle
    for(int i = 0; i < r; i++) {
      for(int j = 0; j < c; j++) {
        curPuzzle[i][j] = rand() % (max - min + 1) + min;
      }
    }
    curPuzzle[r-1][c-1] = 0;

    int solution = 0, black = 0, white = 0, forward = 0, backward = 0;
    bool unique = false;
    
    // let's find the heuristic for it
    int curBest = calcMetrics(curPuzzle, solution, unique, black, white, 
      forward, backward);
    // this will store the best candidate per hill climbing step
    int bestCandidate[10][10];

    bool updateBest = false;
    // while there is still some hill climbing to do
    do { 
      updateBest = false;
      // find all possible neighbors
      for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
          int x = rand() % (max - min + 1);
          // we can't change the goal value
          if(i == r-1 && j == c -1) {
            continue;
          }
          // we make a new puzzle
          int newPuzzle[10][10];

          // copy the current puzzle to the new puzzle
          std::copy(&curPuzzle[0][0], &curPuzzle[0][0]+100, &newPuzzle[0][0]);

          // we change one entry in this puzzle
          newPuzzle[i][j] = ((curPuzzle[i][j] + x) % (max - min + 1)) + min;
          int curSol = 0, curBl = 0, curWh = 0, curFor = 0, curBack = 0;
          bool curUnique = false;

          // and calculate the heuristic of the candidate neighbor
          int candidate = calcMetrics(newPuzzle, curSol, curUnique, curBl, 
            curWh, curFor, curBack);

          // if the candidate heuristic is better, copy everything over
          if (candidate > curBest) {
            curBest = candidate;
            updateBest = true;
            std::copy(&newPuzzle[0][0], &newPuzzle[0][0]+100, 
              &bestCandidate[0][0]);
            solution = curSol; black = curBl; white = curWh; forward = curFor;
            backward = curBack; unique = curUnique;
          } 
        }
      }

      if (updateBest) {
        // if we're updating the best, copy the best over to be the current
        std::copy(&bestCandidate[0][0], &bestCandidate[0][0]+100, 
          &curPuzzle[0][0]);
      } else if (rand() % 7 == 0) {
        // there is a random chance that if we haven't found a better one, 
        // we'll try one anyways
        curPuzzle[rand() % r][rand() % c] = rand() % (max - min + 1) + min;
        curPuzzle[r - 1][c - 1] = 0;
        curBest = calcMetrics(curPuzzle, solution, unique, black, white, 
          forward, backward);
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


  int solution = 0, black = 0, white = 0, forward = 0, backward = 0;
  bool unique = false;
  
  // let's find the heuristic for it
  int score = calcMetrics(bestPuzzle, solution, unique, black, white, forward, backward);

  // print out the best puzzle
  cout << "Puzzle:" << endl;
  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++) {
      cout << bestPuzzle[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
  cout << "Solution: " << ((solution > 0) ? "Yes" : "No") << endl;
  cout << "Unique: " << (unique? "Yes" : "No") << endl;
  cout << "Solution length: " << solution << endl;
  cout << "# of black holes: " << black << endl;
  cout << "# of white holes: " << white << endl;
  cout << "# of forced forward moves: " << forward << endl;
  cout << "# of forced backward moves: " << backward << endl;
  cout << "Puzzle score: " << score << endl << endl;

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
  set<int> reachable = forwardBFS(puzzle, nodes, solution, unique);

  // find all the reachable nodes
  set<int> reaching = backwardBFS(puzzle, nodes);

  // black holes are reachable but not reaching nodes
  set<int> blackHoles;
  std::set_difference(reachable.begin(), reachable.end(), reaching.begin(), 
    reaching.end(), std::inserter(blackHoles, blackHoles.end()));
  black = blackHoles.size();
  if(blackHoles.find((r-1)*100 + (c-1)) != blackHoles.end()) {
    black--;
  }

  // white hole sare reaching but not reachable nodes
  set<int> whiteHoles;
  std::set_difference(reaching.begin(), reaching.end(), reachable.begin(), 
    reachable.end(), std::inserter(whiteHoles, whiteHoles.end()));
  white = whiteHoles.size();
  if(whiteHoles.find(0) != whiteHoles.end()) {
    white--;
  }

  forward = 0;
  // forced forward nodes are reachable nodes with only one outgoing edge
  for(set<int>::iterator it = reachable.begin(); it != reachable.end(); ++it) {
    if(*it == (r-1)*100 + (c-1)){
      continue;
    }
    if(nodes[*it].n.size() == 1){
      forward++;
    }
  }

  backward = 0;
  // forced backwards are reaching nodes with only one incoming edge
  for(set<int>::iterator it = reaching.begin(); it != reaching.end(); ++it) {
    if(*it == 0) {
      continue;
    }
    if(nodes[*it].p.size() == 1){
      backward++;
    }
  }

  if(solution == -1) {
    // we don't have a solution
    return 0;
  }

  int h = 5*(solution); 
  if (unique) {
    h += r*c;
  }
  h -= white * 2;
  h -= black * 2;
  h -= forward * 2;
  h -= backward * 2;
  return h;
}

/**
 * Initializes the map of nodes.
 * @param puzzle The 2d array puzzle.
 * @param nodes  The map of nodes.
 */
void Puzzle::initNodes(int puzzle[][10], map<int, Node>& nodes) {
  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++) {
      nodes.insert(pair<int, Node>(i*100 + j, Node(i, j)));
    }
  }

  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++) {
      Node temp = nodes[i*100 + j];
      int val = puzzle[i][j];
      if(val == 0) {
        continue;
      }

      // cout << i << " , " << j << endl;
      // north
      if (i - val >= 0) {
        // we add that node to the list of forward pointers for our node
        nodes[i*100 + j].n.insert((i - val) * 100 + j);
        // we add our node to a list of backwards pointers for that node
        nodes[(i - val) * 100 + j].p.insert(i * 100 + j);
      }

      // south
      if (i + val < r) {
        nodes[i*100 + j].n.insert((i + val) * 100 + j);
        nodes[(i + val) * 100 + j].p.insert(i * 100 + j);
      }

      // west
      if (j - val >= 0) {
        nodes[i*100 + j].n.insert(i * 100 + (j - val));
        nodes[i * 100 + (j - val)].p.insert(i * 100 + j);
      }

      // east
      if (j + val < c) {
        nodes[i*100 + j].n.insert(i * 100 + (j + val));
        nodes[i * 100 + (j + val)].p.insert(i * 100 + j);
      }
    }
  }
}


/**
 * Initializes the map of nodes from the start.
 * @param puzzle The 2d array puzzle.
 * @param nodes  The map of nodes.
 * @param goal   The number of steps before we expand the goal.
 * @param unique If the solution is unique.
 * @return       A set of reachable nodes.
 */
set<int> Puzzle::forwardBFS(int puzzle[][10], map<int, Node>& nodes, int & goal, 
    bool & unique) {
  queue<int> q;
  set<int> reachable;
  int dist[10][10];

  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      dist[i][j] = -1;
    }
  }
  dist[0][0] = 0;
  unique = true;

  // add start to the set
  q.push(0);
  while(!q.empty()) {
    // take the top of the queue
    int temp = q.front(); 
    q.pop();

    // if we haven't already found this node, expand it
    if(reachable.find(temp) == reachable.end()) {
      // add it to the reachable nodes
      reachable.insert(temp);

      // add its children to the queue
      set<int> children = nodes[temp].n;
      for(set<int>::iterator it = children.begin(); it != children.end(); ++it) 
      {
        if(reachable.find(*it) == reachable.end()) {
          q.push(*it);
        }

        // update the distance of the children
        if(dist[*it/100][*it % 100] == -1) {
          dist[*it/100][*it % 100] = dist[temp/100][temp%100] + 1;
        }
        // if one of the children is the goal node and this solution also has
        // the same length, then we don't have a unique solution
        else if(*it == ((r-1) * 100 + (c-1))) {
          if(dist[r-1][c-1] == dist[temp/100][temp%100] + 1){
            unique = false;
          }
        }
      }
    }
  }

  goal = dist[r-1][c-1];
  if(goal == 0) {
    unique = false;
  }
  return reachable;
}


/**
 * Initializes the map of nodes from the end.
 * @param puzzle The 2d array puzzle.
 * @param nodes  The map of nodes.
 * @return       A set of reaching nodes.
 */
set<int> Puzzle::backwardBFS(int puzzle[][10], map<int, Node>& nodes) {
  queue<int> q;
  set<int> reaching;

  // add goal to the set
  q.push((r-1)*100 + c-1);
  while(!q.empty()) {
    // take the top of the queue
    int temp = q.front(); 
    q.pop();

    // if we haven't already found this node, expand it
    if(reaching.find(temp) == reaching.end()) {
      // add it to the reaching nodes
      reaching.insert(temp);

      // add its parents to the queue
      set<int> parents = nodes[temp].p;
      for(set<int>::iterator it = parents.begin(); it != parents.end(); ++it) {
        q.push(*it);
      }
    }
  }

  return reaching;
}