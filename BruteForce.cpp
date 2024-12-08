//
// Created by macie on 30/11/2024.
//

#include "BruteForce.h"

#include <algorithm>

std::vector<NodeInfo> BruteForce::performTour(Node* startNode) {
    std::vector<Node*> nodes = graph->nodes;
    std::erase(nodes, startNode); // Exclude the start node
    do {
        // Initialize variables for this permutation
        Node* currentNode = startNode;
        std::vector<NodeInfo> routeInfo;
        routeInfo.emplace_back(currentNode, true, 0);
        int currentTime = 0;

        // Traverse nodes in the current permutation order
        for (Node* nextNode : nodes) {
            // Check if the edge between currentNode and nextNode is blocked
            if (const Edge* edge = graph->findEdge(currentNode, nextNode); !edge || edge->blocked) {
                break;
            }

            // Calculate travel time
            const int travelCost = calculateDistance(currentNode, nextNode);
            currentTime += travelCost;
            const bool canVisit = (currentTime >= nextNode->timeWindow.start && currentTime <= nextNode->timeWindow.end);
            routeInfo.emplace_back(nextNode, canVisit, currentTime);
            currentNode = nextNode;
        }

        // Check the final edge back to the startNode
        if (const Edge* returnEdge = graph->findEdge(currentNode, startNode); !returnEdge || returnEdge->blocked) {
            checkIfBestOutcome(routeInfo);
        } else {
            currentTime += calculateDistance(currentNode, startNode);
            routeInfo.emplace_back(startNode, true, currentTime);

            checkIfBestOutcome(routeInfo);
        }

    }
    while (std::ranges::next_permutation(nodes).found);

    return bestRoute;
}

void BruteForce::checkIfBestOutcome(const std::vector<NodeInfo>& currentRoute) {
    const int currentRouteVisitedCount = calculateVisitedNodes(currentRoute);
    const int currentRouteTotalCount = calculateVisitedAndPassedNodes(currentRoute);
    const int currentRouteCost = calculateRouteCost(currentRoute);

    const int bestRouteVisitedCount = calculateVisitedNodes(bestRoute);
    const int bestRouteTotalCount = calculateVisitedAndPassedNodes(bestRoute);
    const int bestRouteCost = calculateRouteCost(bestRoute);

    if (currentRouteVisitedCount > bestRouteVisitedCount ||
        (currentRouteVisitedCount == bestRouteVisitedCount && currentRouteTotalCount > bestRouteTotalCount) ||
        (currentRouteVisitedCount == bestRouteVisitedCount && currentRouteTotalCount == bestRouteTotalCount &&
            currentRouteCost < bestRouteCost)
    ) {
        bestRoute = currentRoute;
    }
}

std::vector<NodeInfo> BruteForce::findSolution(Node* startNode) {
    return performTour(startNode);
}
