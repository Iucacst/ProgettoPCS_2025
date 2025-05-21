#pragma once

#include <iostream>
#include "GeodeticSolid.hpp"

using namespace std;

namespace GeodeticLibrary
{
	void fill_Cell3D(GeodeticSolid& solid);
	void unit_sphere_projection(double& x, double& y, double& z);
	void print_GeodeticSolid(GeodeticSolid& solid);

	GeodeticSolid build_tetrahedron();
	GeodeticSolid build_octahedron();
	GeodeticSolid build_icosahedron();
	
	void triangulation_c1(const unsigned int b, unsigned int q, GeodeticSolid& solid);
	void build_polygon_c1(unsigned int p, unsigned int q, unsigned int b, unsigned int c);
}
