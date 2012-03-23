/***************************************************
 Author: Anjini Shukla
 June 2011
 All code Anjini Shukla, all rights reserved
 
 **************************************************/

#include<iostream>
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
