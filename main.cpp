#include <iostream>
#include <vector>
#include <map>

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

void snapshot(int snap_number, std::vector<Point> points)
{
    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();
    auto n = vtkSmartPointer<vtkDoubleArray>::New();
    n->SetName("concentration");
    auto T = vtkSmartPointer<vtkDoubleArray>::New();
    T->SetName("temperature");
    auto is_freezed = vtkSmartPointer<vtkDoubleArray>::New();
    is_freezed->SetName("state");
    for(unsigned int i = 0; i < points.size(); i++) {
        dumpPoints->InsertNextPoint(points[i].x, points[i].y, points[i].z);
        n->InsertNextValue(points[i].n);
        T->InsertNextValue(points[i].T);
        is_freezed->InsertNextValue(double(points[i].is_freezed));
    }
    unstructuredGrid->SetPoints(dumpPoints);
    unstructuredGrid->GetPointData()->AddArray(n);
    unstructuredGrid->GetPointData()->AddArray(T);

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

int main()
{
    return 0;
}