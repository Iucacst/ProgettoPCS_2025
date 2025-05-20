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

    mesh = build_octahedron();

    for (int x : mesh.Cell1DId) {
        std::cout << x << " " ;
    }

    cout << endl;

    for (int x : mesh.Cell2DId) {
        std::cout << x << " " ;
    }

    cout << mesh.Cell1DExtrema.cols() << endl;
    cout << mesh.NumCell1D << endl;

    for (int i = 0; i < mesh.NumCell1D; ++i) {
        cout << "Cell1DExtrema[" << i << "] : ";
        for (int j = 0; j < 2; ++j) {
            cout << mesh.Cell1DExtrema(j, i) << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < mesh.NumCell2D; ++i) {
        for (int j = 0; j < mesh.Cell2DEdges[i].size(); ++j) {
            cout << mesh.Cell2DEdges[i][j] << " ";
        }
        cout << endl;
    }

    

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