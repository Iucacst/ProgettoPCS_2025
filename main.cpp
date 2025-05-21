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

    mesh = build_tetrahedron();
    // Print all Cell2DVertices
    for (unsigned int i = 0; i < mesh.NumCell2D; ++i)
    {
        cout << "Cell2DVertices[" << i << "]: ";
        for (unsigned int j = 0; j < mesh.Cell2DVertices[i].size(); ++j)
        {
            cout << mesh.Cell2DVertices[i][j] << " ";
        }
        cout << std::endl;
    }
    //triangulation_c1(3, 3, mesh);
    

    
    Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);
        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DCoordinates);
    }

    {
        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DCoordinates,
                                 mesh.Cell1DExtrema,
                                 {});
    }
	
	
    return 0;
}