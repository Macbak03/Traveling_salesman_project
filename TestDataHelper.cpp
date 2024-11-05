#include "TestDataHelper.h"
#include <numeric>
#include <ctime>
#include <algorithm>
#include <random>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "Edge.h"

using nlohmann::json;

void TestDataHelper::generateData(int numberOfNodes, const std::string& fileName) {
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution dist_coord(0, 100);
    std::uniform_int_distribution dist_cost(1, 20);
    std::uniform_int_distribution dist_time(0, 50);

    // Generowanie węzłów
    for (int i = 1; i <= numberOfNodes; ++i) {
        const int x = dist_coord(rng);
        const int y = dist_coord(rng);
        const auto timeWindow = TimeWindow(dist_time(rng), dist_time(rng));
        auto* node = new Node(i, x, y, timeWindow);
        if (node->timeWindow.start > node->timeWindow.end) {
            std::swap(node->timeWindow.start, node->timeWindow.end);
        }
        nodes.push_back(node);
    }

    // Generowanie krawędzi
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            if (rng() % 10 < 7) {  // 70% szans na istnienie krawędzi
                const int cost = dist_cost(rng);
                auto* edge = new Edge(nodes[i], nodes[j], cost, false);
                edges.push_back(edge);
            }
        }
    }

    // Tworzenie obiektu JSON
    json j;

    for (const auto& node : nodes) {
        j[DataLabels::nodes].push_back({
            {DataLabels::id, node->id},
            {DataLabels::x, node->x},
            {DataLabels::y, node->y},
            {DataLabels::time_window, {node->timeWindow.start, node->timeWindow.end}}
        });
    }
    for (const auto& edge : edges) {
        j[DataLabels::edges].push_back({
            {DataLabels::start, edge->start->id},
            {DataLabels::end, edge->end->id},
            {DataLabels::cost, edge->cost},
        });
    }

    // Zapis do pliku JSON
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << fileName << std::endl;
        return;
    }
    file << j.dump(4);
    file.close();
    std::cout << "Generated test data file: " << fileName << std::endl;
}

void TestDataHelper::loadData(const std::string& fileName, Graph& graph) {
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku JSON: " << fileName << std::endl;
        return;
    }

    json data;
    file >> data;

    // Wczytanie węzłów
    for (const auto& nodeData : data[DataLabels::nodes]) {
        const int id = nodeData[DataLabels::id];
        const int x = nodeData[DataLabels::x];
        const int y = nodeData[DataLabels::y];
        const TimeWindow timeWindow =  {nodeData[DataLabels::time_window][0], nodeData[DataLabels::time_window][1]};
        auto* node = new Node(id, x, y, timeWindow);
        graph.addNode(node);
    }

    // Wczytanie krawędzi
    for (const auto& edgeData : data[DataLabels::edges]) {
        const int start = edgeData[DataLabels::start];
        const int end = edgeData[DataLabels::end];
        const int cost = edgeData[DataLabels::cost];

        // Szukamy węzłów na podstawie ich ID
        Node* startNode = nullptr;
        Node* endNode = nullptr;
        for (const auto node : graph.nodes) {
            if (node->id == start) startNode = node;
            if (node->id == end) endNode = node;
        }

        if (startNode && endNode) {
            auto* edge = new Edge(startNode, endNode, cost);
            graph.addEdge(edge);
        }
    }
}
