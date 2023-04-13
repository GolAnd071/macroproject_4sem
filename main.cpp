#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

struct Point
{
    float x, y, z; // coordinates

    float n, T; // physical params

    bool is_freezed = false;

    Point(float x, float y, float z, float n, float T) : x(x), y(y), z(z), n(n), T(T) {}
};

float dist(Point* p1, Point* p2)
{
    float dx = p1->x - p2->x;
    float dy = p1->y - p2->y;
    float dz = p1->z - p2->z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

struct Mesh
{
    std::map<Point*, std::vector<Point*>> neighbors;

    Mesh(int xSize, int ySize, int zSize)
    {
        std::vector<Point*> points;

        const float l = 1.0f;
        const float n = 1.0f;
        const float T = 300.0f;
        const float eps = 1e-10;

        for (int zNum = 0; zNum < zSize; ++zNum)
            for (int yNum = 0; yNum < ySize; ++yNum)
                for (int xNum = 0; xNum < xSize; ++xNum) {
                    points.push_back(new Point(3 * l * xNum + l * (yNum % 2 + zNum % 2) / 2,     std::sqrt(3) * l * (yNum + zNum % 2) / 2, l * zNum, n, T));
                    points.push_back(new Point(3 * l * xNum + l * (yNum % 2 + zNum % 2) / 2 + l, std::sqrt(3) * l * (yNum + zNum % 2) / 2, l * zNum, n, T));
                }

        for (int p1 = 0; p1 < points.size(); ++p1)
            for (int p2 = p1 + 1; p2 < points.size(); ++p2)
                if (std::abs(dist(points[p1], points[p2]) - l) < eps) {
                    neighbors[points[p1]].push_back(points[p2]);
                    neighbors[points[p2]].push_back(points[p1]);
                }
    }
};

int main()
{
    Mesh* mesh = new Mesh(10, 10, 10);

    return 0;
}
