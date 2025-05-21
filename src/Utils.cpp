#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
using std::cerr;
using std::vector;

namespace GeodeticLibrary
{

void build_polygon_class_1(unsigned int p, unsigned int q, unsigned int b, unsigned int c)
{
    GeodeticSolid solid;

    if((b == 0 && c == 0 || b != 0 && c != 0) && p != 3)
    {
        cerr << "Polyhedron does not belong to class 1." << endl;
    }

    if (q > 5 || q < 3)
    {
        cerr << "Polyhedron does not belong to class 1." << endl;
    }

    if (b == 0)
    {
        b = c;
        c = 0;
    }
    
    if (q == 3) 
    {
        solid = build_tetrahedron();
    }
    else{}   
}

GeodeticSolid build_tetrahedron()
{
    
    GeodeticSolid solid;
    
    solid.NumCell0D = 4;
    solid.NumCell1D = 6;
    solid.NumCell2D = 4;

    solid.Cell0DId.reserve(solid.NumCell0D);
    solid.Cell1DId.reserve(solid.NumCell1D);
    solid.Cell2DId.reserve(solid.NumCell2D);

    solid.Cell0DCoordinates = Eigen::MatrixXd::Zero(3, solid.NumCell0D);
    solid.Cell1DExtrema = Eigen::MatrixXi::Zero(2, solid.NumCell1D);

    solid.Cell2DVertices.reserve(solid.NumCell2D);
    solid.Cell2DEdges.reserve(solid.NumCell2D);

    solid.Cell2DVertices.reserve(solid.NumCell2D);
    solid.Cell2DEdges.reserve(solid.NumCell2D);

    Eigen::Matrix<double, 4, 3> vertices;
    vertices <<  1.0,  1.0,  1.0,
                -1.0, -1.0,  1.0,
                -1.0,  1.0, -1.0,
                 1.0, -1.0, -1.0;

    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        solid.Cell0DId.push_back(i);

        for (int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(j, i) = vertices(i, j)/sqrt(3);
        }
    }

    int ctr = 0;
    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        solid.Cell1DId.push_back(i);
        //solid.Cell1DExtrema(i, 0) = solid.Cell1DId[i];
        
        for (int j = i+1; j <= 3; ++j)
        {
            solid.Cell1DExtrema(0, ctr) = solid.Cell0DId[i];
            solid.Cell1DExtrema(1, ctr) = solid.Cell0DId[j];
            ctr++;
        }
    }
    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        int v = 0;

        solid.Cell2DId.push_back(i);
        solid.Cell2DNumVertices.push_back(3);
        solid.Cell2DNumEdges.push_back(3);
        
        solid.Cell2DEdges[i].push_back(i);
        solid.Cell2DVertices[i].push_back(i);

        for (int j = 0; j < 3; ++j)
        {
            v = (j + i + 2) % 4;
            solid.Cell2DVertices[i].push_back(v);
        }
    }

    solid.Cell2DEdges[0].push_back(5); // Faccia 2,3,0
    solid.Cell2DEdges[0].push_back(2);
    solid.Cell2DEdges[0].push_back(1);

    solid.Cell2DEdges[1].push_back(2);  // Faccia 3,0,1
    solid.Cell2DEdges[1].push_back(0);
    solid.Cell2DEdges[1].push_back(4);

    solid.Cell2DEdges[2].push_back(0);  // Faccia 0,1,2
    solid.Cell2DEdges[2].push_back(3);
    solid.Cell2DEdges[2].push_back(1);

    solid.Cell2DEdges[3].push_back(3);  // Faccia 1,2,3
    solid.Cell2DEdges[3].push_back(5);
    solid.Cell2DEdges[3].push_back(4);

    //Solido 3D

    solid.Cell3DNumVertices = 4;
    solid.Cell3DNumEdges = 6;
    solid.Cell3DNumFaces = 4;

    return solid;
}

GeodeticSolid build_octahedron()
{
    GeodeticSolid solid;

    solid.NumCell0D = 6;
    solid.NumCell1D = 12;
    solid.NumCell2D = 8;

    solid.Cell0DId.reserve(solid.NumCell0D);
    solid.Cell1DId.resize(solid.NumCell1D);
    solid.Cell2DId.reserve(solid.NumCell2D);
    
    solid.Cell0DCoordinates = Eigen::MatrixXd::Zero(3, solid.NumCell0D);
    solid.Cell1DExtrema = Eigen::MatrixXi::Zero(2, solid.NumCell1D);

    solid.Cell2DVertices.reserve(solid.NumCell2D);
    solid.Cell2DEdges.reserve(solid.NumCell2D);

    Eigen::Matrix<double, 6, 3> vertices;
    vertices << 0.0,  0.0,  1.0,
                0.0,  0.0, -1.0,
                1.0,  0.0,  0.0,
                0.0,  1.0,  0.0,
               -1.0,  0.0,  0.0,
                0.0, -1.0,  0.0;
                

    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        solid.Cell0DId.push_back(i);
            
        for (int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(j, i) = vertices(i, j);
        }
    }

    //Spigoli del poliedro

    int ctr = 0;
  
    for (int j = 2; j <= 5; ++j) // ciclo for per riempire tutti gli spigoli uscenti dai poli (8 spigoli su 12)
    {
        solid.Cell1DId[ctr] = ctr;
        solid.Cell1DExtrema(0, ctr) = solid.Cell0DId[0]; 
        solid.Cell1DExtrema(1, ctr) = solid.Cell0DId[j];

        solid.Cell1DId[ctr + 4] = ctr + 4;
        solid.Cell1DExtrema(0, ctr + 4) = solid.Cell0DId[1];
        solid.Cell1DExtrema(1, ctr + 4) = solid.Cell0DId[j];
        ctr++;
    }

    ctr += 4;

    for (int j = 2; j < 5; ++j) // ciclo for per riempire gli spigoli che uniscono i vertici sul piano xy (4 spigoli su 12)
    {
        solid.Cell1DId[ctr] = ctr;
        solid.Cell1DExtrema(0, ctr) = solid.Cell0DId[j];
        solid.Cell1DExtrema(1, ctr) = solid.Cell0DId[j + 1];
        ctr++;            
    }
    
    solid.Cell1DId[ctr] = ctr;
    solid.Cell1DExtrema(0, ctr) = solid.Cell0DId[5];
    solid.Cell1DExtrema(1, ctr) = solid.Cell0DId[2];
    
    //Facce del poliedro
    ctr = 0;

    for (int j = 0; j < 2; ++j)
    {
        for (int k = 2; k <= 4; ++k)
        {
            solid.Cell2DId.push_back(ctr);
            solid.Cell2DNumVertices.push_back(3);
            solid.Cell2DNumEdges.push_back(3);
                
            solid.Cell2DVertices[ctr].push_back(ctr);
            solid.Cell2DVertices[ctr].push_back(j);
            solid.Cell2DVertices[ctr].push_back(k);
            solid.Cell2DVertices[ctr].push_back(k+1);
            ctr++;
            if(k + 1 == 5)
            {
                solid.Cell2DId.push_back(ctr);
                solid.Cell2DNumVertices.push_back(3);
                solid.Cell2DNumEdges.push_back(3);
                    
                solid.Cell2DVertices[ctr].push_back(ctr);
                solid.Cell2DVertices[ctr].push_back(j);
                solid.Cell2DVertices[ctr].push_back(k+1);
                solid.Cell2DVertices[ctr].push_back(2);
                ctr++;
            }
            
        }
    }

    //Spigoli Celle 2D

    for(int i = 0; i < solid.NumCell2D; ++i)
    {
        solid.Cell2DEdges[i].push_back(i); // questi sono gli indici dei vertici e degli spigoli di ogni faccia
        solid.Cell2DEdges[i].push_back(i); // sembra uguale alla riga prima, ma nel modo in cui ho ordinato gli spigoli tutte le facce hanno come primo spigolo lo spigolo di indice i

        int v = 8 + (i % 4);
        solid.Cell2DEdges[i].push_back(v);

        int u = (i + 1) % 4;

        if (i + 1 == 4)
        {
            solid.Cell2DEdges[i].push_back(0);
        }
        else if(i + 1 == 8)
        {
            solid.Cell2DEdges[i].push_back(4);
        }
        else if(i >= 0 && i < 3)
        {
            solid.Cell2DEdges[i].push_back(u); 
        }
        else if(i >= 4 && i < 7)
        {
            solid.Cell2DEdges[i].push_back(u + 4); 
        }
    }

    return solid;
}

void unit_sphere_projection(double& x, double& y, double& z)
{
	double norm = sqrt(x*x + y*y + z*z);
	
	if (norm == 0)
	{
		cerr << "Cannot project (0, 0, 0) onto the unit sphere" << endl;
		return;
	}
	
	x /= norm;
	y /= norm;
	z /= norm;
	
	return;
}		

void triangulation_c1(const unsigned int b, unsigned int q, GeodeticSolid& solid)
{
    unsigned int T = b*b;

    if(q == 3)
    {
        solid.Cell0DId.reserve(2*T + 2);
        solid.Cell1DId.reserve(6*T);
        solid.Cell2DId.reserve(4*T);
        
        solid.Cell0DCoordinates.conservativeResize(3, 2*T + 2);
        solid.Cell1DExtrema.conservativeResize(2, 6*T);

        solid.Cell2DNumVertices.reserve(4*T);
        solid.Cell2DNumEdges.reserve(4*T);

        solid.Cell2DVertices.reserve(4*T);
        solid.Cell2DEdges.reserve(4*T);
    }
    else if(q == 4)
    {
        solid.Cell0DId.reserve(4*T + 2);
        solid.Cell1DId.reserve(12*T);
        solid.Cell2DId.reserve(8*T);

        solid.Cell0DCoordinates.conservativeResize(3, 4*T + 2);
        solid.Cell1DExtrema.conservativeResize(2, 12*T);
        
        solid.Cell2DNumVertices.reserve(8*T);
        solid.Cell2DNumEdges.reserve(8*T);
        
        solid.Cell2DVertices.reserve(8*T);
        solid.Cell2DEdges.reserve(8*T);
    }
    else if(q == 5)
    {
        solid.Cell0DId.reserve(10*T + 2);
        solid.Cell1DId.reserve(30*T);
        solid.Cell2DId.reserve(20*T);

        solid.Cell0DCoordinates.conservativeResize(3, 10*T + 2);
        solid.Cell1DExtrema.conservativeResize(2, 30*T);
        
        solid.Cell2DNumVertices.reserve(20*T);
        solid.Cell2DNumEdges.reserve(20*T);
        
        solid.Cell2DVertices.reserve(20*T);
        solid.Cell2DEdges.reserve(20*T);
    }


    unsigned int ctr0D = solid.NumCell0D;
    unsigned int ctr1D = 0;
    unsigned int ctr2D = solid.NumCell2D;

    if (b == 1)
    {
        return;
    }
    
    Eigen::MatrixXi Cell1D_frag = Eigen::MatrixXi::Zero(solid.NumCell1D, b + 1);
    vector <double> w(3);
    vector <double> v(3);
    vector <double> x(3);
    vector <double> y(3);
    vector <double> z(3);
    unsigned int Id_P, Id_Q, Id_R;
    unsigned int P, Q, R;

    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        unsigned int P = solid.Cell1DExtrema(0, i);
        unsigned int Q = solid.Cell1DExtrema(1, i);
        Cell1D_frag(i, 0) = P;
        Cell1D_frag(i, b) = Q;

        for(unsigned int j = 0; j < 3; ++j)
        {
            w[j] = (solid.Cell0DCoordinates(j, Q) - solid.Cell0DCoordinates(j, P)) / b;
        }

        for (unsigned int j = 1; j <= b-1; ++j)
        {
            solid.Cell0DId.push_back(ctr0D);
            for (unsigned int k = 0; k < 3; ++k)
            {
                solid.Cell0DCoordinates(k, ctr0D) = solid.Cell0DCoordinates(k, P) + w[k]*j;
            }
            Cell1D_frag(i, j) = ctr0D;
            ctr0D++;
        }
    }

    if (b >= 3)
    {
    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
        {
            Id_P = solid.Cell2DVertices[i][1];
            Id_Q = solid.Cell2DVertices[i][2];
            Id_R = solid.Cell2DVertices[i][3];
            
            for (unsigned int j = 0; j < 3; ++j)
            {
                w[j] = (solid.Cell0DCoordinates(j, Id_Q) - solid.Cell0DCoordinates(j, Id_P)) / b;
                v[j] = (solid.Cell0DCoordinates(j, Id_R) - solid.Cell0DCoordinates(j, Id_P)) / b;
            }
            for (unsigned int j = 2; j <= b-1; ++j)
            {
                for (unsigned int k = 0; k < 3; ++k)
                {
                    x[k] = solid.Cell0DCoordinates(k, Id_P) + w[k]*j;
                    y[k] = solid.Cell0DCoordinates(k, Id_P) + v[k]*j;

                    z[k] = (y[k] - x[k]) / j;
                }

                for (unsigned int k = 1; k <= j-1; ++k)
                {
                    solid.Cell0DId.push_back(ctr0D);
                    for (unsigned int h = 0; h < 3; ++h)
                    {
                        solid.Cell0DCoordinates(h, ctr0D) = x[h] + z[h]*k;
                    }
                    ctr0D++;
                }
            }
        }
        return;
    }
}



}// namespace GeodeticLibrary