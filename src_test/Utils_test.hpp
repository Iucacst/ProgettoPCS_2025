#pragma once
#include <gtest/gtest.h>
#include "Utils.hpp"
#include "Eigen/Eigen"
#include <numeric>  

using namespace std;
using namespace Eigen;
using namespace GeodeticLibrary;

TEST(utils_test, sphere_projection) 
{
    double x = 1.0;
    double y = 2.0;
    double z = 3.0;
    unit_sphere_projection(x, y, z);

    double norm = sqrt(x*x + y*y + z*z);

    EXPECT_NEAR(norm, 1.0, 1e-9);
}

TEST (utils_test, tetrahedron_build) 
{
    GeodeticSolid solid = build_tetrahedron();

    EXPECT_EQ(solid.NumCell0D, 4);
    EXPECT_EQ(solid.Cell0DId.size(), 4);
    EXPECT_EQ(solid.Cell0DCoordinates.cols(), 4);
  
    EXPECT_EQ(solid.NumCell1D, 6);
    EXPECT_EQ(solid.Cell1DId.size(), 6);
    EXPECT_EQ(solid.Cell1DExtrema.cols(), 6);
   
    EXPECT_EQ(solid.NumCell2D, 4);
    EXPECT_EQ(solid.Cell2DId.size(), 4);
}

TEST (utils_test, icosahedron_build) 
{
    GeodeticSolid solid = build_icosahedron();
    
    EXPECT_EQ(solid.NumCell0D, 12);
    EXPECT_EQ(solid.Cell0DId.size(), 12);
    EXPECT_EQ(solid.Cell0DCoordinates.cols(), 12);
    
    EXPECT_EQ(solid.NumCell1D, 30);
    EXPECT_EQ(solid.Cell1DId.size(), 30);
    EXPECT_EQ(solid.Cell1DExtrema.cols(), 30);
    
    EXPECT_EQ(solid.NumCell2D, 20);
    EXPECT_EQ(solid.Cell2DId.size(), 20);
}

TEST (utils_test, octahedron_build) 
{
    GeodeticSolid solid = build_octahedron();

    EXPECT_EQ(solid.NumCell0D, 6);
    EXPECT_EQ(solid.Cell0DId.size(), 6);
    EXPECT_EQ(solid.Cell0DCoordinates.cols(), 6);

    EXPECT_EQ(solid.NumCell1D, 12);
    EXPECT_EQ(solid.Cell1DId.size(), 12);
    EXPECT_EQ(solid.Cell1DExtrema.cols(), 12);

    EXPECT_EQ(solid.NumCell2D, 8);
    EXPECT_EQ(solid.Cell2DId.size(), 8);
}

TEST(utils_test, find_distance) 
{
    GeodeticSolid solid = build_tetrahedron();
    unsigned int a = 0; // coordinate 1, 1, 1
    unsigned int b = 1; // coordinate -1, -1, 1

    double dist = find_distance(solid, a, b);

    EXPECT_NEAR(dist, sqrt(8.0/3.0), 1e-9);

    double zero_dist = find_distance(solid, a, a); // Test for identical points
    EXPECT_NEAR(zero_dist, 0.0, 1e-9);
}

TEST(utils_test, check_ordination_tetrahedron) {

    GeodeticSolid solid = build_tetrahedron();
    
    bool is_ordered = check_ordination(solid);
    
    EXPECT_TRUE(is_ordered);
}

TEST(utils_test, check_ordination_octahedron) {

    GeodeticSolid solid = build_octahedron();
    
    bool is_ordered = check_ordination(solid);
    
    EXPECT_TRUE(is_ordered);
}

TEST(utils_test, check_ordination_icosahedron) {

    GeodeticSolid solid = build_icosahedron();
    
    bool is_ordered = check_ordination(solid);
    
    EXPECT_TRUE(is_ordered);
}

TEST(utils_test, check_shortest_path_length)
{
    GeodeticSolid solid = build_octahedron();

    shortest_path(0, 1, solid);
    vector <double> valori = solid.vertices_short_path;
    double somma = accumulate(valori.begin(), valori.end(), 0.0);

    EXPECT_NEAR (somma, 3.0, 1e-9);

}

TEST(utils_test, check_shortest_path_edges)
{
    GeodeticSolid solid = build_octahedron();

    shortest_path(0, 1, solid);
    vector <double> valori = solid.edges_short_path;
    double somma = accumulate(valori.begin(), valori.end(), 0.0);

    EXPECT_NEAR (somma, 2.0, 1e-9);
}

TEST(utils_test, check_find_edge)
{
    GeodeticSolid solid = build_octahedron();

    unsigned int edge_id = find_edge(0, 2, solid);
    EXPECT_EQ(edge_id, 0); 

    unsigned int non_existent_edge = find_edge(0, 1, solid);
    EXPECT_EQ(non_existent_edge, -1); 
}
