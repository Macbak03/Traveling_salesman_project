#pragma once

#include <string>
#include "Graph.h"
#include "Algorithm.h"
#include "unordered_set"

struct Outcome {
    int totalTime;
    std::vector<Node*> visitedNodes;

    Outcome() {
        this->totalTime = 0;
    }
};

class BruteForce: public Algorithm {
private:
    Outcome outcome;
    Outcome bestOutcome;
    std::vector<std::pair<Node*, int>> buildCandidateList(const Node* currentNode, int currentTime) const override;
    Outcome performTour(Node* startNode);
    static Node* searchForUnvisitedNeighbor(const std::vector<std::pair<Node*, int>>& candidateList, std::unordered_set<Node*> visited);
    void checkIfBestOutcome();

    const std::string RED = "BLOCKED";
    const std::string YELLOW = "NOT IN TIME";
public:
    using Algorithm::Algorithm;
    explicit BruteForce(Graph *graph);
    Outcome findSolution(Node* startNode);
};