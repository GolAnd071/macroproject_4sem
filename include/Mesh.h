#pragma once

#include "Point.h"

#include <map>
#include <vector>

class Mesh
{
public:
    Mesh(int nx, int ny, int nz, float n, float T);

public:
    std::map<Point*, std::vector<Point*>> neighbors;

private:
    void Init();

private:
    const char* m_path = "../config/grid.json";

    float m_a;
    float m_b;
    float m_c;

    float m_alpha;
    float m_beta;
    float m_gamma;

    float m_l;
    float m_eps;

    std::vector<Point> m_basicCell;
};
