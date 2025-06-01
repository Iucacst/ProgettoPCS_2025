#include <iostream>
#include <vector>
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace GeodeticLibrary;

int main()
{
    GeodeticSolid solid = build_polygon_c1(3, 5, 0, 1);

    triangulation_c2(5, 5, solid);

    Gedim::UCDUtilities utilities;
    {
        utilities.ExportPoints("./Cell0D.inp", solid.Cell0DCoordinates);
        utilities.ExportSegments("./Cell1D.inp", solid.Cell0DCoordinates, solid.Cell1DExtrema);
    }
	
	
    return 0;
}