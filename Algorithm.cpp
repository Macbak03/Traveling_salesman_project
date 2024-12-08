//
// Created by macie on 30/11/2024.
//

#include "Algorithm.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <vector>


Algorithm::Algorithm(Graph *graph) : graph(graph) {}

int Algorithm::calculateDistance(const Node *from, const Node *to) {
    const int x = from->x - to->x;
    const int y = from->y - to->y;
    return static_cast<int>(std::sqrt(x*x + y*y));
}

int Algorithm::calculateVisitedNodes(const std::vector<NodeInfo>& route) {
    const int routeVisitedCount = static_cast<int>(std::ranges::count_if(
       route, [](const NodeInfo& routeInfo) {
           return routeInfo.canVisit;
       }));
    return routeVisitedCount;
}

int Algorithm::calculateVisitedAndPassedNodes(const std::vector<NodeInfo>& route) {
    return static_cast<int>(route.size());
}

int Algorithm::calculateRouteCost(const std::vector<NodeInfo>& route) {
    int totalCost = 0;
    if (!route.empty()) {
        totalCost = route.back().totalCost;
    }
    return totalCost;
}
