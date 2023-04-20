#pragma once

#include "Point.h"

#include <map>
#include <vector>

class Mesh
{
public:
    Mesh(int xSize, int ySize, int zSize, float n, float T);

public:
    size_t size;
    std::vector<Point*> points;
    std::map<Point*, std::vector<Point*>> neighbors;
};
