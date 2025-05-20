GeodeticSolid build_icosahedron()
{
    GeodeticSolid solid;
    
    solid.NumCell0D = 12;
    solid.NumCell1D = 30;
    solid.NumCell2D = 20;

    solid.Cell0DId.reserve(solid.NumCell0D);
	solid.Cell1DId.reserve(solid.NumCell1D);
	solid.Cell2DId.reserve(solid.NumCell2D);
    
	solid.Cell0DCoordinates = Eigen::MatrixXd::Zero(3, solid.NumCell0D);
	
    solid.Cell1DExtrema = Eigen::MatrixXi::Zero(2, solid.NumCell1D);

	solid.Cell2DNumVertices.reserve(solid.NumCell2D);
	solid.Cell2DNumEdges.reserve(solid.NumCell2D);
	
    solid.Cell2DVertices.reserve(solid.NumCell2D);
    solid.Cell2DEdges.reserve(solid.NumCell2D);

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
        solid.Cell0DId.push_back(i);

        for (int j = 0; j < 3; ++j)
        {
            solid.Cell0DCoordinates(j, i) = vertices(i, j);
        }
    }

	//Cell1D
	for (unsigned int j = 0; j < 5; ++j)
	{
		solid.Cell1DId.push_back(j);
		solid.Cell1DExtrema(0, j) = solid.Cell0DId[0];
		solid.Cell1DExtrema(1, j) = solid.Cell0DId[j+1];
		
		solid.Cell1DId.push_back(j+5);
		solid.Cell1DExtrema(0, j+5) = solid.Cell0DId[11];
		solid.Cell1DExtrema(1, j+5) = solid.Cell0DId[j+6];
	}
	
	for (unsigned int j = 0; j < 5, ++j)
	{
		solid.Cell1DId.push_back(j+10);
		solid.Cell1DExtrema(0, j+10) = solid.Cell0DId[j+1];
		solid.Cell1DExtrema(1, j+10) = solid.Cell0DId[(j+2)%5];
		
		solid.Cell1DId.push_back(j+15);
		solid.Cell1DExtrema(0, j+15) = solid.Cell0DId[j+6];
		if (j == 4)
		{
			solid.Cell1DExtrema(1, j+15) = solid.Cell0DId[6];
		}
		else
		{
			solid.Cell1DExtrema(1, j+15) = solid.Cell0DId[j+7];
		}
	}
	
	for (unsigned int j = 0; j < 5; ++j)
	{
		solid.Cell1DId.push_back(j+20);
		solid.Cell1DExtrema(0, j+20) = solid.Cell0DId[j+1];
		solid.Cell1DExtrema(1, j+20) = solid.Cell0DId[j+6];
		
		solid.Cell1DId.push_back(j+25);
		if (j == 4)
		{
			solid.Cell1DExtrema(0, j+25) = solid.Cell0DId[1];
		}
		else 
		{
			solid.Cell1DExtrema(0, j+25) = solid.Cell0DId[j+2];
		}
		solid.Cell1DExtrema(1, j+25) = solid.Cell0DId[j+6];
	}


	//Cell2D
	/*
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
} */