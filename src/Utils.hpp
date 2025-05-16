#pragma once

#include <iostream>
#include "GeodeticSolid.hpp"

using namespace std;

namespace GeodeticLibrary
{
	void build_polygon_class_1(unsigned int p, unsigned int q, unsigned int b, unsigned int c);
	GeodeticSolid build_tetrahedron();

	/*bool ImportMesh(PolygonalMesh& mesh);
	bool ImportCell0Ds(PolygonalMesh& mesh);
	bool ImportCell1Ds(PolygonalMesh& mesh);
	bool ImportCell2Ds(PolygonalMesh& mesh);
	bool edges_test(PolygonalMesh& mesh);
	bool areas_test(PolygonalMesh& mesh);*/
}
