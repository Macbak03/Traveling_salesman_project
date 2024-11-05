#pragma once

#include "TimeWindow.h"

class Node {
public:
    int id;
    int x, y;
    TimeWindow timeWindow;

    Node(int id, int x, int y, TimeWindow timeWindow);
};