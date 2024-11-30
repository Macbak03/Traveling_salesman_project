#include "Graph.h"
#include "TestDataHelper.h"
#include "Greedy.h"
#include <iostream>

int main() {
    constexpr int numberOfNodes = 10;
    const std::string fileName = "..\\data_" + std::to_string(numberOfNodes) + "_nodes.json"; //variables for data generation

    //TestDataHelper::generateData(numberOfNodes, fileName);
    Graph graph;
    TestDataHelper::loadData(fileName, graph);
    graph.blockRandomEdges(0.3);

    const auto greedy = Greedy(&graph);
    const auto tour = greedy.findSolution(graph.nodes[0], false);

    std::cout << "Optimal tour:" << std::endl;
    for (const auto& node : tour) {
        std::cout << "Node ID: " << node->id << " ";
    }
    std::cout << std::endl;


    return 0;
}
