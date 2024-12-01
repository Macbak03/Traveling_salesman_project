#include "Greedy.h"

#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <iomanip>
#include <random>

std::vector<Node*> Greedy::findSolution(Node* startNode, bool randomized) const {
    auto tour = performTour(startNode, randomized);
    return tour;
}

std::vector<std::pair<Node*, int>> Greedy::buildCandidateList(const Node* currentNode, const int currentTime) const {
    std::vector<std::pair<Node*, int>> candidateNodes;
    std::cout << "Current Node: " << currentNode->id << ", Time: " << currentTime << std::endl;
    std::cout << "Potential next nodes:" << std::endl;

    for (auto& neighbor : graph->getNeighbors(currentNode)) {
        bool isBlocked = false;
        bool inTimeWindow = true;

        // Check if edge to neighbor is blocked
        for (const auto& edge : graph->edges) {
            if ((edge->start == currentNode && edge->end == neighbor) ||
                (edge->end == currentNode && edge->start == neighbor)) {
                if (edge->blocked) {
                    isBlocked = true;
                }
                break;
                }
        }

        // Check time window constraint
        const int travelCost = calculateDistance(currentNode, neighbor);
        const int arrivalTime = currentTime + travelCost;
        const int waitTime = (neighbor->timeWindow.start > arrivalTime) ? (neighbor->timeWindow.start - arrivalTime) : 0;

        if (arrivalTime > neighbor->timeWindow.end) {
            inTimeWindow = false;
        }

        std::string color;
        if (isBlocked) {
            color = RED;
        } else if (!inTimeWindow) {
            color = YELLOW;
        }

        std::cout << color << " - Node " << neighbor->id
                  << " (Cost: " << travelCost
                  << ", Arrival: " << arrivalTime
                  << ", Time Window: [" << neighbor->timeWindow.start << ", " << neighbor->timeWindow.end << "]"
                  << ", Waiting time: " << waitTime
                  << ", total totalTime: " << waitTime + travelCost << ")";

        if (!isBlocked && inTimeWindow) {
            candidateNodes.emplace_back(neighbor, travelCost + waitTime);
            std::cout << " -> Valid choice" << std::endl;
        } else {
            std::cout << std::endl;
        }

    }

    return candidateNodes;
}

Node* Greedy::selectGreedy(const std::vector<std::pair<Node*, int>>& candidateNodes) {
    if (candidateNodes.empty()) return nullptr;
    return std::ranges::min_element(candidateNodes,
                                    [](const auto& lhs, const auto& rhs){ return lhs.second < rhs.second; })->first;
}

Node* Greedy::selectGreedyRandomized(const std::vector<std::pair<Node*, int>>& candidateNodes, int candidateListSize) {
    if (candidateNodes.empty()) return nullptr;
    std::mt19937 rng(std::random_device{}());

    // Sort by totalTime to get the top candidates
    std::vector<std::pair<Node*, int>> sortedCandidates = candidateNodes;
    std::ranges::sort(sortedCandidates,
                      [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });

    // Choose randomly among the top `candidateListSize` candidates
    const int selectionRange = std::min(static_cast<int>(sortedCandidates.size()), candidateListSize);
    std::uniform_int_distribution<int> dist(0, selectionRange - 1);
    return sortedCandidates[dist(rng)].first;
}

std::vector<Node*> Greedy::performTour(Node* startNode, bool useRandomized, int candidateListSize) const {
    std::vector<Node*> tour;
    std::unordered_set<Node*> visited;
    Node* currentNode = startNode;
    int currentTime = 0;

    tour.push_back(currentNode);
    visited.insert(currentNode);

    while (tour.size() < graph->nodes.size()) {
        // Build the list of candidate nodes
        auto candidateNodes = buildCandidateList(currentNode, currentTime);

        // Select the next node based on the chosen strategy
        Node* nextNode = nullptr;
        if (useRandomized) {
            nextNode = selectGreedyRandomized(candidateNodes, candidateListSize);
        } else {
            nextNode = selectGreedy(candidateNodes);
        }

        // Move to the next node if a valid one was found
        if (nextNode) {
            tour.push_back(nextNode);
            visited.insert(nextNode);
            currentTime = std::max(currentTime + calculateDistance(currentNode, nextNode), nextNode->timeWindow.start);
            currentNode = nextNode;
        } else {
            //std::cerr << "Warning: No feasible next node due to time window constraints or blocked paths." << std::endl;
            break;
        }
    }

    // Return to start if possible
    if (tour.size() == graph->nodes.size() && !graph->getNeighbors(currentNode).empty()) {
        tour.push_back(startNode);
    }

    return tour;
}


