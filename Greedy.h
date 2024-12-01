#pragma once
#include "Graph.h"
#include "Algorithm.h"
#include <vector>
#include <string>


class Greedy: public Algorithm {
private:
    //std::vector<Node*> nearestNeighbor(Node* startNode) const;
    //static void twoOptOptimization(std::vector<Node*>& tour);

    std::vector<std::pair<Node*, int>> buildCandidateList(const Node* currentNode, int currentTime) const override;
    static Node* selectGreedy(const std::vector<std::pair<Node*, int>>& candidateNodes);
    static Node* selectGreedyRandomized(const std::vector<std::pair<Node*, int>>& candidateNodes, int candidateListSize);
    std::vector<Node*> performTour(Node* startNode, bool useRandomized, int candidateListSize = 3) const;

    const std::string RED = "BLOCKED";
    const std::string YELLOW = "NOT IN TIME";
public:
    using Algorithm::Algorithm;
    std::vector<Node*> findSolution(Node* startNode, bool randomized) const;
};

