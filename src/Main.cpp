#include "Mesh.h"
#include "Point.h"
#include "Utility.h"

#include <algorithm>

int main(int argc, char** argv)
{
    Mesh* mesh = new Mesh(5, 5, 5);
    Utility::ExtractKeys(mesh->neighbors).at(mesh->neighbors.size() / 2)->Seed();
    
    int iter_num = 0;
    if (argc > 1)
        iter_num = atoi(argv[1]);

    Utility::Snapshot(0, mesh);

    for (int iter = 1; iter <= iter_num; ++iter) {

        // mesh traversal to freeze and melt
        // mesh traversal to calculate concentration gradient
        // mesh traversal to update concentration

        Utility::Snapshot(iter, mesh);
    }

    return 0;
}
