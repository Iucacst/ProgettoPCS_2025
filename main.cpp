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
    triangulation_c1(3, 3, mesh);
    
    Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);
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