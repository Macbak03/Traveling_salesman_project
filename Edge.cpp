#include "Edge.h"
#include <cmath>

int Edge::calculateCost(const Node* from, const Node* to) {
    const int x = from->x - to->x;
    const int y = from->y - to->y;
    return static_cast<int>(std::sqrt(x*x + y*y));
}

Edge::Edge(Node* start, Node* end, const int cost, const bool blocked): start(start), end(end), cost(cost), blocked(blocked) {}

Edge::Edge(Node* start, Node* end, const bool blocked): start(start), end(end), blocked(blocked) {
    cost = calculateCost(start, end);
}

void Edge::block() {
    blocked = true;
}

void Edge::unblock() {
    blocked = false;
}
