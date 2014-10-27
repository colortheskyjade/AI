#include "node.h"

using namespace std;

Node::Node() {
    reachable = false;
    reaching  = false;
}

Node::Node(int r, int c) {
    reachable = false;
    reaching  = false;
}

Node::~Node(){

}