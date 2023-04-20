#include "Mesh.h"
#include "Params.h"
#include "Point.h"
#include "Utility.h"

#include <algorithm>
#include <random>
#include <functional>
#include <queue>
#include <unordered_set>

int main(int argc, char** argv)
{
    Mesh* mesh = new Mesh(5, 5, 5, Params::n(), Params::T());

    size_t numPoints = mesh->neighbors.size();

    auto arrPoints = Utility::ExtractKeys(mesh->neighbors);

    Utility::ExtractKeys(mesh->neighbors).at(numPoints / 2)->Seed();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 1);
    auto random = std::bind(dist, gen);
    
    int iter_num = 0;
    if (argc > 1)
        iter_num = atoi(argv[1]);

    Utility::Snapshot(0, arrPoints);

    for (int iter = 1; iter <= iter_num; ++iter) {

        std::cout << "Iteration " << iter << " started.\n";

        std::queue<Point*> hasLoan;

        // mesh traversal to freeze and melt
        for (Point* point : arrPoints) {
            float prob = random();

            if (point->IsFreezed()) {
                int countVapor = 0;

                for (auto neighbor : mesh->neighbors[point])
                    if (!neighbor->IsFreezed())
                        ++countVapor;

                if (countVapor > 0 && prob > Params::MeltP(point))
                    point->Melt();

            } else {
                int countIce = 0;

                for (auto neighbor : mesh->neighbors[point])
                    if (neighbor->IsFreezed())
                        ++countIce;

                if (countIce * prob > Params::FreezeP(point)) {
                    point->Freeze();
                    point->n -= Params::IceN();
                    hasLoan.push(point);
                }
            }
        }

        std::unordered_set<Point*> visited;

        // mesh traversal to fulfill concentration loan after freezing
        while (hasLoan.size() != 0) {
            Point* point = hasLoan.front();
            hasLoan.pop();
            visited.insert(point);

            if (point->n < 0) {
                int num = mesh->neighbors[point].size();

                for (Point* neighbor : mesh->neighbors[point]) {
                    if (visited.count(neighbor))
                        continue;
                    neighbor->n -= point->n / num;
                    hasLoan.push(neighbor);
                }

                point->n = 0;
            }
        }

        std::map<Point*, float> deltaN, deltaH;

        // mesh traversal to calculate concentration and temperature flow for each edge
        for (Point* point : arrPoints) {
            float dN = 0, dH = 0;

            for (Point* neighbor : mesh->neighbors[point]) {
                dN += Params::NFlow(neighbor, point);
                dH += Params::HFlow(neighbor, point);
            }

            deltaN[point] = dN;
            deltaH[point] = dH;
        }
        
        // mesh traversal to update concentration and temperature
        for (Point* point : arrPoints) {
            Params::Diffuse(point, deltaN[point]);
            Params::Heat(point, deltaH[point]);
        }

        Utility::Snapshot(iter, arrPoints);

        std::cout << "Iteration " << iter << " ended.\n";
    }

    return 0;
}
