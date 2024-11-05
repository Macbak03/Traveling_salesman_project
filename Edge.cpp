#include "Edge.h"

Edge::Edge(Node* start, Node* end, const int cost, const bool blocked): start(start), end(end), cost(cost), blocked(blocked) {}

void Edge::block() {
    blocked = true;
}

void Edge::unblock() {
    blocked = false;
}
