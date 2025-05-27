#include <iostream>
#include <vector>
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace GeodeticLibrary;

int main()
{
    GeodeticSolid solid = build_polygon_c1(3, 3, 0, 12);
    
    check_ordination(solid);

    Gedim::UCDUtilities utilities;
    {
        utilities.ExportPoints("./Cell0D.inp", solid.Cell0DCoordinates);
        utilities.ExportSegments("./Cell1D.inp", solid.Cell0DCoordinates, solid.Cell1DExtrema);
    }
	
	
    return 0;
}