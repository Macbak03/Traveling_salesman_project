#pragma once

#include "Edge.h"
#include <vector>

class Graph {
public:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;

    void addNode(Node* node);
    void addEdge(Edge* edge);
    void blockRandomEdges(double percentage) const;
    std::vector<Node*> getNeighbors(const Node* node) const;
    Edge* findEdge(const Node* startNode, const Node* endNode) const;
    void unvisitNodes();
};
