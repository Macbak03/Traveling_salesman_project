#include "Graph.h"
#include "TestDataHelper.h"

int main() {
    constexpr int numberOfNodes = 10;
    const std::string fileName = "..\\data_" + std::to_string(numberOfNodes) + "_nodes.json"; //variables for data generation

    TestDataHelper::generateData(numberOfNodes, fileName);
    Graph graph;
    TestDataHelper::loadData(fileName, graph);
    graph.blockRandomEdges(0.3);

    return 0;
}
