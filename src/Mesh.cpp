#include "Mesh.h"

#include "json/json.hpp"

#include <cmath>
#include <fstream>

Mesh::Mesh(int length, int height, float n, float T) {
    Init();

    length *= 2;

    for (int i = -length; i <= length; ++i)
        for (int j = -length; j <= length; ++j)
            for (int k = -height; k <= height; ++k)
                for (int l = 0; l < m_basicCell.size(); ++l) {
                    Point temp = m_basicCell[l];

                    temp.x += i / 3.0f;
                    temp.y += j / 3.0f;
                    temp.z += k / 3.0f;

                    if (std::abs(temp.x - temp.y) < length / 3.0f + m_eps &&
                        std::abs(temp.x) < length / 3.0f + m_eps &&
                        std::abs(temp.y) < length / 3.0f + m_eps &&
                        std::abs(temp.z) < height / 3.0f + m_eps)
                        points.push_back(new Point(temp.x, temp.y, temp.z, n, T));
                }

    Point aVec(m_a, 0.0f, 0.0f);
    Point bVec(m_b * cosf(m_gamma), m_b * sinf(m_gamma), 0.0f);
    Point cVec(m_c * cosf(m_beta), m_c * cosf(m_alpha) * sinf(m_gamma), m_c * sqrtf(sinf(m_beta) * sinf(m_beta) - sinf(m_gamma) * sinf(m_gamma) * cosf(m_alpha) * cosf(m_alpha)));

    for (auto& point : points) {
        auto old_x = point->x;
        auto old_y = point->y;
        auto old_z = point->z;

        point->x = aVec.x * old_x + bVec.x * old_y + cVec.x * old_z;
        point->y = aVec.y * old_x + bVec.y * old_y + cVec.y * old_z;
        point->z = aVec.z * old_x + bVec.z * old_y + cVec.z * old_z;
    }

    size = points.size();

    for (int p1 = 0; p1 < size; ++p1)
        for (int p2 = p1 + 1; p2 < size; ++p2)
            if (std::abs(points[p1]->Dist(points[p2]) - m_l) < m_eps) {
                neighbors[points[p1]].push_back(points[p2]);
                neighbors[points[p2]].push_back(points[p1]);
            }
}

void Mesh::Init()
{
    std::ifstream f(m_path);
    nlohmann::json grid = nlohmann::json::parse(f);

    m_a = grid["a"];
    m_b = grid["b"];
    m_c = grid["c"];

    m_alpha = grid["alpha"];
    m_beta = grid["beta"];
    m_gamma = grid["gamma"];

    m_alpha *= (float)M_PI / 180.0f;
    m_beta *= (float)M_PI / 180.0f;
    m_gamma *= (float)M_PI / 180.0f;

    m_l = grid["l"];
    m_eps = grid["eps"];

    m_basicCell = {
        Point(grid["O1"]["x"],  grid["O1"]["y"],  grid["O1"]["z"]),
        Point(grid["O2"]["x"],  grid["O2"]["y"],  grid["O2"]["z"]),
        Point(grid["O3"]["x"],  grid["O3"]["y"],  grid["O3"]["z"]),
        Point(grid["O4"]["x"],  grid["O4"]["y"],  grid["O4"]["z"]),
        Point(grid["O5"]["x"],  grid["O5"]["y"],  grid["O5"]["z"]),
        Point(grid["O6"]["x"],  grid["O6"]["y"],  grid["O6"]["z"]),
        Point(grid["O7"]["x"],  grid["O7"]["y"],  grid["O7"]["z"]),
        Point(grid["O8"]["x"],  grid["O8"]["y"],  grid["O8"]["z"]),
        Point(grid["O9"]["x"],  grid["O9"]["y"],  grid["O9"]["z"]),
        Point(grid["O10"]["x"], grid["O10"]["y"], grid["O10"]["z"]),
        Point(grid["O11"]["x"], grid["O11"]["y"], grid["O11"]["z"]),
        Point(grid["O12"]["x"], grid["O12"]["y"], grid["O12"]["z"])
    };
}
