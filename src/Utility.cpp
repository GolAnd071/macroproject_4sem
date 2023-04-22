#include "Utility.h"

namespace Utility {

    void Snapshot(int snap_number, Mesh* mesh)
    {
        vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();
        
        auto n = vtkSmartPointer<vtkDoubleArray>::New();
        n->SetName("concentration");
        auto T = vtkSmartPointer<vtkDoubleArray>::New();
        T->SetName("temperature");
        auto is_freezed = vtkSmartPointer<vtkDoubleArray>::New();
        is_freezed->SetName("state");

        for (Point* point : mesh->points) {
            dumpPoints->InsertNextPoint(point->x, point->y, point->z);
            n->InsertNextValue(point->n);
            T->InsertNextValue(point->T);
            is_freezed->InsertNextValue(double(point->IsFreezed()));
        }
        unstructuredGrid->SetPoints(dumpPoints);
        unstructuredGrid->GetPointData()->AddArray(n);
        unstructuredGrid->GetPointData()->AddArray(T);
        unstructuredGrid->GetPointData()->AddArray(is_freezed);

        std::filesystem::create_directories("result");
        std::string fileName = "result/crystal-step-" + std::to_string(snap_number) + ".vtu";
        vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(unstructuredGrid);
        writer->Write();
    }

}