//
// Created by macie on 30/11/2024.
//

#include <unordered_set>
#include "BruteForce.h"

#include <algorithm>
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
    std::vector<Node*> nodes = graph->nodes;
    std::erase(nodes, startNode); // Exclude the start node
    int tourCount = 1;
    do {
        // Initialize variables for this permutation
        Node* currentNode = startNode;
        outcome.visitedNodes.push_back(currentNode);
        int currentTime = 0;
        int totalCost = 0;

        bool valid = false;





        // Traverse nodes in the current permutation order
        for (Node* nextNode : nodes) {
            // Check if the edge between currentNode and nextNode is blocked
            if (const Edge* edge = findEdge(currentNode, nextNode); !edge || edge->blocked) {
                valid = false;
                break;
            }

            // Calculate travel time, arrival time, and waiting time
            const int travelCost = calculateDistance(currentNode, nextNode);
            const int arrivalTime = currentTime + travelCost;
            if (arrivalTime > nextNode->timeWindow.end) {
                valid = false;
                break; // Violates the time window
            }
            const int waitTime = std::max(0, nextNode->timeWindow.start - arrivalTime);
            totalCost += travelCost + waitTime;

            // Update current time and current node
            currentTime = arrivalTime + waitTime;
            outcome.visitedNodes.push_back(nextNode);
            outcome.totalTime = totalCost;
            currentNode = nextNode;
        }

        // Check the final edge back to the startNode
        if (const Edge* returnEdge = findEdge(currentNode, startNode); !returnEdge || returnEdge->blocked) {
            valid = false;
            break;
        }
        totalCost += calculateDistance(currentNode, startNode);
        outcome.totalTime = totalCost;

        //print outcome:
        std::cout << "tour: " << tourCount++ << std::endl;
        for (const Node* node : outcome.visitedNodes) {
            std::cout << node->id << " ";
        } std::cout << "cost: " << outcome.totalTime << std::endl;

        // If the tour is better than the current best, update the best outcome
        checkIfBestOutcome();

    } while (std::ranges::next_permutation(nodes).found);

    return bestOutcome;
}

Edge* BruteForce::findEdge(const Node* startNode, const Node* endNode) const {
    for (auto* edge : graph->edges) {
        if ((edge->start == startNode && edge->end == endNode) ||
            (edge->end == startNode && edge->start == endNode)) {
            return edge;
            }
    }
    return nullptr; // No edge found
}

Node* BruteForce::searchForUnvisitedNeighbor(const std::vector<std::pair<Node *, int>>& candidateList, const std::unordered_set<Node*>& visited) {
    for(auto neighbor: candidateList) {
        if(!visited.contains(neighbor.first)) {
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
