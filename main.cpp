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

    mesh = build_icosahedron();

    triangulation_c1(5, 5, mesh);
    
    print_GeodeticSolid(mesh);
    
    Gedim::UCDUtilities utilities;
    {
        utilities.ExportPoints("./Cell0D.inp", mesh.Cell0DCoordinates);
        utilities.ExportSegments("./Cell1D.inp", mesh.Cell0DCoordinates, mesh.Cell1DExtrema);
    }
	
	
    return 0;
}