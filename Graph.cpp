#include "Graph.h"
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include "json.hpp"

using nlohmann::json;

void Graph::addNode(Node* node) {
    nodes.push_back(node);
}

void Graph::addEdge(Edge* edge) {
    edges.push_back(edge);
}

std::vector<Node*> Graph::getNeighbors(const Node* node) const {
    std::vector<Node*> neighbors;
    for (const auto edge : edges) {
        if (edge->start == node && !edge->blocked) {
            neighbors.push_back(edge->end);
        }
    }
    return neighbors;
}

void Graph::blockRandomEdges(const double percentage) const {
    const int numEdgesToBlock = static_cast<int>(static_cast<double>(edges.size()) * percentage);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::vector<int> indices(edges.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::ranges::shuffle(indices, std::mt19937(std::random_device()()));

    for (int i = 0; i < numEdgesToBlock; ++i) {
        edges[indices[i]]->block();
    }
}
