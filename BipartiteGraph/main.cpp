/*
 *  Author anjini shukla on 7/11/11.
 *  Copyright 2011 All code Anjini Shukla, all rights reserved
 *
 */

#include "liar.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <assert.h>


/******************************************************* TEST
inputformat
4
A 2
d
c
B 1
c
C 2
a
b
D 1
a
 
output format
2 2

********************************************************/

int main ( int argc, char * const argv[ ] )
{
	try
	{
		std::ifstream myfile ( argv[1] /*input file name*/ );
		
		Bipartite puzzle_solver;
		int isInitialized = false;
	
		if( myfile.is_open() )
		{
			//get first line
			std::string totalPopulation;
			std::getline(myfile, totalPopulation );
		
			int populationCount = atoi( totalPopulation.c_str() );
		
			if( populationCount == 0 )
			{
				return 1;
			}
		
			// read totalPopulation chunks of data
			for (int i = 0; i < populationCount ; i++ )
			{
				std::string firstLineOfChunk;
				std::getline(myfile, firstLineOfChunk);
			
				std::string buf;
				std::istringstream iss(firstLineOfChunk);
			
				// Create vector to hold our words
				std::vector<std::string> tokensFirstChunk;
			
				while( iss >> buf )
				{
					tokensFirstChunk.push_back(buf);
				}
			
				assert( tokensFirstChunk.size( ) == 2 );
			
				std::string numberOfAccusedString = tokensFirstChunk.back( );
				int numberOfAccused = atoi(numberOfAccusedString.c_str());
			
				tokensFirstChunk.pop_back( );
			
				std::string accuserName = tokensFirstChunk.back( );
				tokensFirstChunk.pop_back( );
			
				std::vector<std::string> accusedList;
			
				for (int accusedCount = 0; accusedCount < numberOfAccused; ++accusedCount ) 
				{
					std::string line;
					std::getline(myfile, line);
					
					accusedList.push_back( line );
				}
			
				if( isInitialized == false )
				{
					puzzle_solver.SeedInsert( accuserName, accusedList );
					isInitialized = true;
				}
				else
				{
					puzzle_solver.Insert( accuserName, accusedList );
				}
			
			} // eo outer for
			
			puzzle_solver.FinalVisit( );
			
			if( puzzle_solver.m_visitLatterList.empty( ) != true )
			{
				std::cout<<"Application Failed to add all nodes to two buckets, possible bad input ";
				return 1;
			}
			else
			{
				puzzle_solver.PrintFormattedOutput();
			}
		}
		else 
		{	
			std::cout << "Unable to open file";
			return 1;
		}
	}
	catch ( std::exception& ex )
	{
		std::cout << "Application Failed...exception "<< ex.what()<<"\n";
		
	}
		
	return 0;
}
