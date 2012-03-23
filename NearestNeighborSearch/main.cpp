/***************************************************
 Author: Anjini Shukla
 June 2011
 All code Anjini Shukla, all rights reserved
 
 **************************************************/
#include <iostream>
#include <fstream>
#include "nnd.h"
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

int main (int argc, char * const argv[])
{
	//std::ifstream myfile ("md5Stylecheck.txt");
	std::ifstream myfile (argv[1]);
	std::string line;

	NearestNeighborDirectory smallWorldSolver;
	
	if (myfile.is_open())
	{
		while ( myfile.good( ) )
		{	
			getline ( myfile, line );
			
			if( line.empty( ) == false )
			{
				std::string buf; // Have a buffer string
				std::stringstream ss(line); // Insert the string into a stream
				
				std::vector<std::string> tokens; // Create vector to hold our words
				
				while( ss >> buf )
				{
					tokens.push_back(buf);
				}
				
				assert( tokens.size( ) == 3 );
			
				std::string logitude = tokens.back( );
				tokens.pop_back( );
				
				std::string latitude = tokens.back( );
				tokens.pop_back( );
				
				std::string uniqueId = tokens.back( );
				tokens.pop_back( );
				
				double xcord = strtod(latitude.c_str(), NULL );
				double ycord = strtod(logitude.c_str(), NULL);
				
				smallWorldSolver.Insert(xcord, ycord, uniqueId );
				
			}
		}
		myfile.close();
	}
	
	else 
	{	
		std::cout << "Unable to open file";
	}
	
	smallWorldSolver.IterativeSolve( );
	
	smallWorldSolver.PrintInputOrdered( );
	
    return 0;
}
