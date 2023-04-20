#include "Mesh.h"

#include <cmath>

Mesh::Mesh(int xSize, int ySize, int zSize, float n, float T)
{
    const float l = 1.0f;
    const float eps = 1e-10;

    for (int zNum = 0; zNum < 3 * zSize; ++zNum)
        for (int yNum = 0; yNum < 3 * ySize; ++yNum)
            for (int xNum = 0; xNum < xSize; ++xNum) {
                points.push_back(new Point( 3 * l * xNum + l * (yNum % 2 + zNum % 2) / 2                        - 3 * xSize / 2,
                                            std::sqrt(3) * l * (yNum + zNum % 2) / 2                            - 3 * ySize / 2,
                                            l * zNum                                                            - 3 * zSize / 2
                                            , n, T));
                points.push_back(new Point( 3 * l * xNum + l * (yNum % 2 + zNum % 2) / 2 + (2 - yNum % 2) * l   - 3 * xSize / 2,
                                            std::sqrt(3) * l * (yNum + zNum % 2) / 2                            - 3 * ySize / 2,
                                            l * zNum                                                            - 3 * zSize / 2
                                            , n, T));
            }

    size = points.size();

    for (int p1 = 0; p1 < points.size(); ++p1)
        for (int p2 = p1 + 1; p2 < points.size(); ++p2)
            if (std::abs(points[p1]->Dist(points[p2]) - l) < eps) {
                neighbors[points[p1]].push_back(points[p2]);
                neighbors[points[p2]].push_back(points[p1]);
            }
}
