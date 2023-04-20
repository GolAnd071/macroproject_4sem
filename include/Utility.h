#include <map>
#include <string>
#include <vector>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkTetra.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>

#include "Mesh.h"
#include "Point.h"

namespace Utility {

    std::vector<Point*> ExtractKeys(std::map<Point*, std::vector<Point*>> const& input_map) {
        std::vector<Point*> retval;
        for (auto const& element : input_map)
            retval.push_back(element.first);
        return retval;
    }

    void Snapshot(int snap_number, Mesh* mesh)
    {
        std::vector<Point*> points = ExtractKeys(mesh->neighbors);
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
            is_freezed->InsertNextValue(double(points[i]->IsFreezed()));
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

}
