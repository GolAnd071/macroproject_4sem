#include <iostream>
#include <vector>
#include <map>

struct Point
{
    float x, y, z; // coordinates

    float n, T; // physical params

    bool is_freezed = false;

    Point(float x, float y, float z, float n, float T) : x(x), y(y), z(z), n(n), T(T) {}
};

struct Mesh
{
    std::map<Point*, std::vector<Point*>> neighbors;
};

void createMesh(int xSize, int ySize, int zSize)
{
    std::vector<Point*> points;

    const float l = 1.0f;
    const float n = 1.0f;
    const float T = 300.0f;

    float x = 0.0f;

    for (int i = 0; i < xSize; ++i)
        points.push_back(new Point(x + 3 * l * i, 0.0f, 0.0f, n, T));
}

int main()
{
    return 0;
}
