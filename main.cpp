#include <iostream>
#include <vector>
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace GeodeticLibrary;

int main()
{
    GeodeticSolid solid = build_geodetic_polygon_c1(3, 4, 0, 4);

    shortest_path(0, 7, solid);

    build_UCD(solid);
	
	
    return 0;
}