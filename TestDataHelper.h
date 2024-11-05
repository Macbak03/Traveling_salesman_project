#pragma once

#include <string>
#include "Graph.h"

namespace DataLabels {
    constexpr auto nodes = "nodes";
    constexpr auto id = "id";
    constexpr auto x = "x";
    constexpr auto y = "y";
    constexpr auto time_window = "time_window";

    constexpr auto edges = "edges";
    constexpr auto start = "start";
    constexpr auto end = "end";
    constexpr auto cost = "cost";
};

class TestDataHelper {
public:
    static void generateData(int numberOfNodes = 0, const std::string& fileName = "..\\file.json");
    static void loadData(const std::string& fileName, Graph& graph);
};

