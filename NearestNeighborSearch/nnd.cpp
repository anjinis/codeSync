./._nnd.h                                                                                           000644  000765  000024  00000000253 11606177231 014207  0                                                                                                    ustar 00anjinishukla                    staff                           000000  000000                                                                                                                                                                             Mac OS X            	   2   y      �                                      ATTR�ї   �   �                     �     com.apple.TextEncoding   utf-8;134217984                                                                                                                                                                                                                                                                                                                                                     nnd.h                                                                                               000644  000765  000024  00000010734 11606177231 013642  0                                                                                                    ustar 00anjinishukla                    staff                           000000  000000                                                                                                                                                                         #include<iostream>

/***************************************************
Author: Anjini Shukla
June 2011
All code Anjini Shukla, all rights reserved

**************************************************/
#include<memory>
#include<vector>
#include<assert.h>
#include<math.h>
#include<queue>
#include<list>

//forward delcare
class Node;

struct NearNeighborNode
{
	Node* Neighbor;
	double distanceFromYou;
};

typedef std::vector<NearNeighborNode > NearNeighborList;


/******************************************************************************
 Node
 
 *****************************************************************************/
class Node
{
public:
	Node( )
	: m_left( NULL ),
	  m_right( NULL ),
	  leftCheck( false ),
	  rightCheck( false )
	{
	}
	
	double GetLocation( int dimension )
	{
		return m_Location[dimension];
	}
	double* GetLocationPtr( )
	{
		return m_Location; 
	}
	
	double* GetCurrentBestLocation( )
	{
		if( m_NearNeighborList.empty( ) )
		{
			return NULL;
		}
		return m_NearNeighborList.front( ).Neighbor->m_Location;
	}
	
	double* GetCurrentWorstLoc( )
	{
		if( m_NearNeighborList.empty( ) )
		{
			return NULL;
		}
		return m_NearNeighborList.back( ).Neighbor->m_Location;
	}
	
	std::string GetName( )
	{
		return m_Name;
	}
	
	NearNeighborList GetNearNeighborList( )
	{
		return m_NearNeighborList;
	}
		
	int GetNewSplitPlane( )
	{	
		return ( m_splitPlane + 1 ) % 2; //TWO_DIMENSION
	}
	
	int GetCurrentSplitPlane( )
	{ 
		assert( m_splitPlane == 0 || m_splitPlane == 1 );
		return m_splitPlane; 
	}

	void SetLocation( const double* location )
	{
		//assert( m_Location == NULL );
		m_Location = new( double );
		m_Location[0] = location[0];
		m_Location[1] = location[1];
	}
	
	void UpdateNodeInfo( const double* location, std::string name, int splitPlane )
	{
		assert( splitPlane == 0 || splitPlane == 1 );
		
		//m_Location = NULL;
		SetLocation( location );
		m_Name = name;
		m_splitPlane = splitPlane;
		
		assert( m_splitPlane == 0 || m_splitPlane == 1 );
	}
	
	
	//assumption no two nodes will have same name
	void SortedInsert( Node* neighborNodePtr, double thisSqDist )
	{
		//TBD assert neighborNodePtr is not null
		
		//initialize
		NearNeighborNode nodeToInsert;
		nodeToInsert.Neighbor = neighborNodePtr;
		nodeToInsert.distanceFromYou = thisSqDist;
		
		if( m_NearNeighborList.empty( ) == true )
		{
			m_NearNeighborList.push_back( nodeToInsert );
			return;
		}
		
		//get the correct location
		NearNeighborList temp;
		
		while( m_NearNeighborList.empty( ) == false )
		{
			if( m_NearNeighborList.back( ).distanceFromYou > nodeToInsert.distanceFromYou )
			{
				temp.push_back( m_NearNeighborList.back( ) );
				m_NearNeighborList.pop_back( );
			}
			else if( m_NearNeighborList.back( ).distanceFromYou == nodeToInsert.distanceFromYou )
			{
				//it is not the same node
				if( m_NearNeighborList.back( ).Neighbor->GetName( ) != nodeToInsert.Neighbor->GetName( ) )
				{ 
					temp.push_back( m_NearNeighborList.back( ) );
					m_NearNeighborList.pop_back( );
				}
			}
			else
			{
				break;
			}
		}
		
		if( m_NearNeighborList.size( ) < 3 )
		{
			m_NearNeighborList.push_back( nodeToInsert );
		}
		
		//finally insert
		while( m_NearNeighborList.size(  ) <= 3 && !temp.empty( ) )
		{
			if( m_NearNeighborList.size(  ) == 3 )
			{
				break;
			}
			m_NearNeighborList.push_back( temp.back( ) );
			temp.pop_back( );
		}
		
	}
		
	double*						m_Location;
	int							m_splitPlane;
	std::string					m_Name;
	Node*						m_left;
	Node*						m_right;
	bool						leftCheck;
	bool						rightCheck;

	NearNeighborList			m_NearNeighborList;
};

/******************************************************************************
 kdtree
 
 *****************************************************************************/
struct kdtree {
	int dim;
	Node *root;
	void (*destr)(void*);
};

/******************************************************************************
   NearestNeighborDirectory
 
 *****************************************************************************/
class NearestNeighborDirectory
{
public:
	NearestNeighborDirectory( );
	
	void CreateTree( );
	void Insert( double x, double y, std::string name );

	void PrintInputOrdered( );
	void PrintList( );
	
	void IterativeSolve( );

	double CalculateSquaredDist( double* point1, double* point2 );
	
	void FindNearest( Node* currentNodePtr, Node* toComputeNNDFor );
	
	int insert_rec( struct Node*& currentNodePtr, const double *pos, std::string name, int direction );
	void insertInList( double x, double y, std::string name );
	
	
	int GetDimension( )
	{
		if( m_tree != NULL )
		{
			return m_tree->dim;
		}
		return 0;
	}
	
	struct kdtree* m_tree;
	
	std::queue<Node* > inputOrderedQueue;
	std::list<Node * > inputOrderedList;
};
                                    ./._main.cpp                                                                                        000644  000765  000024  00000000253 11606430750 014705  0                                                                                                    ustar 00anjinishukla                    staff                           000000  000000                                                                                                                                                                             Mac OS X            	   2   y      �                                      ATTR�і   �   �                     �     com.apple.TextEncoding   utf-8;134217984                                                                                                                                                                                                                                                                                                                                                     main.cpp                                                                                            000644  000765  000024  00000002663 11606430750 014342  0                                                                                                    ustar 00anjinishukla                    staff                           000000  000000                                                                                                                                                                         /***************************************************
 Author: Anjini Shukla
 April 2011
 
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
                                                                             ./._Makefile                                                                                        000644  000765  000024  00000001012 11561074161 014707  0                                                                                                    ustar 00anjinishukla                    staff                           000000  000000                                                                                                                                                                             Mac OS X            	   2  �     
                                      ATTR�љ  
   �  B                  �   �  %com.apple.metadata:kMDItemWhereFroms   �   X  com.apple.quarantine bplist00�_zhttps://mail.google.com/mail/?ui=2&ik=4b8509b08c&view=att&th=12fc6ed411a3de78&attid=0.1&disp=attd&realattid=f_gndk4f3r1&zw_<https://mail.google.com/mail/?ui=2&view=bsp&ver=ohhl4rw8mbn4�                            �q/0000;4dc609ca;Google\x20Chrome;6CE76AE1-2532-4A28-8E24-7B1539EF3950|com.google.Chrome                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       Makefile                                                                                            000644  000765  000024  00000001007 11561074161 014341  0                                                                                                    ustar 00anjinishukla                    staff                           000000  000000                                                                                                                                                                         APP_BINARY = smallworld
APP_SOURCE = main.cpp
INCLUDES   = nnd.cpp # None. Use -Ipath to add
OPTIONS    = -O2 -std=gnu++98

CXXFLAGS   += $(INCLUDES) $(OPTIONS)
LDFLAGS    += -lm

all: $(APP_BINARY)

$(APP_BINARY): 
_SOURCES = $(APP_SOURCE)
_CPPOBJECTS = $(_SOURCES:.cpp=.o)
_EXECUTABLE=$(APP_BINARY)

$(_EXECUTABLE): $(_CPPOBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(_CPPOBJECTS) -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean realclean

clean: 
	rm -rf $(_CPPOBJECTS)

realclean: clean
	rm -rf $(_EXECUTABLE)_*
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         