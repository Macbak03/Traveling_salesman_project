//
// Created by macie on 30/11/2024.
//

#include <unordered_set>
#include "BruteForce.h"
#include <iostream>

BruteForce::BruteForce(Graph *graph) : Algorithm(graph) {
    outcome = Outcome();
    bestOutcome = Outcome();
}

std::vector<std::pair<Node *, int>> BruteForce::buildCandidateList(const Node *currentNode, int currentTime) const {
    std::vector<std::pair<Node *, int>> candidateNodes;
    std::cout << "Current Node: " << currentNode->id << ", Time: " << currentTime << std::endl;
    std::cout << "Potential next nodes:" << std::endl;

    for (auto &neighbor: graph->getNeighbors(currentNode)) {
        bool isBlocked = false;
        bool inTimeWindow = true;

        // Check if edge to neighbor is blocked
        for (const auto &edge: graph->edges) {
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
        const int waitTime = (neighbor->timeWindow.start > arrivalTime)
                ? (neighbor->timeWindow.start - arrivalTime): 0;

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
        }
        std::cout << std::endl;
    }
    return candidateNodes;
}

Outcome BruteForce::performTour(Node *startNode) {
    std::unordered_set<Node *> visited;
    Node *currentNode = startNode;
    int currentTime = 0;
    outcome.visitedNodes.push_back(currentNode);

    while (visited.find(startNode) == visited.end()) {
        // Build the list of candidate nodes
        auto candidateNodes = buildCandidateList(currentNode, currentTime);

        // Select the next node based on the chosen strategy
        Node* nextNode = nullptr;
        nextNode = searchForUnvisitedNeighbor(candidateNodes, visited);
        // Move to the next node if a valid one was found
        if(nextNode) {
            outcome.visitedNodes.push_back(nextNode);
            currentTime = std::max(currentTime + calculateDistance(currentNode, nextNode), nextNode->timeWindow.start);
            currentNode = nextNode;
            outcome.totalTime = currentTime;
        } else {
            visited.insert(currentNode);
            checkIfBestOutcome();
            currentNode = startNode;
            currentTime = 0;
        }

    }

    return bestOutcome;
}

Node* BruteForce::searchForUnvisitedNeighbor(const std::vector<std::pair<Node *, int>>& candidateList, std::unordered_set<Node*> visited) {
    for(auto neighbor: candidateList) {
        if(visited.find(neighbor.first) == visited.end()) {
            return neighbor.first;
        }
    }
    return nullptr;
}

void BruteForce::checkIfBestOutcome() {
    if(outcome.visitedNodes.size() > bestOutcome.visitedNodes.size()) {
        bestOutcome = outcome;
    } else if(outcome.visitedNodes.size() == bestOutcome.visitedNodes.size()) {
        if(outcome.totalTime < bestOutcome.totalTime) {
            bestOutcome = outcome;
        }
    }
    outcome.visitedNodes.clear();
    outcome.totalTime = 0;
}

Outcome BruteForce::findSolution(Node *startNode) {
    return performTour(startNode);
}
