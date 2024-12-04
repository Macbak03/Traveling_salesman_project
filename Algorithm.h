#pragma once

#include "Graph.h"

class Algorithm {
protected:
    Graph* graph;
    static int calculateDistance(const Node* from, const Node* to);
    virtual std::vector<std::pair<Node*, int>> buildCandidateList(const Node* currentNode, int currentTime) const = 0;

public:
    virtual ~Algorithm() = default;
    explicit Algorithm(Graph* graph);
};
