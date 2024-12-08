#pragma once
#include "Algorithm.h"
#include <vector>
#include <string>


class Greedy: public Algorithm {
private:
    std::vector<NodeInfo> buildCandidateList(const Node* currentNode, int currentTime) const;
    static std::pair<Node*, bool> selectGreedy(const std::vector<NodeInfo>& candidateNodes);
    static std::pair<Node*, bool> selectGreedyRandomized(const std::vector<NodeInfo>& candidateNodes, int candidateListSize);
    std::vector<NodeInfo> performTour(Node* startNode, bool useRandomized, int candidateListSize = 3) const;

    const std::string RED = "BLOCKED";
    const std::string YELLOW = "NOT IN TIME";
    const std::string BLUE = "ALREADY VISITED";
public:
    using Algorithm::Algorithm;
    std::vector<NodeInfo> findSolution(Node* startNode, bool randomized) const;
};

