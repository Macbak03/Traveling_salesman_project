#pragma once
#include "Graph.h"
#include <vector>
#include <string>


class Greedy {
private:
    Graph* graph;
    static int calculateDistance(const Node* from, const Node* to) ;
    //std::vector<Node*> nearestNeighbor(Node* startNode) const;
    //static void twoOptOptimization(std::vector<Node*>& tour);

    std::vector<std::pair<Node*, int>> buildCandidateList(const Node* currentNode, int currentTime) const;
    static Node* selectGreedy(const std::vector<std::pair<Node*, int>>& candidateNodes);
    static Node* selectGreedyRandomized(const std::vector<std::pair<Node*, int>>& candidateNodes, int candidateListSize);
    std::vector<Node*> performTour(Node* startNode, bool useRandomized, int candidateListSize = 3) const;

    const std::string RED = "BLOCKED";
    const std::string YELLOW = "NOT IN TIME";
public:
    explicit Greedy(Graph* graph);
    std::vector<Node*> findSolution(Node* startNode, bool randomized) const;
};

