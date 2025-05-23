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

void fill_Cell3D(GeodeticSolid& solid)
{
    solid.Cell3DNumVertices = solid.NumCell0D;
    solid.Cell3DNumEdges = solid.NumCell1D;
    solid.Cell3DNumFaces = solid.NumCell2D;

    solid.Cell3DVertices.resize(solid.NumCell0D);
    solid.Cell3DEdges.resize(solid.NumCell1D);
    solid.Cell3DFaces.resize(solid.NumCell2D);

    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        solid.Cell3DVertices[i] = i;
    }
    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        solid.Cell3DEdges[i] = i;
    }
    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        solid.Cell3DFaces[i] = i;
    }
}

void unit_sphere_projection(double& x, double& y, double& z)
{
    double norm = std::sqrt(x*x + y*y + z*z);
    if (norm > 0.0)
    {
        x /= norm;
        y /= norm;
        z /= norm;
    }
    else
    {
        cout << "Error: Zero vector cannot be projected onto the unit sphere." << endl;
    }
}

void print_GeodeticSolid(GeodeticSolid& solid)
{
    //Cell0D
    std::ofstream Cell0D("Cell0D.txt");
    if (!Cell0D.is_open())
    {
        std::cerr << "Errore: impossibile aprire il file." << std::endl;
    }

    Cell0D << "Id; Coordinates" << std::endl;
    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        Cell0D << solid.Cell0DId[i] << "; (";
        for (unsigned int j = 0; j < 3; ++j)       
        {
            if (j == 2)
            {
                Cell0D << solid.Cell0DCoordinates(j, i);
            }
            else
            {
                Cell0D << solid.Cell0DCoordinates(j, i) << ", ";
            }
        }
        Cell0D << ")" << std::endl;
    }
    Cell0D.close();

    //Cell1D
    std::ofstream Cell1D("Cell1D.txt");
    if (!Cell1D.is_open())
    {
        std::cerr << "Errore: impossibile aprire il file." << std::endl;
    }

    Cell1D << "Id; Extrema" << std::endl;
    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        Cell1D << solid.Cell1DId[i] << "; {";
        for (unsigned int j = 0; j < 2; ++j)
        {
            if (j == 0)
            {
                Cell1D << solid.Cell1DExtrema(j, i) << ", ";
            }
            else
            {
                Cell1D << solid.Cell1DExtrema(j, i);
            }
        }
        Cell1D << "}" << std::endl;
    }
    Cell1D.close();

    //Cell2D
    std::ofstream Cell2D("Cell2D.txt");
    if (!Cell2D.is_open())
    {
        std::cerr << "Errore: impossibile aprire il file." << std::endl;
    }

    Cell2D << "Id; Vertices; Edges" << std::endl;
    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        Cell2D << solid.Cell2DId[i] << "; {";
        for (unsigned int j = 0; j < solid.Cell2DNumVertices[i]; ++j)
        {
            if (j == solid.Cell2DNumVertices[i] - 1)
            {
                Cell2D << solid.Cell2DVertices[i][j];
            }
            else
            {
                Cell2D << solid.Cell2DVertices[i][j] << ", ";
            }
        }
        Cell2D << "}; {";
        for (unsigned int j = 0; j < solid.Cell2DNumEdges[i]; ++j)
        {
            if (j == solid.Cell2DNumEdges[i] - 1)
            {
                Cell2D << solid.Cell2DEdges[i][j];
            }
            else
            {
                Cell2D << solid.Cell2DEdges[i][j] << ", ";
            }
        }
        Cell2D << "}" << std::endl;
    }
    Cell2D.close();

    //Cell3D
    std::ofstream Cell3D("Cell3D.txt");
    if (!Cell3D.is_open())
    {
        std::cerr << "Errore: impossibile aprire il file." << std::endl;
    }

    Cell3D << "Cell0DId: ";
    for (unsigned int j = 0; j < solid.Cell3DNumVertices; ++j)
    {
        if (j == solid.Cell3DNumVertices - 1)
        {
            Cell3D << solid.Cell3DVertices[j];
        }
        else
        {
            Cell3D << solid.Cell3DVertices[j] << ", ";
        }
    }
    Cell3D << std::endl;

    Cell3D << "Cell1DId: ";
    for (unsigned int j = 0; j < solid.Cell3DNumEdges; ++j)
    {
        if (j == solid.Cell3DNumEdges - 1)
        {
            Cell3D << solid.Cell3DEdges[j];
        }
        else
        {
            Cell3D << solid.Cell3DEdges[j] << ", ";
        }
    }
    Cell3D << std::endl;

    Cell3D << "Cell2DId: ";
    for (unsigned int j = 0; j < solid.Cell3DNumFaces; ++j)
    {
        if (j == solid.Cell3DNumFaces - 1)
        {
            Cell3D << solid.Cell3DFaces[j];
        }
        else
        {
            Cell3D << solid.Cell3DFaces[j] << ", ";
        }
    }
    Cell3D << std::endl;
    Cell3D.close();
   
}

GeodeticSolid build_tetrahedron()
{
    GeodeticSolid solid;
    
    //Size
    solid.NumCell0D = 4;
    solid.Cell0DId.resize(solid.NumCell0D);
    solid.Cell0DCoordinates = Eigen::MatrixXd::Zero(3, solid.NumCell0D);

    solid.NumCell1D = 6;
    solid.Cell1DId.resize(solid.NumCell1D);
    solid.Cell1DExtrema = Eigen::MatrixXi::Zero(2, solid.NumCell1D);

    solid.NumCell2D = 4;
    solid.Cell2DId.resize(solid.NumCell2D);
    solid.Cell2DNumVertices.resize(solid.NumCell2D);
    solid.Cell2DNumEdges.resize(solid.NumCell2D);
    solid.Cell2DVertices.resize(solid.NumCell2D);
    solid.Cell2DEdges.resize(solid.NumCell2D);

    //Cell0D
    Eigen::Matrix<double, 4, 3> vertices;
    vertices <<  1.0,  1.0,  1.0,
                -1.0, -1.0,  1.0,
                -1.0,  1.0, -1.0,
                 1.0, -1.0, -1.0;

    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        for (unsigned int i = 0; i < solid.NumCell0D; ++i)
	    {
            unit_sphere_projection(vertices(i, 0), vertices(i, 1), vertices(i, 2));
        }
        solid.Cell0DId[i] = i;
        for (unsigned int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(j, i) = vertices(i, j);
        }
    }

    //Cell1D
    unsigned int ctr = 0;
    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        solid.Cell1DId[i] = i;
        
        for (int j = i+1; j <= 3; ++j)
        {
            solid.Cell1DExtrema(0, ctr) = solid.Cell0DId[i];
            solid.Cell1DExtrema(1, ctr) = solid.Cell0DId[j];
            ctr++;
        }
    }

    //Cell2D
    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        int v = 0;

        solid.Cell2DId[i] = i;
        solid.Cell2DNumVertices[i] = 3;
        solid.Cell2DNumEdges[i] = 3;

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

    //Cell3D
    fill_Cell3D(solid);

    return solid;
}

GeodeticSolid build_octahedron()
{
    GeodeticSolid solid;

    //Size
    solid.NumCell0D = 6;
    solid.Cell0DId.resize(solid.NumCell0D);
    solid.Cell0DCoordinates = Eigen::MatrixXd::Zero(3, solid.NumCell0D);
    
    solid.NumCell1D = 12;
    solid.Cell1DId.resize(solid.NumCell1D);
    solid.Cell1DExtrema = Eigen::MatrixXi::Zero(2, solid.NumCell1D);

    solid.NumCell2D = 8;
    solid.Cell2DId.resize(solid.NumCell2D);
    solid.Cell2DNumVertices.resize(solid.NumCell2D);
    solid.Cell2DNumEdges.resize(solid.NumCell2D);
    solid.Cell2DVertices.resize(solid.NumCell2D);
    solid.Cell2DEdges.resize(solid.NumCell2D);

    //Cell0D
    Eigen::Matrix<double, 6, 3> vertices;
    vertices << 0.0,  0.0,  1.0,
                0.0,  0.0, -1.0,
                1.0,  0.0,  0.0,
                0.0,  1.0,  0.0,
               -1.0,  0.0,  0.0,
                0.0, -1.0,  0.0;
    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        solid.Cell0DId[i] = i;
            
        for (int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(j, i) = vertices(i, j);
        }
    }

    //Cell1D
    int ctr = 0;
  
    for (unsigned int j = 2; j <= 5; ++j) // ciclo for per riempire tutti gli spigoli uscenti dai poli (8 spigoli su 12)
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

    for (unsigned int j = 2; j <= 5; ++j) // ciclo for per riempire gli spigoli che uniscono i vertici sul piano xy (4 spigoli su 12)
    {
        solid.Cell1DId[ctr] = ctr;
        solid.Cell1DExtrema(0, ctr) = solid.Cell0DId[j];
        if (j == 5)
        {
            solid.Cell1DExtrema(1, ctr) = solid.Cell0DId[2];
        }
        else 
        {
            solid.Cell1DExtrema(1, ctr) = solid.Cell0DId[j + 1];
        }
        ctr++;            
    }
    
    //Cell2D
    ctr = 0;
    for (unsigned int j = 0; j < 2; ++j)
    {
        for (unsigned int k = 2; k <= 4; ++k)
        {
            solid.Cell2DId[ctr] = ctr;
            solid.Cell2DNumVertices[ctr] = 3;
            solid.Cell2DNumEdges[ctr] = 3;
            
            solid.Cell2DVertices[ctr].push_back(j);
            solid.Cell2DVertices[ctr].push_back(k);
            solid.Cell2DVertices[ctr].push_back(k+1);
            ctr++;

            if(k + 1 == 5)
            {
                solid.Cell2DId[ctr] = ctr;
                solid.Cell2DNumVertices[ctr] = 3;
                solid.Cell2DNumEdges[ctr] = 3;
                
                solid.Cell2DVertices[ctr].push_back(j);
                solid.Cell2DVertices[ctr].push_back(k+1);
                solid.Cell2DVertices[ctr].push_back(2);
                ctr++;
            }
            
        }
    }

    for(unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        // questi sono gli indici dei vertici e degli spigoli di ogni faccia
        solid.Cell2DEdges[i].push_back(i); // sembra uguale alla riga prima, ma nel modo in cui ho ordinato gli spigoli tutte le facce hanno come primo spigolo lo spigolo di indice i

        unsigned int v = 8 + (i % 4);
        solid.Cell2DEdges[i].push_back(v);

        unsigned int u = (i + 1) % 4;

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

    //Cell3D
    fill_Cell3D(solid);

    return solid;
}

GeodeticSolid build_icosahedron()
{
    GeodeticSolid solid;
    
    solid.NumCell0D = 12;
	solid.Cell0DId.resize(solid.NumCell0D);
	solid.Cell0DCoordinates = Eigen::MatrixXd::Zero(3, solid.NumCell0D);

    solid.NumCell1D = 30;
	solid.Cell1DId.resize(solid.NumCell1D);
	solid.Cell1DExtrema = Eigen::MatrixXi::Zero(2, solid.NumCell1D);

    solid.NumCell2D = 20;
	solid.Cell2DId.resize(solid.NumCell2D);
    solid.Cell2DNumVertices.resize(solid.NumCell2D);
	solid.Cell2DNumEdges.resize(solid.NumCell2D);
    solid.Cell2DVertices.resize(solid.NumCell2D);
    solid.Cell2DEdges.resize(solid.NumCell2D);

	//Cell0D
    Eigen::Matrix<double, 12, 3> vertices;
	const double phi = (1.0 + std::sqrt(5.0)) / 2.0;
    vertices << 
		 0.0,  1.0,  phi,
		   
         0.0, -1.0,  phi,
		 phi,  0.0,  1.0,
		 1.0,  phi,  0.0,
		-1.0,  phi,  0.0,
		-phi,  0.0,  1.0,
		  
		 1.0, -phi,  0.0,
		 phi,  0.0, -1.0,
		 0.0,  1.0, -phi,
		-phi,  0.0, -1.0,
		-1.0, -phi,  0.0,
		  
         0.0, -1.0, -phi;

	for (unsigned int i = 0; i < solid.NumCell0D; ++i)
	{
		unit_sphere_projection(vertices(i, 0), vertices(i, 1), vertices(i, 2));
	}


    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        solid.Cell0DId[i] = i;

        for (int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(j, i) = vertices(i, j);
        }
    }

	//Cell1D
	for (unsigned int j = 0; j < 5; ++j)
	{
		solid.Cell1DId[j] = j;
		solid.Cell1DExtrema(0, j) = solid.Cell0DId[0];
		solid.Cell1DExtrema(1, j) = solid.Cell0DId[j + 1];
		
		solid.Cell1DId[j + 5] = j + 5;
		solid.Cell1DExtrema(0, j + 5) = solid.Cell0DId[11];
		solid.Cell1DExtrema(1, j + 5) = solid.Cell0DId[j + 6];

		solid.Cell1DId[j+10] = j + 10;
		solid.Cell1DExtrema(0, j + 10) = solid.Cell0DId[j + 1];
		solid.Cell1DExtrema(1, j + 10) = solid.Cell0DId[(j + 1) % 5 + 1];

		solid.Cell1DId[j + 15] = j + 15;
		solid.Cell1DExtrema(0, j + 15) = solid.Cell0DId[j + 6];
		solid.Cell1DExtrema(1, j + 15) = solid.Cell0DId[(j + 1) % 5 + 6];

		solid.Cell1DId[j + 20] = j + 20;
		solid.Cell1DExtrema(0, j + 20) = solid.Cell0DId[j + 1];
		solid.Cell1DExtrema(1, j + 20) = solid.Cell0DId[j + 6];
		
		solid.Cell1DId[j + 25] = j + 25;
           	if (j == 4)
		{
			solid.Cell1DExtrema(0, j + 25) = solid.Cell0DId[1];
		}
		else 
		{
			solid.Cell1DExtrema(0, j + 25) = solid.Cell0DId[j + 2];
		}
		solid.Cell1DExtrema(1, j + 25) = solid.Cell0DId[j + 6];
	}


	//Cell2D 
    for (int i = 0; i < 5; ++i)
    {
        solid.Cell2DId[i] = i;
        solid.Cell2DNumVertices[i] = 3;
        solid.Cell2DNumEdges[i] = 3;
		solid.Cell2DVertices[i].push_back(0);
		solid.Cell2DVertices[i].push_back(i + 1);
		solid.Cell2DVertices[i].push_back((i + 1) % 5 + 1);
        solid.Cell2DEdges[i].push_back(i); // Faccia 0, i, i+1
        solid.Cell2DEdges[i].push_back(i + 10);
        solid.Cell2DEdges[i].push_back(i + 1);

		solid.Cell2DId[i + 5] = i + 5;
		solid.Cell2DNumVertices[i + 5] = 3;
		solid.Cell2DNumEdges[i + 5] = 3;
		solid.Cell2DVertices[i + 5].push_back(11);
		solid.Cell2DVertices[i + 5].push_back(i + 6);
		solid.Cell2DVertices[i + 5].push_back((i + 1) % 5 + 6);
        solid.Cell2DEdges[i + 5].push_back(i + 5); // Faccia 11, i+6, i+7
        solid.Cell2DEdges[i + 5].push_back(i + 15);
        solid.Cell2DEdges[i + 5].push_back((i + 1) % 5 + 6);

		solid.Cell2DId[i + 10] = i + 10;
		solid.Cell2DNumVertices[i + 10] = 3;
		solid.Cell2DNumEdges[i + 10] = 3;
		solid.Cell2DVertices[i + 10].push_back(i + 1);
		solid.Cell2DVertices[i + 10].push_back((i + 1) % 5 + 1);
		solid.Cell2DVertices[i + 10].push_back(i + 6);
        solid.Cell2DEdges[i + 10].push_back(i + 10); // Faccia i+1, i+2, i+6
        solid.Cell2DEdges[i + 10].push_back(i + 25);
        solid.Cell2DEdges[i + 10].push_back(i + 20);

		solid.Cell2DId[i + 15] = i + 15;
		solid.Cell2DNumVertices[i + 15] = 3;
		solid.Cell2DNumEdges[i + 15] = 3;
		solid.Cell2DVertices[i + 15].push_back(i + 6);
		solid.Cell2DVertices[i + 15].push_back((i + 1) % 5 + 1);
		if (i == 4)
		{
			solid.Cell2DVertices[i + 15].push_back(6);
        }
		else
		{
			solid.Cell2DVertices[i + 15].push_back(i + 7);
		}
        solid.Cell2DEdges[i + 15].push_back(i + 25); // Faccia i+6, i+2, i+7
        solid.Cell2DEdges[i + 15].push_back((i + 1) % 5 + 20);
        solid.Cell2DEdges[i + 15].push_back(i + 15);
    }

    //Cell3D
    fill_Cell3D(solid);

    return solid;
} 

void triangulation_c1(const unsigned int b, unsigned int q, GeodeticSolid& solid)
{
    unsigned int T = b*b;

    unsigned int ctr0D = solid.NumCell0D;
    unsigned int ctr1D = 0;
    unsigned int ctr2D = solid.NumCell2D;

    if(q == 3)
    {
        solid.NumCell0D = 2*T + 2;
        
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
        solid.NumCell0D = 4*T + 2;

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
        solid.NumCell0D = 10*T + 2;

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
            Id_P = solid.Cell2DVertices[i][0]; 
            Id_Q = solid.Cell2DVertices[i][1];
            Id_R = solid.Cell2DVertices[i][2];
        
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
        }
}


void build_polygon_c1(unsigned int p, unsigned int q, unsigned int b, unsigned int c)
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
    return;    
}
}// namespace GeodeticLibrary