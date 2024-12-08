#include "Graph.h"
#include "TestDataHelper.h"
#include "Greedy.h"
#include "BruteForce.h"
#include <iostream>

#include "SimulatedAnnealing.h"

enum Method {
    GREEDY,
    BRUTE_FORCE,
    SIMULATED_ANNEALING
};

std::string toString(const Method method) {
    switch (method) {
        case GREEDY: return "greedy";
        case BRUTE_FORCE: return "brute force";
        case SIMULATED_ANNEALING: return "simulated annealing";
        default: return "unknown algorithm";
    }
}

void printRoute(const std::vector<NodeInfo>& routeInfo, const Method method) {
    std::cout << "best cost: " << Algorithm::calculateRouteCost(routeInfo) << std::endl;
    std::cout << "Optimal tour " << toString(method) << ":" << std::endl;
    for (const auto& [node, visited, totalCost] : routeInfo) {
        std::cout << "Node: " << node->id
                  << ", Status: " << (visited ? "visited" : "passing through")
                  << "  ";
    }
    std::cout << std::endl;
    std::cout << "visited nodes: " << Algorithm::calculateVisitedNodes(routeInfo) << std::endl;
    std::cout << "visited and passed nodes: " << Algorithm::calculateVisitedAndPassedNodes(routeInfo) << std::endl;
    std::cout << std::endl;
}

int main() {
    constexpr int numberOfNodes = 10;
    const std::string fileName = "..\\data_" + std::to_string(numberOfNodes) + "_nodes.json"; //variables for data generation

    //TestDataHelper::generateData(numberOfNodes, fileName);
    Graph graph;
    TestDataHelper::loadData(fileName, graph);
    //graph.blockRandomEdges(0.3);

    const auto greedy = Greedy(&graph);
    const auto routeGreedy = greedy.findSolution(graph.nodes[0], false);
    printRoute(routeGreedy, GREEDY);

    auto bruteForce = BruteForce(&graph);
    const auto routeBruteForce = bruteForce.findSolution(graph.nodes[0]);
    printRoute(routeBruteForce, BRUTE_FORCE);

    auto simulatedAnnealing = SimulatedAnnealing(&graph);
    constexpr double initialTemp = 5000.0;
    constexpr double coolingRate = 0.99;
    constexpr int maxIterations = 10000;
    auto [routeSimulatedAnnealing, cost] = simulatedAnnealing.simulateAnnealing(graph.nodes[0], initialTemp, coolingRate, maxIterations);
    printRoute(routeSimulatedAnnealing, SIMULATED_ANNEALING);


    return 0;
}
