//
// Created by macie on 30/11/2024.
//

#include "Algorithm.h"
#include <cmath>

Algorithm::Algorithm(Graph *graph) : graph(graph) {}

int Algorithm::calculateDistance(const Node *from, const Node *to) {
    const int x = from->x - to->x;
    const int y = from->y - to->y;
    return static_cast<int>(std::sqrt(x*x + y*y));
}
