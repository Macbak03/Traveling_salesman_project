#pragma once

#include "Graph.h"
#include "Algorithm.h"

class BruteForce final : public Algorithm {
private:
    std::vector<NodeInfo> bestRoute;
    std::vector<NodeInfo> performTour(Node* startNode);
    void checkIfBestOutcome(const std::vector<NodeInfo>& currentRoute);
public:
    using Algorithm::Algorithm;
    std::vector<NodeInfo> findSolution(Node* startNode);
};