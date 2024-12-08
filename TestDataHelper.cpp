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
    std::uniform_int_distribution dist_time(0, 400);

    // Generating nodes
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

    // Generating edges
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            if (rng() % 10 < 7) {  // 70% possibility for edge existence
                auto* edge = new Edge(nodes[i], nodes[j]);
                edges.push_back(edge);
            }
        }
    }

    // Creating json object
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

    // Saving to JSON file
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
    // Open the JSON file
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Error when opening JSON file: " << fileName << std::endl;
        return;
    }

    json data;
    try {
        file >> data;  // Attempt to parse JSON data
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
        return;
    }

    // Safely load nodes
    if (data.contains(DataLabels::nodes) && data[DataLabels::nodes].is_array()) {
        for (const auto& nodeData : data[DataLabels::nodes]) {
            if (nodeData.contains(DataLabels::id) && nodeData.contains(DataLabels::x) &&
                nodeData.contains(DataLabels::y) && nodeData.contains(DataLabels::time_window) &&
                nodeData[DataLabels::time_window].is_array() &&
                nodeData[DataLabels::time_window].size() == 2) {

                const int id = nodeData[DataLabels::id].get<int>();
                const int x = nodeData[DataLabels::x].get<int>();
                const int y = nodeData[DataLabels::y].get<int>();
                const TimeWindow timeWindow = {
                        nodeData[DataLabels::time_window][0].get<int>(),
                        nodeData[DataLabels::time_window][1].get<int>()
                };

                auto* node = new Node(id, x, y, timeWindow);
                graph.addNode(node);
            } else {
                std::cerr << "Invalid node data found. Skipping..." << std::endl;
            }
        }
    } else {
        std::cerr << "No valid nodes array found in the JSON data." << std::endl;
    }

    // Safely load edges
    if (data.contains(DataLabels::edges) && data[DataLabels::edges].is_array()) {
        for (const auto& edgeData : data[DataLabels::edges]) {
            if (edgeData.contains(DataLabels::start) && edgeData.contains(DataLabels::end) &&
                edgeData.contains(DataLabels::cost)) {

                const int start = edgeData[DataLabels::start].get<int>();
                const int end = edgeData[DataLabels::end].get<int>();
                const int cost = edgeData[DataLabels::cost].get<int>();

                // Find start and end nodes in the graph
                Node* startNode = nullptr;
                Node* endNode = nullptr;
                for (const auto node : graph.nodes) {
                    if (node->id == start) startNode = node;
                    if (node->id == end) endNode = node;
                }

                // Create edge if both nodes are found
                if (startNode && endNode) {
                    auto* edge = new Edge(startNode, endNode, cost);
                    graph.addEdge(edge);
                } else {
                    std::cerr << "Edge refers to invalid node IDs: start=" << start
                              << ", end=" << end << ". Skipping..." << std::endl;
                }
            } else {
                std::cerr << "Invalid edge data found. Skipping..." << std::endl;
            }
        }
    } else {
        std::cerr << "No valid edges array found in the JSON data." << std::endl;
    }
}
