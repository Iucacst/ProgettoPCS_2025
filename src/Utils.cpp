#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace GeodeticLibrary
{

void build_polygon_class_1(unsigned int p, unsigned int q, unsigned int b, unsigned int c)
{
    GeodeticSolid solid;

    if((b == 0 && c == 0 || b != 0 && c != 0) && p != 3)
    {
        cerr << "Il poligono non appartiene alla classe 1." << endl;
    }

    if (q > 5 || q < 3)
    {
        cerr << "Il poligro non appartiene alla classe 1." << endl;
    }
    {
        cerr << "Non è possibile processare il poliedro" << endl;
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

    solid.Cell0DId.resize(solid.NumCell0D);
    solid.Cell0DCoordinates = Eigen::MatrixXd::Zero(4, solid.NumCell0D);
    solid.Cell1DExtrema = Eigen::MatrixXi::Zero(3, solid.NumCell1D);

    solid.Cell2DVertices.resize(4);
    solid.Cell2DEdges.resize(4);

    Eigen::Matrix<double, 4, 3> vertices;
    vertices <<  1,  1,  1,
                -1, -1,  1,
                -1,  1, -1,
                 1, -1, -1;

    for (int i = 0; i < solid.NumCell0D; ++i)
    {
        solid.Cell0DId.push_back(i);
        solid.Cell0DCoordinates(i, 0) = solid.Cell0DId[i];
        for (int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(i, j + 1) = vertices(i, j)/sqrt(3);
        }
    }
    for (int i = 0; i < solid.NumCell1D; ++i)
    {
        solid.Cell1DId.push_back(i);
        solid.Cell1DExtrema(i, 0) = solid.Cell1DId[i];
        
        for (int j = i+1; j <= 3; ++j)
        {
            solid.Cell1DExtrema(j + i - 1, 1) = solid.Cell0DId[i];
            solid.Cell1DExtrema(j + i - 1, 2) = solid.Cell0DId[j];
        }
    }
    for (int i = 0; i < solid.NumCell2D; ++i)
    {
        int v = 0;

        solid.Cell2DId.push_back(i);
        solid.Cell2DNumVertices.push_back(3);
        solid.Cell2DNumEdges.push_back(3);
        
        solid.Cell2DEdges[i].push_back(i);
        solid.Cell2DVertices[i].push_back(i);

        for (int j = 0; j < 3; ++j)
        {
            v = (j + 2) % 4;
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
    
}

// ***************************************************************************
// ***************************************************************************
// ***************************************************************************
// ***************************************************************************
// ***************************************************************************
// ***************************************************************************

/*
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
		string field_value;
		vector<string> field_values;
		
		while (getline(converter, field_value, ';'))
			field_values.push_back(field_value);
		
		id = stoi(field_values[0]);
		marker = stoi(field_values[1]);
		mesh.Cell0DsCoordinates(0, id) = stod(field_values[2]);
		mesh.Cell0DsCoordinates(1, id) = stod(field_values[3]);
		
        //converter >> id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);
        mesh.Cell0DsId.push_back(id);
       /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell0Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }

    }
    return true;
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
		string field_value;
		vector<string> field_values;
		
		while (getline(converter, field_value, ';'))
			field_values.push_back(field_value);
		
		id = stoi(field_values[0]);
		marker = stoi(field_values[1]);
		mesh.Cell1DsExtrema(0, id) = stoi(field_values[2]);
		mesh.Cell1DsExtrema(1, id) = stoi(field_values[3]);
        /*converter >> id >> marker >>  mesh.Cell1DsExtrema(0, id) >>  mesh.Cell1DsExtrema(1, id);
		cout << marker;
        mesh.Cell1DsId.push_back(id);

        /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell1Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
    }

    return true;
}
// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string field_value;
        vector<string> field_values;

        unsigned int id, marker, numVertixes, numEdges;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

		while (getline(converter, field_value, ';')){
			field_values.push_back(field_value);
			
		}
		
        id = stoi(field_values[0]);
        marker = stoi(field_values[1]);
        numVertixes = stoi(field_values[2]);
        
        for(unsigned int i = 3; i < 3 + numVertixes; i++)
            vertices.push_back(stoi(field_values[i]));
        
        numEdges = stoi(field_values[3 + numVertixes]);
        
        for(unsigned int i = 3 + numVertixes; i <  3 + numVertixes + numEdges; i++)
            edges.push_back(stoi(field_values[i]));

        mesh.Cell2DsId.push_back(id);
        
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell2Ds.find(marker);
            if(it == mesh.MarkerCell2Ds.end())
            {
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell1Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
        }
	file.close();
	return true;
}

bool edges_test(PolygonalMesh& mesh)
{
	for(unsigned int i = 0; i<mesh.NumCell2Ds; i++)
	{
        /*con questi for, prendo ogni poligono (i), e per ogni poligono prendo ogni vertice esistente 
        appartenente al poligono (j)
		for(unsigned int j = 0; j<mesh.Cell2DsEdges[i].size();j++)
		{	
			
			vector<unsigned int>& edges = mesh.Cell2DsEdges[i];
			
            //prendo le coordinate dei punti estremi dell' j-esimo edge
			int& Origin_index = mesh.Cell1DsExtrema(0,edges[j]);
			int& End_index = mesh.Cell1DsExtrema(1,edges[j]);
			
			//mi ricavo le coordinate dei punti estremi del segmento, e faccio il teorema di pitagora 
            //per ricavarne la lunghezza
			double& X_Origin = mesh.Cell0DsCoordinates(0,Origin_index);
			double& Y_Origin = mesh.Cell0DsCoordinates(1,Origin_index);
			double& X_End = mesh.Cell0DsCoordinates(0,End_index);
			double& Y_End = mesh.Cell0DsCoordinates(1,End_index);	
			
			double distance = sqrt(pow(X_Origin-X_End,2)+pow(Y_Origin-Y_End,2));
			
			if(distance < 1e-16)
			{
				cout<<"il poligono con ID "<< i <<" presenta il vertice con ID "<<edges[j]<<" che è di lunghezza 0"<<endl;
				return false;
			}
		}
	}
	cout<<"nessun edge ha lunghezza 0 "<<endl;
	return true;
}


bool areas_test(PolygonalMesh& mesh)
{
	
	for(unsigned int i = 0; i<mesh.NumCell2Ds; i++)
	{
		double area = 0.0;
		unsigned int n = mesh.Cell2DsVertices[i].size();
		for(unsigned int j = 0; j < n; j++)
		{
			/*per far sì che la formula per il calcolo dell'area sia funzionante, l'ultimo vertice deve 
            chiudersi con il primo e per fare ciò utilizzo le operazioni algebriche modulo n, in modo 
            tale che raggiunto l'inidce n, l'indice j ritorni direttamente a 0
			unsigned int& P1_id = mesh.Cell2DsVertices[i][j];
			unsigned int& P2_id = mesh.Cell2DsVertices[i][(j+1)%n];
			
			double& X_P1 = mesh.Cell0DsCoordinates(0,P1_id);
			double& Y_P1 = mesh.Cell0DsCoordinates(1,P1_id);
			double& X_P2 = mesh.Cell0DsCoordinates(0,P2_id);
			double& Y_P2 = mesh.Cell0DsCoordinates(1,P2_id);
			
			area += X_P1*Y_P2-X_P2*Y_P1;
		}
		
		area = 0.5*abs(area);
		
		if(area < 1e-12)
		{
			cout<<"C'è un errore nel poligono che ha ID "<<i<<", ha area pari a 0."<<endl;
			return false;
		}
	}
	cout<<"Non ci sono poligoni che hanno area pari a 0."<<endl;
	return true;
}
}*/
