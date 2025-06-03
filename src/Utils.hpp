#pragma once

#include <iostream>
#include "GeodeticSolid.hpp"

using namespace std;

namespace GeodeticLibrary
{
	void fill_Cell3D(GeodeticSolid& solid);
	void unit_sphere_projection(double& x, double& y, double& z);
	void GeodeticSolid_projection(GeodeticSolid& solid);
	void print_GeodeticSolid(GeodeticSolid& solid);
	unsigned int find_edge(unsigned int P, unsigned int Q, GeodeticSolid& solid);
	bool check_ordination(GeodeticSolid& solid);

	GeodeticSolid build_tetrahedron();
	GeodeticSolid build_octahedron();
	GeodeticSolid build_icosahedron();
	
	void triangulation_c1(const unsigned int b, unsigned int q, GeodeticSolid& solid);
	void triangulation_c2(const unsigned int b, unsigned int q, GeodeticSolid& solid);
	GeodeticSolid dualize(GeodeticSolid& solid);

	GeodeticSolid build_geodetic_polygon_c1(unsigned int p, unsigned int q, unsigned int b, unsigned int c);
	GeodeticSolid build_geodetic_polygon_c2(unsigned int p, unsigned int q, unsigned int b, unsigned int c);
	GeodeticSolid build_goldberg_polyhedron_c1(unsigned int p, unsigned int q, unsigned int b, unsigned int c);
	GeodeticSolid build_goldberg_polyhedron_c2(unsigned int p, unsigned int q, unsigned int b, unsigned int c);
}
