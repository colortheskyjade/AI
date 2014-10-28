#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <set>

class Node {
  public:
    Node();
    Node(int, int);
    ~Node();

    std::set<int> p;
    std::set<int> n;

    bool reachable;
    bool reaching;
};

#endif
