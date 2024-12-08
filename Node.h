#pragma once

#include "TimeWindow.h"

class Node {
public:
    int id;
    int x, y;
    TimeWindow timeWindow;
    bool visited = false;

    Node(int id, int x, int y, TimeWindow timeWindow);
};