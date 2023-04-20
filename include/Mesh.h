#pragma once

#include "Point.h"

#include <map>
#include <vector>

class Mesh
{
public:
    Mesh(int xSize, int ySize, int zSize);

public:
    std::map<Point*, std::vector<Point*>> neighbors;
};
