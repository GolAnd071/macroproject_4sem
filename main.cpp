#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkTetra.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <string>


struct Point
{
    float x, y, z; // coordinates

    float n, T; // physical params

private:
    // state params
    // "meltable" control what is represented: water particle or heterogeneous crystallization center (seed)
    // to prevent seed from being melt "meltable" and "freezed" are incapsulated with methods below
    bool freezed = false, meltable = false;

public:
    Point(float x, float y, float z, float n, float T) : x(x), y(y), z(z), n(n), T(T) {}

    bool is_freezed()
    {
        return freezed;
    }

    void freeze()
    {
        freezed = true;
    }

    void melt()
    {
        if (!meltable)
            freezed = false;
    }

    void seed()
    {
        meltable = true;
        freezed = true;
    }
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

        for (int zNum = 0; zNum < 3 * zSize; ++zNum)
            for (int yNum = 0; yNum < 3 * ySize; ++yNum)
                for (int xNum = 0; xNum < xSize; ++xNum) {
                    points.push_back(new Point( 3 * l * xNum + l * (yNum % 2 + zNum % 2) / 2                        -3*xSize/2,         
                                                std::sqrt(3) * l * (yNum + zNum % 2) / 2                            -3*ySize/2, 
                                                l * zNum                                                            -3*zSize/2
                                                , n, T));
                    points.push_back(new Point( 3 * l * xNum + l * (yNum % 2 + zNum % 2) / 2 + (2 - yNum % 2) * l   -3*xSize/2,
                                                std::sqrt(3) * l * (yNum + zNum % 2) / 2                            -3*ySize/2, 
                                                l * zNum                                                            -3*zSize/2
                                                , n, T));
                }

        for (int p1 = 0; p1 < points.size(); ++p1)
            for (int p2 = p1 + 1; p2 < points.size(); ++p2)
                if (std::abs(dist(points[p1], points[p2]) - l) < eps) {
                    neighbors[points[p1]].push_back(points[p2]);
                    neighbors[points[p2]].push_back(points[p1]);
                }
    }
};

std::vector<Point*> extract_keys(std::map<Point*, std::vector<Point*>> const& input_map) {
    std::vector<Point*> retval;
    for (auto const& element : input_map)
        retval.push_back(element.first);   
    return retval;
}

void snapshot(int snap_number, Mesh* mesh)
{
    std::vector<Point*> points = extract_keys(mesh->neighbors);
    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();
    auto n = vtkSmartPointer<vtkDoubleArray>::New();
    n->SetName("concentration");
    auto T = vtkSmartPointer<vtkDoubleArray>::New();
    T->SetName("temperature");
    auto is_freezed = vtkSmartPointer<vtkDoubleArray>::New();
    is_freezed->SetName("state");
    for(unsigned int i = 0; i < points.size(); i++) {
        dumpPoints->InsertNextPoint(points[i]->x, points[i]->y, points[i]->z);
        n->InsertNextValue(points[i]->n);
        T->InsertNextValue(points[i]->T);
        is_freezed->InsertNextValue(double(points[i]->is_freezed()));
    }
    unstructuredGrid->SetPoints(dumpPoints);
    unstructuredGrid->GetPointData()->AddArray(n);
    unstructuredGrid->GetPointData()->AddArray(T);
    unstructuredGrid->GetPointData()->AddArray(is_freezed);

    // А теперь пишем, как наши точки объединены в тетраэдры
    // for(unsigned int i = 0; i < elements.size(); i++) {
    //     auto tetra = vtkSmartPointer<vtkTetra>::New();
    //     tetra->GetPointIds()->SetId( 0, elements[i].nodesIds[0] );
    //     tetra->GetPointIds()->SetId( 1, elements[i].nodesIds[1] );
    //     tetra->GetPointIds()->SetId( 2, elements[i].nodesIds[2] );
    //     tetra->GetPointIds()->SetId( 3, elements[i].nodesIds[3] );
    //     unstructuredGrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
    // }

    std::string fileName = "crystal-step-" + std::to_string(snap_number) + ".vtu";
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(unstructuredGrid);
    writer->Write();
}


int main(int argc, char** argv)
{
    Mesh* mesh = new Mesh(5, 5, 1);
    extract_keys(mesh->neighbors).at(mesh->neighbors.size()/2)->seed();
    
    int iterations = 0;
    // int iterations = 1000;
    if (argc > 1)
        iterations = atoi(argv[1]);

    snapshot(0, mesh);
    for (int i = 1; i <= iterations; i++)
    {
        // mesh traversal to freeze and melt
        // mesh traversal to calculate concentration gradient
        // mesh traversal to update concentration
        snapshot(i, mesh);
    }

    return 0;
}