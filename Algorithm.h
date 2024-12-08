#pragma once

#include "Graph.h"
#include "NodeInfo.h"

class Algorithm {
protected:
    Graph* graph;
    static int calculateDistance(const Node* from, const Node* to);

public:
    virtual ~Algorithm() = default;
    explicit Algorithm(Graph* graph);

    static int calculateRouteCost(const std::vector<NodeInfo>& route);
    static int calculateVisitedAndPassedNodes(const std::vector<NodeInfo>& route);
    static int calculateVisitedNodes(const std::vector<NodeInfo>& route);
};
