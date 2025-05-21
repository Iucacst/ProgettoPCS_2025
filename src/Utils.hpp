#pragma once

#include <iostream>
#include "GeodeticSolid.hpp"

using namespace std;

namespace GeodeticLibrary
{
	GeodeticSolid build_tetrahedron();
	GeodeticSolid build_octahedron();
	GeodeticSolid build_icosahedron();
	
	void unit_sphere_projection(double& x, double& y, double& z);
	void triangulation_c1(const unsigned int b, unsigned int q, GeodeticSolid& solid);
	void build_polygon_class_1(unsigned int p, unsigned int q, unsigned int b, unsigned int c);

	/*bool ImportMesh(PolygonalMesh& mesh);
	bool ImportCell0Ds(PolygonalMesh& mesh);
	bool ImportCell1Ds(PolygonalMesh& mesh);
	bool ImportCell2Ds(PolygonalMesh& mesh);
	bool edges_test(PolygonalMesh& mesh);
	bool areas_test(PolygonalMesh& mesh);*/
}
