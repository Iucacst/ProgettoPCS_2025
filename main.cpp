#include <iostream>
#include <vector>
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace GeodeticLibrary;

int main()
{

    GeodeticSolid solid = build_goldberg_polyhedron_c2(5, 3, 2, 2);


    //triangulation_c1(1, 4, solid); // b, q, solid

    Gedim::UCDUtilities utilities;
    {
        utilities.ExportPoints("./Cell0D.inp", solid.Cell0DCoordinates);
        utilities.ExportSegments("./Cell1D.inp", solid.Cell0DCoordinates, solid.Cell1DExtrema);
    }
	
	
    return 0;
}