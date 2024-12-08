#include "Greedy.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>


std::vector<NodeInfo> Greedy::findSolution(Node* startNode, const bool randomized) const {
    auto tour = performTour(startNode, randomized);
    return tour;
}

std::vector<NodeInfo> Greedy::buildCandidateList(const Node* currentNode, const int currentTime) const {
    std::vector<NodeInfo> candidateNodes;
    std::cout << "Current Node: " << currentNode->id << ", Time: " << currentTime << std::endl;
    std::cout << "Potential next nodes:" << std::endl;

    for (auto& neighbor : graph->getNeighbors(currentNode)) {
        bool isBlocked = false;
        const bool isVisited = neighbor->visited;

        // Check if edge to neighbor is blocked
        const Edge* edge = graph->findEdge(currentNode, neighbor);
        if (!edge) {
            break;
        }
        if (edge->blocked) {
            isBlocked = true;
        }

        // Check time window constraint
        const int travelCost = calculateDistance(currentNode, neighbor);
        const int arrivalTime = currentTime + travelCost;

        const bool canVisit = (arrivalTime >= neighbor->timeWindow.start && arrivalTime <= neighbor->timeWindow.end);

        std::string color;
        if (isBlocked) {
            color = RED;
        }
        else if (!canVisit) {
            color = YELLOW;
        }
        else if (isVisited) {
            color = BLUE;
        }

        std::cout << color << " - Node " << neighbor->id
            << " (Cost: " << travelCost
            << ", Arrival: " << arrivalTime
            << ", Time Window: [" << neighbor->timeWindow.start << ", " << neighbor->timeWindow.end << "]";

        if (!isBlocked && !isVisited) {
            candidateNodes.emplace_back(neighbor, canVisit, travelCost);
            std::cout << " -> Valid choice" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
    }

    return candidateNodes;
}

std::pair<Node*, bool> Greedy::selectGreedy(const std::vector<NodeInfo>& candidateNodes) {
    // 1. Prefer nodes that can be visited.
    // 2. Among nodes with the same can visit status, prefer lower cost.
    const auto candidateToVisit = std::ranges::min_element(candidateNodes,
    [](const NodeInfo& route1, const NodeInfo& route2) {
        if (route1.canVisit != route2.canVisit) {
            return route1.canVisit > route2.canVisit;
        }
        return route1.totalCost < route2.totalCost;
    });
    if (candidateToVisit != candidateNodes.end()) {
        return {candidateToVisit->node, candidateToVisit->canVisit};
    }
    //return null if no such node found
    return {nullptr, false};
}

std::pair<Node*, bool> Greedy::selectGreedyRandomized(const std::vector<NodeInfo>& candidateNodes, int candidateListSize) {
    if (candidateNodes.empty()) return {nullptr, false};
    std::mt19937 rng(std::random_device{}());

    // Sort candidates by hierarchical priority:
    // 1. Prefer nodes that can be visited.
    // 2. Among nodes with the same can visit status, prefer lower cost.
    std::vector<NodeInfo> sortedCandidates = candidateNodes;
    std::ranges::sort(sortedCandidates, [](const NodeInfo& a, const NodeInfo& b) {
        if (a.canVisit != b.canVisit) {
            return a.canVisit > b.canVisit;
        }
        return a.totalCost < b.totalCost;
    });

    const int selectionRange = std::min(static_cast<int>(sortedCandidates.size()), candidateListSize);
    std::uniform_int_distribution<int> dist(0, selectionRange - 1);
    return {sortedCandidates[dist(rng)].node, sortedCandidates[dist(rng)].canVisit} ;
}

std::vector<NodeInfo> Greedy::performTour(Node* startNode, bool useRandomized, int candidateListSize) const {
    std::vector<NodeInfo> route;
    Node* currentNode = startNode;
    int currentTime = 0;

    route.emplace_back(currentNode, true, currentTime);
    currentNode->visited = true;

    while (route.size() <= graph->nodes.size() + 1) {
        // Build the list of candidate nodes
        auto candidateNodes = buildCandidateList(currentNode, currentTime);

        // Select the next node based on the chosen strategy
        Node* nextNode = nullptr;
        bool canVisit = false;
        if (useRandomized) {
            auto [node, visit] = selectGreedyRandomized(candidateNodes, candidateListSize);
            nextNode = node;
            canVisit = visit;
        }
        else {
            auto [node, visit] = selectGreedy(candidateNodes);
            nextNode = node;
            canVisit = visit;
        }

        // Move to the next node if a valid one was found
        if (nextNode) {
            route.emplace_back(nextNode, canVisit, currentTime);
            nextNode->visited = true;
            currentTime += calculateDistance(currentNode, nextNode);
            currentNode = nextNode;
        }
        else {
            break;
        }
    }

    // Return to start if possible
    if (const Edge* returnEdge = graph->findEdge(currentNode, startNode); !returnEdge || returnEdge->blocked) {
        return route;
    }
    currentTime += calculateDistance(currentNode, startNode);
    route.emplace_back(startNode, true, currentTime);
    return route;
}
