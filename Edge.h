#pragma once
#include "Node.h"

class Edge {
public:
    Node* start;
    Node* end;
    int cost;
    bool blocked;

    Edge(Node* start, Node* end, int cost, bool blocked = false);

    void block();
    void unblock();
};
