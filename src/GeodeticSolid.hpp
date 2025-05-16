#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace GeodeticLibrary {
	
	struct GeodeticSolid {
		unsigned int NumCell0D = 0; 
		vector<unsigned int> Cell0DId = {}; 
		MatrixXd Cell0DCoordinates = {}; 
		map<unsigned int, list <unsigned int>> MarkerCell0Ds = {};
		
		unsigned int NumCell1D = 0; 
		vector<unsigned int> Cell1DId = {}; 
		MatrixXi Cell1DExtrema = {};

		unsigned int NumCell2D = 0; 
		vector<unsigned int> Cell2DId = {}; 
		vector<unsigned int> Cell2DNumVertices = {};
		vector<unsigned int> Cell2DNumEdges = {};
		vector<vector<unsigned int>> Cell2DVertices = {}; 
		vector<vector<unsigned int>> Cell2DEdges = {}; 
		
		unsigned int NumCell3D = 1;
		unsigned int Cell3DId = 0;
		unsigned int Cell3DNumVertices = {};
		unsigned int Cell3DNumEdges = {};
		unsigned int Cell3DNumFaces = {};
		vector<unsigned int> Cell3DVertices = {}; 
		vector<unsigned int> Cell3DEdges = {};
		vector<unsigned int> Cell3DFaces = {};
	};
}


