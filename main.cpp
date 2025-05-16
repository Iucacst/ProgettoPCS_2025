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

    for (int x : mesh.Cell0DId) {
        std::cout << x << " " << endl;
    }

    for (int x : mesh.Cell1DId) {
        std::cout << x << " " << endl;
    }

    cout << mesh.Cell1DExtrema << endl;

    cout << mesh.Cell0DCoordinates << endl;

    Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0D, 0.0);
        for(const auto &m : mesh.MarkerCell0Ds)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DCoordinates,
                               cell0Ds_properties);
    }

    {
        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DCoordinates,
                                 mesh.Cell1DExtrema,
                                 {});
    }
    return 0;
}