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

    void Snapshot(int snap_number, Mesh* mesh);

}
