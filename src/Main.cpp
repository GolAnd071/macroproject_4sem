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
    Mesh* mesh = new Mesh(5, 5, Params::n(), Params::T());
    mesh->points.at(mesh->size/2)->Seed();
    Utility::Snapshot(0, mesh);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 1);
    auto random = std::bind(dist, gen);
    
    int iter_num = 0;
    if (argc > 1)
        iter_num = atoi(argv[1]);

    if (iter_num == 0)
    {
        std::cout << "No iters for simultion, only mesh was snapshoted.";
        return 0;
    }
    std::cout << "Simulation started.\n";

    for (int iter = 1; iter <= iter_num; ++iter) {

        // mesh traversal to freeze and melt

        std::queue<Point*> hasLoan;
        std::vector<Point*> wantFreeze, wantMelt;

        for (Point* point : mesh->points) {
            float prob = random();

            if (point->IsFreezed()) {
                int countVapor = 0;

                for (auto neighbor : mesh->neighbors[point])
                    if (!neighbor->IsFreezed())
                        ++countVapor;

                if (countVapor > 0 && prob < Params::MeltP(point))
                    wantMelt.push_back(point);

            } else {
                int countIce = 0;

                for (auto neighbor : mesh->neighbors[point])
                    if (neighbor->IsFreezed())
                        ++countIce;

                if (prob < Params::FreezeP(point) * countIce)
                    wantFreeze.push_back(point);
            }
        }

        for (auto point : wantFreeze) {
            point->Freeze();
            point->n -= Params::IceN();
            hasLoan.push(point);
        }

        for (auto point : wantMelt) {
            point->Melt();
            point->n += Params::IceN();
        }



        // mesh traversal to fulfill concentration loan after freezing

        std::unordered_set<Point*> fulfilledLoan;

        while (hasLoan.size() != 0) {
            Point* point = hasLoan.front();
            hasLoan.pop();
            fulfilledLoan.insert(point);

            if (point->n < 0) {
                size_t num = mesh->neighbors[point].size();

                for (Point* neighbor : mesh->neighbors[point]) {
                    if (fulfilledLoan.count(neighbor))
                        continue;
                    neighbor->n -= point->n / num;
                    hasLoan.push(neighbor);
                }

                point->n = 0;
            }
        }


        // mesh traversal to calculate concentration and temperature flow for each edge

        std::map<Point*, float> deltaN, deltaH;

        for (Point* point : mesh->points) {
            float dN = 0, dH = 0;

            for (Point* neighbor : mesh->neighbors[point]) {
                dN += Params::NFlow(neighbor, point);
                dH += Params::HFlow(neighbor, point);
            }

            deltaN[point] = dN;
            deltaH[point] = dH;
        }
        

        // mesh traversal to update concentration and temperature

        for (Point* point : mesh->points) {
            Params::Diffuse(point, deltaN[point]);
            Params::Heat(point, deltaH[point]);
        }

        Utility::Snapshot(iter, mesh);
        
        std::cout << "Iteration " << iter << " finished.\n";
    }

    std::cout << "Simulation finished.\n";
    return 0;
}
