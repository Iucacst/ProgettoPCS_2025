#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

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

void GeodeticSolid_projection(GeodeticSolid& solid)
{
    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        unit_sphere_projection(solid.Cell0DCoordinates(0, i), solid.Cell0DCoordinates(1, i), solid.Cell0DCoordinates(2, i));
    }
}

void print_GeodeticSolid(GeodeticSolid& solid)
{
    //Cell0D
    ofstream Cell0D("Cell0D.txt");
    if (!Cell0D.is_open())
    {
        cerr << "Errore: impossibile aprire il file." << endl;
    }

    Cell0D << "Id; Coordinates" << endl;
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
        Cell0D << ")" << endl;
    }
    Cell0D.close();

    //Cell1D
    ofstream Cell1D("Cell1D.txt");
    if (!Cell1D.is_open())
    {
        cerr << "Errore: impossibile aprire il file." << endl;
    }

    Cell1D << "Id; Extrema" << endl;
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
        Cell1D << "}" << endl;
    }
    Cell1D.close();

    //Cell2D
    ofstream Cell2D("Cell2D.txt");
    if (!Cell2D.is_open())
    {
        cerr << "Errore: impossibile aprire il file." << endl;
    }

    Cell2D << "Id; Vertices; Edges" << endl;
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
        Cell2D << "}" << endl;
    }
    Cell2D.close();

    //Cell3D
    ofstream Cell3D("Cell3D.txt");
    if (!Cell3D.is_open())
    {
        cerr << "Errore: impossibile aprire il file." << endl;
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
    Cell3D << endl;

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
    Cell3D << endl;

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
    Cell3D << endl;
    Cell3D.close();
   
}

unsigned int find_edge(unsigned int P, unsigned int Q, GeodeticSolid& solid)
{
    unsigned int edge_id = -1;
    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        if ((solid.Cell1DExtrema(0, i) == P && solid.Cell1DExtrema(1, i) == Q) ||
            (solid.Cell1DExtrema(0, i) == Q && solid.Cell1DExtrema(1, i) == P))
        {
            edge_id = i;
            break;
        }
    }
    if (edge_id == -1)
    {
        cerr << "Edge not found between vertices " << P << " and " << Q << endl;
        return edge_id; // Return -1 to indicate not found
    }

    return edge_id;
}

bool check_ordination(GeodeticSolid& solid)
{
    if(solid.NumCell2D == 0)
    {
        cerr << "No faces to check." << endl;
        return false;
    }

    for(unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        
        unsigned int E = solid.Cell2DNumEdges[i];
        for(unsigned int e = 0; e < E; ++e)
        {
            unsigned int l1 = solid.Cell2DEdges[i][e];
            unsigned int l2 = solid.Cell2DEdges[i][(e + 1) % E];

            unsigned int e11 = solid.Cell1DExtrema(0, l1);
            unsigned int e12 = solid.Cell1DExtrema(1, l1);
            unsigned int e21 = solid.Cell1DExtrema(0, l2);
            unsigned int e22 = solid.Cell1DExtrema(1, l2);
            
            if(e11 != e21 && e11 != e22 &&
               e12 != e21 && e12 != e22)
            {
                cerr << "Edges " << l1 << " and " << l2 << " are not ordered correctly in face " << i << endl;
                return false;
            }

            unsigned int v1 = solid.Cell2DVertices[i][e];
            if(v1 != e11 && v1 != e12)
            {
                cerr << "Vertex " << v1 << " is not stored correctly in face " << i << endl;
                return false;
            }
        }
    }
    cout << "For every face edges and vertices are correctly stored." << endl;
    return true;
}

double find_distance(GeodeticSolid& solid, unsigned int P, unsigned int Q)
{
    if (P >= solid.NumCell0D || Q >= solid.NumCell0D || P < 0 || Q < 0)
    {
        cerr << "Error: Vertex index out of bounds." << endl;
        return -1.0; 
    }

    double dx = solid.Cell0DCoordinates(0, P) - solid.Cell0DCoordinates(0, Q);
    double dy = solid.Cell0DCoordinates(1, P) - solid.Cell0DCoordinates(1, Q);
    double dz = solid.Cell0DCoordinates(2, P) - solid.Cell0DCoordinates(2, Q);

    return sqrt(dx*dx + dy*dy + dz*dz);
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

    solid.vertices_short_path.resize(solid.NumCell0D);
    solid.edges_short_path.resize(solid.NumCell1D);

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
        else if(/*i >= 0 && */ i < 3)
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

    solid.vertices_short_path.resize(solid.NumCell0D);
    solid.edges_short_path.resize(solid.NumCell1D);

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
        solid.Cell2DEdges[i].push_back((i + 1) % 5);

		solid.Cell2DId[i + 5] = i + 5;
		solid.Cell2DNumVertices[i + 5] = 3;
		solid.Cell2DNumEdges[i + 5] = 3;
		solid.Cell2DVertices[i + 5].push_back(11);
		solid.Cell2DVertices[i + 5].push_back(i + 6);
		solid.Cell2DVertices[i + 5].push_back((i + 1) % 5 + 6);
        solid.Cell2DEdges[i + 5].push_back(i + 5); // Faccia 11, i+6, i+7
        solid.Cell2DEdges[i + 5].push_back(i + 15);
        solid.Cell2DEdges[i + 5].push_back((i + 1) % 5 + 5);

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

    solid.vertices_short_path.resize(solid.NumCell0D);
    solid.edges_short_path.resize(solid.NumCell1D);

    return solid;
} 

void triangulation_c1(const unsigned int b, unsigned int q, GeodeticSolid& solid)
{
    unsigned int T = b*b;

    unsigned int ctr0D = solid.NumCell0D;
    unsigned int ctr1D = 0;
    unsigned int ctr2D = 0;

    if(q == 3)
    {
        solid.NumCell0D = 2*T + 2;
        
        solid.Cell0DId.reserve(2*T + 2);
        solid.Cell1DId.reserve(6*T);
        solid.Cell2DId.reserve(4*T);
        
        solid.Cell0DCoordinates.conservativeResize(3, 2*T + 2);
        solid.Cell1DExtrema.conservativeResize(2, 6*T); 

        solid.Cell2DNumVertices.resize(4*T);
        solid.Cell2DNumEdges.resize(4*T);

        solid.Cell2DVertices.resize(4*T);
        solid.Cell2DEdges.resize(4*T);
    }
    else if(q == 4)
    {
        solid.NumCell0D = 4*T + 2;

        solid.Cell0DId.reserve(4*T + 2);
        solid.Cell1DId.reserve(12*T);
        solid.Cell2DId.reserve(8*T);

        solid.Cell0DCoordinates.conservativeResize(3, 4*T + 2);
        solid.Cell1DExtrema.conservativeResize(2, 12*T);
        
        solid.Cell2DNumVertices.resize(8*T);
        solid.Cell2DNumEdges.resize(8*T);
        
        solid.Cell2DVertices.resize(8*T);
        solid.Cell2DEdges.resize(8*T);
    }
    else if(q == 5)
    {
        solid.NumCell0D = 10*T + 2;

        solid.Cell0DId.reserve(10*T + 2);
        solid.Cell1DId.reserve(30*T);
        solid.Cell2DId.reserve(20*T);

        solid.Cell0DCoordinates.conservativeResize(3, 10*T + 2);
        solid.Cell1DExtrema.conservativeResize(2, 30*T);
        
        solid.Cell2DNumVertices.resize(20*T);
        solid.Cell2DNumEdges.resize(20*T);
        
        solid.Cell2DVertices.resize(20*T);
        solid.Cell2DEdges.resize(20*T);
    }


    if (b == 1)
    {
        return;
    }
    
    solid.Cell1D_frag.resize(solid.NumCell1D, b + 1); 

    vector <double> w(3);
    vector <double> v(3);
    vector <double> x(3);
    vector <double> y(3);
    vector <double> z(3);
    unsigned int P, Q, R;
    unsigned int PQ, QR, RP;

    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        unsigned int P = solid.Cell1DExtrema(0, i);
        unsigned int Q = solid.Cell1DExtrema(1, i);
        solid.Cell1D_frag(i, 0) = P;
        solid.Cell1D_frag(i, b) = Q;

        for(unsigned int j = 0; j < 3; ++j)
        {
            w[j] = (solid.Cell0DCoordinates(j, Q) - solid.Cell0DCoordinates(j, P)) / b;
        }

        for (unsigned int j = 1; j <= b - 1; ++j)
        {
            solid.Cell0DId.push_back(ctr0D);
            for (unsigned int k = 0; k < 3; ++k)
            {
                solid.Cell0DCoordinates(k, ctr0D) = solid.Cell0DCoordinates(k, P) + w[k]*j;
            }
            solid.Cell1D_frag(i, j) = ctr0D;
            ctr0D++;
        }        
    }

    //Tolgo gli ID vecchi perchè non mi servono più a nulla
    solid.Cell1DId.clear();

    for(unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        for(unsigned int j = 0; j <= b - 1; ++j) // messo l'uguale
        {
            solid.Cell1DId.push_back(ctr1D);
            solid.Cell1DExtrema(0, ctr1D) = solid.Cell1D_frag(i, j);
            solid.Cell1DExtrema(1, ctr1D) = solid.Cell1D_frag(i, j + 1);
            ctr1D++;
        }
    }
    solid.NumCell1D = ctr1D; // Da fare per far funzionare il print del txt e per l'esportazione dei punti

    solid.Cell2D_frag.resize(solid.NumCell2D); 
    for (auto& mat : solid.Cell2D_frag) 
    {
        mat = Eigen::MatrixXi::Zero(b + 1, b + 1);       
    } 

    if (b >= 2)  // Parte interna
    {
        
        for (unsigned int i = 0; i < solid.NumCell2D; ++i)
        {
            P = solid.Cell2DVertices[i][0]; 
            Q = solid.Cell2DVertices[i][1];
            R = solid.Cell2DVertices[i][2];

            PQ = solid.Cell2DEdges[i][0];
            QR = solid.Cell2DEdges[i][1];
            RP = solid.Cell2DEdges[i][2];

            solid.Cell2D_frag[i](0,0) = P;
            solid.Cell2D_frag[i](b,0) = Q;
            solid.Cell2D_frag[i](b,b) = R;

            if(solid.Cell1D_frag(PQ, 0) == P && solid.Cell1D_frag(PQ, b) == Q) // Check in what direction the edges are "oriented"
            {
                for (unsigned int j = 1; j <= b-1; ++j)
                {
                    solid.Cell2D_frag[i](j, 0) = solid.Cell1D_frag(PQ , j);
                }
            }
            else
            {
                for (unsigned int j = 1; j <= b-1; ++j)
                {
                    solid.Cell2D_frag[i](j, 0) = solid.Cell1D_frag(PQ, b - j);
                }
            }

            if(solid.Cell1D_frag(QR, 0) == Q && solid.Cell1D_frag(QR, b) == R)
            {
                for (unsigned int j = 1; j <= b-1; ++j)
                {
                    solid.Cell2D_frag[i](b, j) = solid.Cell1D_frag(QR, j);
                }
            }
            else
            {
                for (unsigned int j = 1; j <= b-1; ++j)
                {
                    solid.Cell2D_frag[i](b, j) = solid.Cell1D_frag(QR, b - j);
                }
            }

            if(solid.Cell1D_frag(RP, 0) == P && solid.Cell1D_frag(RP, b) == R)
            {
                for (unsigned int j = 1; j <= b-1; ++j)
                {
                    solid.Cell2D_frag[i](j, j) = solid.Cell1D_frag(RP, j);
                }
            }
            else
            {
                for (unsigned int j = 1; j <= b-1; ++j)
                {
                    solid.Cell2D_frag[i](j, j) = solid.Cell1D_frag(RP, b-j);
                }
            }
            
            for (unsigned int j = 0; j < 3; ++j)
            {
                w[j] = (solid.Cell0DCoordinates(j, Q) - solid.Cell0DCoordinates(j, P)) / b;
                v[j] = (solid.Cell0DCoordinates(j, R) - solid.Cell0DCoordinates(j, P)) / b;
            }

            for (unsigned int j = 2; j <= b-1; ++j)
            {
                for (unsigned int k = 0; k < 3; ++k)
                {
                    x[k] = solid.Cell0DCoordinates(k, P) + w[k]*j;
                    y[k] = solid.Cell0DCoordinates(k, P) + v[k]*j;

                    z[k] = (y[k] - x[k]) / j; // difference vector to build the interior points
                }

                for (unsigned int k = 1; k <= j-1; ++k)
                {
                    solid.Cell0DId.push_back(ctr0D);
                    for (unsigned int h = 0; h < 3; ++h)
                    {
                        solid.Cell0DCoordinates(h, ctr0D) = x[h] + z[h]*k;
                        
                    }
                    solid.Cell2D_frag[i](j, k) = ctr0D;

                    ctr0D++;
                }
            }

            for (unsigned int j = 1; j <= b-1; ++j)
            {
                for(unsigned int k = 0; k < j; ++k) 
                {
                    solid.Cell1DId.push_back(ctr1D);
                    solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                    solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j, k + 1); 
                    ctr1D++;                   
                }
                
                for(unsigned int k = 0; k < j; ++k)
                {
                    solid.Cell1DId.push_back(ctr1D);
                    solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](b - k, b - j);
                    solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](b - k - 1, b - j);
                    ctr1D++;                    
                }

                for(unsigned int k = 0; k < j; ++k)
                {
                    solid.Cell1DId.push_back(ctr1D);
                    solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](b - j + k, k);
                    solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](b - j + k + 1, k + 1);
                    ctr1D++;
                }            
            }
        }

        solid.NumCell1D = ctr1D;
        solid.Cell2DId.clear();

        for (unsigned int i = 0; i < solid.NumCell2D; ++i)
        {
            for(unsigned int j = 0; j <= b - 1; ++j)
            {
                for(unsigned int k = 0; k <= j; ++k)
                {
                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();

                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;

                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    for (unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;
                    
                    if(j >= 1 && k < j)
                    {
                        solid.Cell2DVertices[ctr2D].clear();
                        solid.Cell2DEdges[ctr2D].clear();

                        solid.Cell2DId.push_back(ctr2D);
                        solid.Cell2DNumVertices[ctr2D] = 3;
                        solid.Cell2DNumEdges[ctr2D] = 3;

                        solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j, k));
                        solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j, k + 1));
                        solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                        for (unsigned int h = 0; h < 3; ++h)
                        {
                            PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                            solid.Cell2DEdges[ctr2D].push_back(PQ);
                        }
                        ctr2D++;
                    }  
                }
            }
        }

        solid.NumCell2D = ctr2D;

        fill_Cell3D(solid);

        solid.vertices_short_path.resize(solid.NumCell0D);
        solid.edges_short_path.resize(solid.NumCell1D);
    }
}

void triangulation_c2(const unsigned int b, unsigned int q, GeodeticSolid& solid)
{
    GeodeticSolid solid_tmp = solid;
    triangulation_c1(b, q, solid_tmp);
    unsigned int ctr = 0;

    unsigned int ctr0D = solid.NumCell0D;
    unsigned int ctr1D = 0;
    unsigned int ctr2D = 0;

    solid.Cell0DCoordinates.conservativeResize(3, solid.NumCell0D + solid.NumCell1D*(2*b - 1) + solid.NumCell2D*(3*(b*b)/2 - 3*b/2 + b) + 20);
    solid.Cell1DExtrema.conservativeResize(2, solid.NumCell1D*2*b + solid.NumCell2D*(9*(b*b)/2 + 3*b/2) + 20);
    solid.Cell2DVertices.resize(solid.NumCell2D*(3*(b*b) + 3*b));
    solid.Cell2DEdges.resize(solid.NumCell2D*(3*(b*b) + 3*b));
    solid.Cell2DId.clear();
    solid.Cell1DId.clear();

    solid.Cell2DNumVertices.resize(solid.NumCell2D*(3*(b*b) + 3*b));
    solid.Cell2DNumEdges.resize(solid.NumCell2D*(3*(b*b) + 3*b));
    solid.Cell2DId.reserve(solid.NumCell2D*(3*(b*b) + 3*b));

    solid.Cell1D_frag.resize(solid.NumCell1D, 2*b + 1);
    solid.Cell2D_frag.resize(solid.NumCell2D);
    for (auto& mat : solid.Cell2D_frag) 
    {
        mat = Eigen::MatrixXi::Zero(2*b + 1, 2*b + 1);       
    } 

    vector <double> w(3);
    vector <double> v(3);
    vector <double> x(3);
    vector <double> y(3);
    vector <double> z(3);
    unsigned int P, Q, R;
    unsigned int PQ, QR, RP;

    for (unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        unsigned int P = solid.Cell1DExtrema(0, i);
        unsigned int Q = solid.Cell1DExtrema(1, i);
        solid.Cell1D_frag(i, 0) = P;
        solid.Cell1D_frag(i, 2*b) = Q;

        for(unsigned int j = 0; j < 3; ++j)
        {
            w[j] = (solid.Cell0DCoordinates(j, Q) - solid.Cell0DCoordinates(j, P)) / (2*b);
        }

        for (unsigned int j = 1; j <= 2*b - 1; ++j)
        {
            solid.Cell0DId.push_back(ctr0D);
            for (unsigned int k = 0; k < 3; ++k)
            {
                solid.Cell0DCoordinates(k, ctr0D) = solid.Cell0DCoordinates(k, P) + w[k]*j;
            }
            solid.Cell1D_frag(i, j) = ctr0D;
            ctr0D++;
        }        
    }


    for(unsigned int i = 0; i < solid.NumCell1D; ++i)
    {
        for(unsigned int j = 0; j <= 2*b - 1; ++j) 
        {
            solid.Cell1DId.push_back(ctr1D);
            solid.Cell1DExtrema(0, ctr1D) = solid.Cell1D_frag(i, j);
            solid.Cell1DExtrema(1, ctr1D) = solid.Cell1D_frag(i, j + 1);
            ctr1D++;
        }
    }
    
    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        P = solid.Cell2DVertices[i][0]; 
        Q = solid.Cell2DVertices[i][1];
        R = solid.Cell2DVertices[i][2];

        PQ = solid.Cell2DEdges[i][0];
        QR = solid.Cell2DEdges[i][1];
        RP = solid.Cell2DEdges[i][2];

        solid.Cell2D_frag[i](0,0) = P;
        solid.Cell2D_frag[i](2*b,0) = Q;
        solid.Cell2D_frag[i](2*b, 2*b) = R;

        if(solid.Cell1D_frag(PQ, 0) == P && solid.Cell1D_frag(PQ, 2*b) == Q)
        {
            for (unsigned int j = 1; j < 2*b; ++j)
            {
                solid.Cell2D_frag[i](j, 0) = solid.Cell1D_frag(PQ , j);
            }
        }
        else
        {
            for (unsigned int j = 1; j < 2*b; ++j)
            {
                solid.Cell2D_frag[i](j, 0) = solid.Cell1D_frag(PQ, 2*b - j);
            }
        }

        if(solid.Cell1D_frag(QR, 0) == Q && solid.Cell1D_frag(QR, 2*b) == R)
        {
            for (unsigned int j = 1; j < 2*b; ++j)
            {
                solid.Cell2D_frag[i](2*b, j) = solid.Cell1D_frag(QR, j);
            }
        }
        else
        {
            for (unsigned int j = 1; j < 2*b; ++j)
            {
                solid.Cell2D_frag[i](2*b, j) = solid.Cell1D_frag(QR, 2*b - j);
            }
        }

        if(solid.Cell1D_frag(RP, 0) == P && solid.Cell1D_frag(RP, 2*b) == R)
        {
            for (unsigned int j = 1; j < 2*b; ++j)
            {
                solid.Cell2D_frag[i](j, j) = solid.Cell1D_frag(RP, j);
            }
        }
        else
        {
            for (unsigned int j = 1; j < 2*b; ++j)
            {
                solid.Cell2D_frag[i](j, j) = solid.Cell1D_frag(RP, 2*b-j);
            }
        }
    }

    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        for (unsigned int j = 1; j <= 2*b - 1; j += 2)
        {
            solid.Cell2D_frag[i](j, j+1) = solid.Cell2D_frag[i](j, j);
            solid.Cell2D_frag[i](j, j) = 0;
        }
    }

    if (b >= 3)
    {
        if (q == 3)
        {
            ctr = 4 + 6*(b - 1);
        }
        else if (q == 4)
        {
            ctr = 6 + 12*(b-1);
        }
        else if (q == 5)
        {
            ctr = 12 + 30*(b-1);
        }

        for (unsigned int i = ctr; i < solid_tmp.NumCell0D; ++i)
        {
            solid.Cell0DId.push_back(ctr0D);
            for (unsigned int j = 0; j < 3; ++j)
            {
                solid.Cell0DCoordinates(j, ctr0D) = solid_tmp.Cell0DCoordinates(j, i);
            }
            ctr0D++;
        }
    }


    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        for (unsigned int j = 1; j <= b - 1; ++j)
        {
            for (unsigned int k = 1; k <= j - 1; ++k)
            {
                P = solid_tmp.Cell2D_frag[i](j, k) ;
                Q = -1;

                for (unsigned int h = 0; h < ctr0D; ++h)
                {
                    if (solid.Cell0DCoordinates(0, h) == solid_tmp.Cell0DCoordinates(0, P) &&
                        solid.Cell0DCoordinates(1, h) == solid_tmp.Cell0DCoordinates(1, P) &&
                        solid.Cell0DCoordinates(2, h) == solid_tmp.Cell0DCoordinates(2, P))
                    {
                        Q = h;
                        break;
                    }
                }
                if (Q == -1)
                {
                    cout << "Vertex not found for P: " << P << endl;
                }

                solid.Cell2D_frag[i](2 * j, k*2) = Q;
            }
        }

    }
    
    
    for (unsigned int i = 0; i < solid_tmp.NumCell2D; ++i)
    {
        P = solid_tmp.Cell2DVertices[i][0]; 
        Q = solid_tmp.Cell2DVertices[i][1];
        R = solid_tmp.Cell2DVertices[i][2];

        solid.Cell0DId.push_back(ctr0D);
        for (unsigned int k = 0; k < 3; ++k)
        {
            solid.Cell0DCoordinates(k, ctr0D) = (solid_tmp.Cell0DCoordinates(k, P)+
                                                solid_tmp.Cell0DCoordinates(k, Q)+
                                                solid_tmp.Cell0DCoordinates(k, R)) / 3.0;
        }

        for (unsigned int j = 0; j < solid.NumCell2D; ++j) //Fill Cell2D with baricenter of the t1 faces
        {
            for (unsigned int k = 0; k <= 2*b - 2; k += 2)
            {
                for (unsigned int h = 0; h <= k; h += 2)
                {
                    P = solid.Cell2D_frag[j](k, h);
                    Q = solid.Cell2D_frag[j](k + 2, h);
                    R = solid.Cell2D_frag[j](k + 2, h + 2);

                    for (unsigned int d = 0; d < 3; ++d)
                    {
                        v[d] = (solid.Cell0DCoordinates(d, P) +
                              solid.Cell0DCoordinates(d, Q) +
                              solid.Cell0DCoordinates(d, R)) / 3.0;
                    }
                    if(v[0] == solid.Cell0DCoordinates(0, ctr0D) &&
                       v[1] == solid.Cell0DCoordinates(1, ctr0D) &&
                       v[2] == solid.Cell0DCoordinates(2, ctr0D))
                    {
                        solid.Cell2D_frag[j](k + 1, h + 1) = ctr0D;
                    }
                
                    if(h < k)
                    {
                        P = solid.Cell2D_frag[j](k, h);
                        Q = solid.Cell2D_frag[j](k, h + 2);
                        R = solid.Cell2D_frag[j](k + 2, h + 2);

                        for (unsigned int d = 0; d < 3; ++d)
                        {
                            v[d] = (solid.Cell0DCoordinates(d, P) +
                              solid.Cell0DCoordinates(d, Q) +
                              solid.Cell0DCoordinates(d, R)) / 3.0;
                        }
                        if(v[0] == solid.Cell0DCoordinates(0, ctr0D) &&
                            v[1] == solid.Cell0DCoordinates(1, ctr0D) &&
                            v[2] == solid.Cell0DCoordinates(2, ctr0D))
                        {
                            solid.Cell2D_frag[j](k + 1, h + 2) = ctr0D;
                            break;
                        }
                    }
                    
                }
            }
        }
        ctr0D++;
    }

    for (unsigned int i = 0; i < solid.NumCell2D; ++i)
    {
        for(unsigned int j = 1; j <= 2*b - 1; j += 2)
        {
            for(unsigned int k = 0; k <= j; ++k)
            {
                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j, k + 1);
                ctr1D++;
            }
            
            for(unsigned int k = 1; k <= j; k += 2)
            {
                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j - 1, k - 1);
                ctr1D++;

                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j + 1, k - 1);
                ctr1D++;

                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j + 1, k + 1);
                ctr1D++;
            }

            for(unsigned int k = 2; k <= j - 1; k += 2)
            {
                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j - 1, k);
                ctr1D++;

                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j + 1, k);
                ctr1D++;
                
                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j - 1, k - 2);
                ctr1D++;
            } 
        }
        for(unsigned int k = 1; k <= 2*b - 1; k += 2)
        {
            solid.Cell1DId.push_back(ctr1D);
            solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](2*b - 1, k);
            solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](2*b, k);
            ctr1D++;
        }
        for(unsigned int j = 1; j < 2*b - 1; j += 2) // Triangolazione dei baricentri aggiunta dopo 
        {
            for(unsigned int k = 1; k <= j; k += 2)
            {
                solid.Cell1DId.push_back(ctr1D);
                solid.Cell1DExtrema(0, ctr1D) = solid.Cell2D_frag[i](j, k);
                solid.Cell1DExtrema(1, ctr1D) = solid.Cell2D_frag[i](j + 2, k + 1);
                ctr1D++;
            }
            
        }
    }

    solid.NumCell0D = ctr0D;  
    solid.NumCell1D = ctr1D;

    for(unsigned int i = 0; i < solid.NumCell2D; ++i) // Starting from Cell2DFrag we build Cell2DVertices and Cell2DEdges
    {
        for(unsigned int j = 0; j <= 2*b - 2; j+=2)
        {        
            for(unsigned int k = 0; k <= j; k+= 2)
            {
                if(j < 2*b - 2)
                {
                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 2));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k));               
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 2));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k + 2));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k + 2));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 3, k + 2));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;


                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 3, k + 2));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;     
                }
                else
                {
                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 2));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 2));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k + 2));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k + 1));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;

                    solid.Cell2DVertices[ctr2D].clear();
                    solid.Cell2DEdges[ctr2D].clear();
                    solid.Cell2DId.push_back(ctr2D);
                    solid.Cell2DNumVertices[ctr2D] = 3;
                    solid.Cell2DNumEdges[ctr2D] = 3;
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 1, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k + 1));
                    solid.Cell2DVertices[ctr2D].push_back(solid.Cell2D_frag[i](j + 2, k + 2));
                    for(unsigned int h = 0; h < 3; ++h)
                    {
                        PQ = find_edge(solid.Cell2DVertices[ctr2D][h], solid.Cell2DVertices[ctr2D][(h + 1) % 3], solid);
                        solid.Cell2DEdges[ctr2D].push_back(PQ);
                    }
                    ctr2D++;
                }
            }
        }          
    }

    // Da fare per far funzionare il print del txt e per l'esportazione dei punti
    solid.NumCell2D = ctr2D;
    fill_Cell3D(solid);

    solid.vertices_short_path.resize(solid.NumCell0D);
    solid.edges_short_path.resize(solid.NumCell1D);
}

void shortest_path(unsigned int S, unsigned int V, GeodeticSolid& solid)
{
    if(S >= solid.NumCell0D || V >= solid.NumCell0D || S < 0 || V < 0)
    {
        cerr << "Invalid vertex indices." << endl;
        return;
    }
    if (S == V)
    {
        cout << S << " and " << V << " are the same vertex" << endl;
        return;
    }

    vector<double> dist(solid.NumCell0D, 100.0);
    vector<unsigned int> prev(solid.NumCell0D, -1);
    vector<bool> visited(solid.NumCell0D, false);

    unsigned int v = -1;
    unsigned int u = -1; 
    double weight = 1.0;

    dist[S] = 0.0;
    priority_queue<pair<double, unsigned int>, vector<pair<double, unsigned int>>, greater<pair<double, unsigned int>>> pq;
    pq.push(make_pair(0.0, S));
    while (!pq.empty())
    {
        u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (unsigned int i = 0; i < solid.NumCell1D; ++i)
        {
            if (solid.Cell1DExtrema(0, i) == u || solid.Cell1DExtrema(1, i) == u)
            {
                if(solid.Cell1DExtrema(0, i) == u)
                {
                    v = solid.Cell1DExtrema(1, i);
                }
                else
                {
                    v = solid.Cell1DExtrema(0, i);
                }

                weight = find_distance(solid, u, v);

                if (dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }
    }

    unsigned int id = -1;
    double path_length = 0.0;
    vector<unsigned int> vertices_path;

    for (unsigned int i = V; i != -1; i = prev[i])
    {
        vertices_path.push_back(i);
        solid.vertices_short_path[i] = 1.0;
    }

    for (unsigned int i = 0; i < vertices_path.size() - 1; ++i)
    {
        unsigned int P = vertices_path[i];
        unsigned int Q = vertices_path[i + 1];
        id = find_edge(P, Q, solid);
        path_length += find_distance(solid, P, Q);
        solid.edges_short_path[id] = 1.0;
    }

    cout << "Path length: " << vertices_path.size() - 1 << endl;
    cout << "Total distance: " << path_length << endl;

    return;
}

GeodeticSolid dualize(GeodeticSolid& s)
{
    GeodeticSolid solid;

    solid.NumCell0D = s.NumCell2D;
    solid.NumCell1D = s.NumCell1D;
    solid.NumCell2D = s.NumCell0D;

    solid.Cell0DId.resize(solid.NumCell0D);
    solid.Cell1DId.resize(solid.NumCell1D);
    solid.Cell2DId.resize(solid.NumCell2D);
    solid.Cell0DCoordinates.resize(3, solid.NumCell0D);
    solid.Cell1DExtrema.resize(2, solid.NumCell1D);
    solid.Cell2DVertices.resize(solid.NumCell2D);
    solid.Cell2DEdges.resize(solid.NumCell2D);
    solid.Cell2DNumVertices.resize(solid.NumCell2D);
    solid.Cell2DNumEdges.resize(solid.NumCell2D);

    unsigned int P, Q, R;
    unsigned int ctr1D = 0;
    unsigned int edge_id;
    vector<unsigned int> v = {};
    vector<unsigned int> w = {};

    for (unsigned int i = 0; i < solid.NumCell0D; ++i)
    {
        solid.Cell0DId[i] = i;
        P = s.Cell2DVertices[i][0];
        Q = s.Cell2DVertices[i][1];
        R = s.Cell2DVertices[i][2];

        for (unsigned int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(j, i) = (s.Cell0DCoordinates(j, P) +
                                            s.Cell0DCoordinates(j, Q) +
                                            s.Cell0DCoordinates(j, R)) / 3.0;
        }
    }

    
    for (unsigned int i = 0; i < s.NumCell2D; ++i)
    {
        for (unsigned int j = i + 1; j < s.NumCell2D; ++j)
        {
            if (s.Cell2DEdges[i][0] == s.Cell2DEdges[j][0] ||
                s.Cell2DEdges[i][0] == s.Cell2DEdges[j][1] ||
                s.Cell2DEdges[i][0] == s.Cell2DEdges[j][2] ||
                s.Cell2DEdges[i][1] == s.Cell2DEdges[j][0] ||
                s.Cell2DEdges[i][1] == s.Cell2DEdges[j][1] ||
                s.Cell2DEdges[i][1] == s.Cell2DEdges[j][2] ||
                s.Cell2DEdges[i][2] == s.Cell2DEdges[j][0] ||
                s.Cell2DEdges[i][2] == s.Cell2DEdges[j][1] ||
                s.Cell2DEdges[i][2] == s.Cell2DEdges[j][2])
            {
                solid.Cell1DId[ctr1D] = ctr1D;
                solid.Cell1DExtrema(0, ctr1D) = i;
                solid.Cell1DExtrema(1, ctr1D) = j;
                ctr1D++;
            }
        }
    }

    unsigned int N = 0;
    unsigned int current_edge, current_face = -1;
    Q = 0;
    R = 0;

    for (unsigned int i = 0; i < s.NumCell0D; ++i)
    {
        N = 0;
        for (unsigned int k = 0; k < s.NumCell2D; ++k)
        {
            if (s.Cell2DVertices[k][0] == i || 
                s.Cell2DVertices[k][1] == i || 
                s.Cell2DVertices[k][2] == i)
            {
                N++;
            }
        }

        for (unsigned int j = 0; j < s.NumCell1D; ++j)       
        {
            if (s.Cell1DExtrema(0, j) == i || s.Cell1DExtrema(1, j) == i)
            {
                current_edge = j;
                if (s.Cell1DExtrema(0, j) == i)
                {
                    Q = s.Cell1DExtrema(1, j);
                }        
                else 
                {
                    Q = s.Cell1DExtrema(0, j);
                }
                break;
            }
        }            
        for (unsigned int k = 0; k < s.NumCell2D; ++k)
        {
            if (s.Cell2DEdges[k][0] == current_edge ||
                s.Cell2DEdges[k][1] == current_edge || 
                s.Cell2DEdges[k][2] == current_edge)
            {  
                current_face = k;
                v.push_back(current_face);   
                break;          
            }
        }
    
    
        for(unsigned int j = 1; j < N; ++j)
        {
            for (unsigned int h = 0; h < 3; ++h)
            {
                if (s.Cell2DVertices[current_face][h] != i && 
                    s.Cell2DVertices[current_face][h] != Q)
                {
                    R = s.Cell2DVertices[current_face][h];
                    break;
                }
            }
            current_edge = find_edge(i, R, s);
            Q = R;

            for(unsigned int k = 0; k < s.NumCell2D; ++k)
            {
                if((s.Cell2DEdges[k][0] == current_edge || 
                    s.Cell2DEdges[k][1] == current_edge || 
                    s.Cell2DEdges[k][2] == current_edge) && 
                    k != current_face)
                {    
                    current_face = k;
                    v.push_back(current_face);
                    break;             
                }
            }
        }

        solid.Cell2DId[i] = i;
        solid.Cell2DVertices[i].clear();
        solid.Cell2DEdges[i].clear();
        solid.Cell2DNumVertices[i] = v.size();
        solid.Cell2DNumEdges[i] = v.size();
        solid.Cell2DVertices[i] = v;
        for (unsigned int k = 0; k < v.size(); ++k)
        {
            unsigned int edge_id = find_edge(v[k], v[(k + 1) % v.size()], solid);
            solid.Cell2DEdges[i].push_back(edge_id);
        }
        v.clear();
    }
    
    GeodeticSolid_projection(solid);
    fill_Cell3D(solid);

    solid.vertices_short_path.resize(solid.NumCell0D);
    solid.edges_short_path.resize(solid.NumCell1D);
    
    return solid;
}

GeodeticSolid build_geodetic_polyhedron_c1(unsigned int p, unsigned int q, unsigned int b, unsigned int c)
{
    GeodeticSolid solid;
    if (q != 3 && q != 4 && q != 5)
    {
        cerr << "Polyhedron not supported" << endl;
        return solid;
    }
    if ((b == 0 && c == 0) || (b != 0 && c != 0))
    {
        cerr << "Triangulation not supported" << endl;
        return solid;
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
    else if (q == 4)
    {
        solid = build_octahedron();
    }
    else if (q == 5)
    {
        solid = build_icosahedron();
    }

    triangulation_c1(b, q, solid); 
    GeodeticSolid_projection(solid); 
    print_GeodeticSolid(solid);

    cout << "Polyhedron created" << endl;
    return solid;    
}

GeodeticSolid build_geodetic_polyhedron_c2(unsigned int p, unsigned int q, unsigned int b, unsigned int c)
{
    GeodeticSolid solid;
    if (q != 3 && q != 4 && q != 5)
    {
        cerr << "Polyhedron not supported" << endl;
        return solid;
    }
    if (b != c)
    {
        cerr << "Triangulation not supported" << endl;
        return solid;
    }
    if (q == 3) 
    {
        solid = build_tetrahedron();
    }
    else if (q == 4)
    {
        solid = build_octahedron();
    }
    else if (q == 5)
    {
        solid = build_icosahedron();
    }

    triangulation_c2(b, q, solid); 
    GeodeticSolid_projection(solid); 
    print_GeodeticSolid(solid);

    cout << "Polyhedron created" << endl;
    return solid;    
}

GeodeticSolid build_goldberg_polyhedron_c1(unsigned int p, unsigned int q, unsigned int b, unsigned int c)
{
    GeodeticSolid geodetic_solid;
    GeodeticSolid goldberg_solid;

    if (p != 3 && p != 4 && p != 5)
    {
        cerr << "Polyhedron not supported" << endl;
        return goldberg_solid;
    }

    unsigned int p_new = q;
    unsigned int q_new = p;

    geodetic_solid = build_geodetic_polyhedron_c1(p_new, q_new, b, c);
    goldberg_solid = dualize(geodetic_solid);
    print_GeodeticSolid(goldberg_solid);

    cout << "Polyhedron created" << endl;
    return goldberg_solid;
}

GeodeticSolid build_goldberg_polyhedron_c2(unsigned int p, unsigned int q, unsigned int b, unsigned int c)
{
    GeodeticSolid geodetic_solid;
    GeodeticSolid goldberg_solid;

    if (p != 3 && p != 4 && p != 5)
    {
        cerr << "Polyhedron not supported" << endl;
        return goldberg_solid;
    }

    unsigned int p_new = q;
    unsigned int q_new = p;

    geodetic_solid = build_geodetic_polyhedron_c2(p_new, q_new, b, c);
    goldberg_solid = dualize(geodetic_solid);
    print_GeodeticSolid(goldberg_solid);

    cout << "Polyhedron created" << endl;
    return goldberg_solid;
}

void build_UCD(GeodeticSolid& solid)
{
    bool flag = false;
    unsigned int ctr = 0;
    while (!flag && ctr < solid.NumCell0D)
    {
        if (solid.vertices_short_path[ctr] == 1.0)
        {
            flag = true;
        }
        else
        {
            ctr++;
        }
    }

    if (!flag) // If user didn't ask for a path we export without calling vertices and edges short paths
    {
        Gedim::UCDUtilities utilities;
        {
            utilities.ExportPoints("./Cell0D.inp", solid.Cell0DCoordinates);
            utilities.ExportSegments("./Cell1D.inp", solid.Cell0DCoordinates, solid.Cell1DExtrema);
        }
    }
    else
    {
        Gedim::UCDProperty<double> vertices_property;
	    vertices_property.NumComponents = 1;
	    const double* ptr1 = solid.vertices_short_path.data();
	    vertices_property.Data = ptr1;
	    vertices_property.Label = "Visited Nodes";
	    vector<Gedim::UCDProperty<double>> vertices_properties_UCD = { vertices_property };

        Gedim::UCDProperty<double> edges_property;
        edges_property.NumComponents = 1;
        const double* ptr2 = solid.edges_short_path.data();
        edges_property.Data = ptr2;
        edges_property.Label = "Visited Edges";
        vector<Gedim::UCDProperty<double>> edges_properties_UCD = { edges_property };

        Gedim::UCDUtilities utilities;
        {
            utilities.ExportPoints("./Cell0D.inp", solid.Cell0DCoordinates, vertices_properties_UCD);
            utilities.ExportSegments("./Cell1D.inp", solid.Cell0DCoordinates, solid.Cell1DExtrema, vertices_properties_UCD, edges_properties_UCD);
        }
    }

}

void main_function(vector<unsigned int>& input)
{

    // p, q, triangolazione, b, c, v1, v2
    GeodeticSolid solid;
    unsigned int size = input.size();
    bool flag = true;

    switch(size)
    {
        case 5:
            if(input[0] == 3)
            {
                if(input[2] == 1)
                    solid = build_geodetic_polyhedron_c1(input[0], input[1], input[3], input[4]);
                else if(input[2] == 2)
                    solid = build_geodetic_polyhedron_c2(input[0], input[1], input[3], input[4]);
                else
                    cout << "Triangulation type not valid";
            }

            if (input[1] == 3 && input[0] != 3)
            {
                if(input[2] == 1)
                    solid = build_goldberg_polyhedron_c1(input[0], input[1], input[3], input[4]);
                else if(input[2] == 2)
                    solid = build_goldberg_polyhedron_c2(input[0], input[1], input[3], input[4]);
                else
                    cout << "Triangulation type not valid";
            }
            break;
        case 7:
            if(input[0] == 3)           
            {
                if(input[2] == 1)
                    solid = build_geodetic_polyhedron_c1(input[0], input[1], input[3], input[4]);
                else if(input[2] == 2)
                    solid = build_geodetic_polyhedron_c2(input[0], input[1], input[3], input[4]);
                else
                    cout << "Triangulation type not valid";
            }

            if (input[1] == 3 && input[0] != 3)
            {
                if(input[2] == 1)
                    solid = build_goldberg_polyhedron_c1(input[0], input[1], input[3], input[4]);
                else if(input[2] == 2)
                    solid = build_goldberg_polyhedron_c2(input[0], input[1], input[3], input[4]);
                else
                    cout << "Triangulation type not valid";
            }
            shortest_path(input[5], input[6], solid);
            break;
        default:
            cout << "Input not valid" << endl;
            flag = false;
            
    }

    if(flag)
    {
        build_UCD(solid);
    }

    check_ordination(solid);
    

}

}// namespace GeodeticLibrary