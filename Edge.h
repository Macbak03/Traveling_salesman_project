#pragma once
#include "Node.h"

class Edge {
private:
    static int calculateCost(const Node* from, const Node* to);
public:
    Node* start;
    Node* end;
    int cost;
    bool blocked;

    Edge(Node* start, Node* end, int cost, bool blocked = false);
    Edge(Node* start, Node* end, bool blocked = false);

    void block();
    void unblock();
};
