#include <iostream>
#include <vector>
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace GeodeticLibrary;

int main()
{
    GeodeticSolid mesh;

    mesh = build_tetrahedron();

    Gedim::UCDUtilities utilities;
    {
        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DCoordinates);
    }

    {
        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DCoordinates,
                                 mesh.Cell1DExtrema,
                                 {});
    }
    return 0;
}