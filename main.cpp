#include <iostream>
#include <vector>
#include <string>
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace GeodeticLibrary;

int main()
{
    vector<unsigned int> input;
    unsigned int i = 0;
    string str;
    bool flag = true;

    while(flag)
    {
        input.clear();
        system("clear");

        cout << "Value for p: ";
        cin >> i;
        if(cin.fail())
        {
            cout << "Input type not valid" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }              
        input.push_back(i);


        cout << "Value for q: ";
        cin >> i;
        if(cin.fail())
        {
            cout << "Input type not valid" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        input.push_back(i);

        cout << "Triangulation type (1 or 2): ";
        cin >> i;
        if(cin.fail())
        {
            cout << "Input type not valid" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        input.push_back(i);

        cout << "Value for triangulation parameter: ";   
        cin >> i;    
        if(cin.fail())
        {
            cout << "Input type not valid" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        input.push_back(i);

        if(input[2] == 1)
        {
            input.push_back(0);
        }
        else if(input[2] == 2)
        {
            input.push_back(i);
        }

        cout << "Do you want to find the shortest path between two vertices? (YES or NO) ";
        cin >> str;
        if(cin.fail())
        {
            cout << "Input type not valid";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if(str == "YES")
        {
            cout << "Value for vertex 1: ";
            cin >> i;
            if(cin.fail())
            {
                cout << "Input type not valid" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            input.push_back(i);
        
            cout << "Value for vertex 2: ";
            cin >> i;
            if(cin.fail())
            {
                cout << "Input type not valid" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            input.push_back(i);
        }
        flag = false;
    }
    
    main_function(input);

    

	
	
    return 0;
}