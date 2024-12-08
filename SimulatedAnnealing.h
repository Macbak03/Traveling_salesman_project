#pragma once
#include "Algorithm.h"

class SimulatedAnnealing: public Algorithm{
private:
    std::pair<int, std::vector<NodeInfo>> calculateRoute(const std::vector<Node*>& route, Node* startNode) const;
    static std::vector<Node*> generateNeighbor(const std::vector<Node*>& currentRoute);
public:
    using Algorithm::Algorithm;
    std::pair<std::vector<NodeInfo>, int> simulateAnnealing(Node* startNode, double initialTemp, double coolingRate, int maxIterations) const;
};
