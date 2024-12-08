#include "SimulatedAnnealing.h"

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>

std::pair<int, std::vector<NodeInfo>> SimulatedAnnealing::calculateRoute(const std::vector<Node*>& route, Node* startNode) const {
    int currentTime = 0;

    std::vector<NodeInfo> routeInfo;
    routeInfo.emplace_back(startNode, true);

    Node* from = startNode;
    std::vector<Node*> finalRoute = route;
    startNode->visited = true;

    for (Node* to : finalRoute) {
        bool edgeFound = false;
        if (to->visited) {
            continue;
        }
        for (const Edge* edge : graph->edges) {
            if ((edge->start == from && edge->end == to) ||
                (edge->start == to && edge->end == from)) {
                edgeFound = true;

                if (edge->blocked) {
                    continue; // if the edge is blocked, find different edge
                }
                currentTime += edge->cost;

                // checks if node is in time window
                const bool canVisit = (currentTime >= to->timeWindow.start && currentTime <= to->timeWindow.end);
                routeInfo.emplace_back(to, canVisit, currentTime);
                to->visited = true;
                from = to;

                break;
                }
        }



        if (!edgeFound) {
            return {currentTime, routeInfo}; // if no edges between nodes, finish the tour
        }
    }
    if (const Edge* returnEdge = graph->findEdge(from, startNode); !returnEdge || returnEdge->blocked) {
        return {currentTime, routeInfo};
    }
    currentTime += calculateDistance(from, startNode);
    routeInfo.emplace_back(startNode, true, currentTime);
    return {currentTime, routeInfo};
}

std::vector<Node*> SimulatedAnnealing::generateNeighbor(const std::vector<Node*>& currentRoute) {
    std::vector<Node*> neighbor = currentRoute;

    // 2 nodes changes place with each other
    const size_t i = rand() % neighbor.size();
    const size_t j = rand() % neighbor.size();
    std::swap(neighbor[i], neighbor[j]);

    return neighbor;
}


std::pair<std::vector<NodeInfo>, int> SimulatedAnnealing::simulateAnnealing(
    Node* startNode,
    const double initialTemp,
    const double coolingRate,
    const int maxIterations
) const {
    std::vector<Node*> currentSolution = graph->nodes;
    std::vector<NodeInfo> bestRouteInfo;
    int bestVisitedCount = 0;
    int bestTotalCount = 0;
    int bestCost = INT_MAX;

    auto currentMetrics = calculateRoute(currentSolution, startNode);
    graph->unvisitNodes();
    int currentCost = currentMetrics.first;
    auto currentRouteInfo = currentMetrics.second;

    double temperature = initialTemp;

    for (int iter = 0; iter < maxIterations && temperature > 1; ++iter) {

        std::vector<Node*> neighbor = generateNeighbor(currentSolution);
        const auto neighborMetrics = calculateRoute(neighbor, startNode);
        graph->unvisitNodes();

        const int neighborCost = neighborMetrics.first;
        auto neighborRouteInfo = neighborMetrics.second;

        const int neighborVisitedCount = static_cast<int>(std::ranges::count_if(neighborRouteInfo, [](const NodeInfo& info) {
            return info.canVisit;
        }));
        const int neighborTotalCount = static_cast<int>(neighborRouteInfo.size());

        const int currentVisitedCount = static_cast<int>(std::ranges::count_if(currentRouteInfo, [](const NodeInfo& info) {
            return info.canVisit;
        }));
        const int currentTotalCount = static_cast<int>(currentRouteInfo.size());

        // comparing solutions (priority: number of visited nodes, then visited + passed by, then cost)
        bool acceptNeighbor = false;
        if (neighborVisitedCount > currentVisitedCount || (neighborVisitedCount == currentVisitedCount && neighborTotalCount > currentTotalCount) ||
            (neighborVisitedCount == currentVisitedCount && neighborCost < currentCost)) {
            acceptNeighbor = true;
        } else {
            const double probability = exp(
                (currentVisitedCount - neighborVisitedCount + (currentTotalCount - neighborTotalCount) +
                (currentCost - neighborCost)/ 1000.0) / temperature
            );
            if (probability > (static_cast<double>(rand()) / RAND_MAX)) {
                acceptNeighbor = true;
            }
        }

        if (acceptNeighbor) {
            currentSolution = neighbor;
            currentMetrics = neighborMetrics;
            currentCost = currentMetrics.first;
            currentRouteInfo = currentMetrics.second;

            if (neighborVisitedCount > bestVisitedCount || (neighborVisitedCount == bestVisitedCount && neighborTotalCount > bestTotalCount) ||
                (neighborVisitedCount == bestVisitedCount && neighborCost < bestCost)) {
                bestRouteInfo = neighborRouteInfo;
                bestVisitedCount = neighborVisitedCount;
                bestTotalCount = neighborTotalCount;
                bestCost = neighborCost;
            }
        }

        temperature *= coolingRate;
    }

    return {bestRouteInfo, bestCost};
}
