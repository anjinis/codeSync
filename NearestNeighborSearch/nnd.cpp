#include "nnd.h"

#define INITIAL_NO_DIRECTION 0
#define TWO_DIMENSION 2

#define	LEFT	0
#define RIGHT	1

#define X_CORDINATE	0
#define Y_CORDINATE	1


/************************************************************************************
 c'tor
 
***********************************************************************************/
NearestNeighborDirectory::NearestNeighborDirectory( )
{
	CreateTree( );
}

/************************************************************************************
 CalculateSquaredDist
 dis = (x1-x2)^2 + (y1-y2)^2
 
 *************************************************************************************/
double NearestNeighborDirectory::CalculateSquaredDist( double* point1, double* point2 )
{
	double squareDistance = 0;
	
	squareDistance += ( point1[0] - point2[0] ) * ( point1[0] - point2[0] );
	squareDistance += ( point1[1] - point2[1] ) * ( point1[1] - point2[1] );
	
	return squareDistance;
}

/************************************************************************************
  CreateTree
 
***********************************************************************************/
void NearestNeighborDirectory::CreateTree( )
{
	m_tree = new( kdtree );
	
	if( m_tree == NULL )
	{
		return;
	}
	m_tree->dim = TWO_DIMENSION;
	m_tree->root = NULL;
	m_tree->destr = NULL;
}

/****************************************************************************************
 PrintInputOrdered
 
 *****************************************************************************************/
void NearestNeighborDirectory::PrintInputOrdered( )
{
	while( inputOrderedQueue.empty( ) == false )
	{
		std::cout.precision(17);
		
		Node* currentNodePtr = inputOrderedQueue.front( );
		std::cout<<currentNodePtr->GetName()<<" ";
		int count = 0;
		
		for( NearNeighborList::iterator it = currentNodePtr->m_NearNeighborList.begin( ); 
			it< currentNodePtr->m_NearNeighborList.end( ); 
			++it )
		{
			std::cout<<(*it).Neighbor->GetName( );
			
			if( count < 2 )
			{
				std::cout<<",";
			}
			count++;
		}
		std::cout<<"\n";
		inputOrderedQueue.pop( );
	}
}


/****************************************************************************************
   insert_rec
 
*****************************************************************************************/
int NearestNeighborDirectory::insert_rec( Node*& currentNodePtr, const double *pos, std::string name, int splitPlane )
{	
	if( currentNodePtr == NULL  )
	{
		currentNodePtr = new( Node );
		
		currentNodePtr->UpdateNodeInfo( pos, name, splitPlane);
	
		inputOrderedQueue.push( currentNodePtr );
		
		return 0;
	}
	
	Node* node = currentNodePtr;
	
	int newSplitPlane = node->GetNewSplitPlane( );
	
	int currentSplitPlane = node->GetCurrentSplitPlane( );
	
	if( pos[currentSplitPlane] < node->m_Location[currentSplitPlane] )
	{
		return insert_rec( currentNodePtr->m_left, pos, name, newSplitPlane );
	}
	else
	{
		return insert_rec( currentNodePtr->m_right, pos, name, newSplitPlane );
	}
}


/************************************************************************************
 Insert
 
*************************************************************************************/
void NearestNeighborDirectory::Insert( double x, double y, std::string name )
{
	double locationBuffer[2];
	locationBuffer[0] = x;
	locationBuffer[1] = y;
	
	insert_rec( m_tree->root, locationBuffer, name, INITIAL_NO_DIRECTION );
}


/************************************************************************************
 IterativePrint
 
 *************************************************************************************/
void NearestNeighborDirectory::IterativeSolve( )
{	
	std::vector<Node* > stackOfTreeNodes;
	
	stackOfTreeNodes.push_back( m_tree->root );
	
	Node* currentNodePtr;
	
	while( stackOfTreeNodes.empty( ) == false )
	{
		currentNodePtr = stackOfTreeNodes.back( );
		stackOfTreeNodes.pop_back( );
	
		if( currentNodePtr->m_right != NULL )
		{
			stackOfTreeNodes.push_back( currentNodePtr->m_right );
		}

		if( currentNodePtr->m_left != NULL )
		{
			stackOfTreeNodes.push_back( currentNodePtr->m_left );
		}
		
		FindNearest( m_tree->root, currentNodePtr );
	}
}


/************************************************************************************
 FindNearest
 
 *************************************************************************************/
void NearestNeighborDirectory::FindNearest( Node* currentNodePtr, Node* toComputeNNDFor )
{
	if( currentNodePtr == NULL )
	{
		return;
	}
	
	int currentSplittingPlane = currentNodePtr->GetCurrentSplitPlane( );

	if( toComputeNNDFor->GetLocation( currentSplittingPlane ) < currentNodePtr->GetLocation( currentSplittingPlane ) )
	{
		if( currentNodePtr->m_left != NULL )
		{
			FindNearest( currentNodePtr->m_left, toComputeNNDFor );
		}
		else if( currentNodePtr->m_right != NULL )
		{
			FindNearest( currentNodePtr->m_right, toComputeNNDFor );
		}
	}
	else 
	{
		if( currentNodePtr->m_right != NULL )
		{
			FindNearest( currentNodePtr->m_right, toComputeNNDFor );
		}
		else if( currentNodePtr->m_left != NULL )
		{
			FindNearest( currentNodePtr->m_left, toComputeNNDFor );
		}
	}
	
	bool shouldCheckOtherBranch;
	
	double* currentWorstLocPtr = toComputeNNDFor->GetCurrentWorstLoc( );
	
	if( currentWorstLocPtr == NULL || toComputeNNDFor->m_NearNeighborList.size( ) < 3 )
	{
		shouldCheckOtherBranch = true;
	}
	else 
	{
		int sp = toComputeNNDFor->GetCurrentSplitPlane( );
		
		double x1 = toComputeNNDFor->GetLocation(sp);
		double x2 = currentNodePtr->GetLocation(sp);
		double xworst = toComputeNNDFor->GetCurrentWorstLoc( )[ sp ];
		
		int sp_other = ( sp + 1 ) % 2;
		
		double y1 = toComputeNNDFor->GetLocation(sp_other);
		double yworst = toComputeNNDFor->GetCurrentWorstLoc( )[ sp_other ];
	
		shouldCheckOtherBranch = fabs(x1 - x2) < ( fabs(x1 - xworst) + fabs(y1 -yworst) );
		
		if( currentNodePtr == m_tree->root )
		{
			shouldCheckOtherBranch = true;
		}
	}
						
	
	if( shouldCheckOtherBranch )
	{
		int otherSide;
		int persplit = currentNodePtr->GetCurrentSplitPlane( );

		int previousDir = toComputeNNDFor->GetLocation( persplit ) < currentNodePtr->GetLocation( persplit );
		
		
		if( previousDir /* LEFT*/ )
		{
			if( currentNodePtr->m_left != NULL )
			{
				otherSide = RIGHT;
			}
			else if( currentNodePtr->m_right != NULL )
			{
				otherSide = LEFT;
			}
		}
		else
		{
			if( currentNodePtr->m_right != NULL )
			{
				otherSide = LEFT;
			}
			else if( currentNodePtr->m_left != NULL )
			{
				otherSide = RIGHT;
			}
		}
		
		if( otherSide == LEFT  )
		{
			if( currentNodePtr->m_left != NULL )
			{
				FindNearest( currentNodePtr->m_left, toComputeNNDFor );
			}
		}
		else if( otherSide == RIGHT )
		{
			if( currentNodePtr->m_right != NULL )
			{
				FindNearest( currentNodePtr->m_right, toComputeNNDFor );
			}
		}

	}
	double thisSquaredDist = CalculateSquaredDist( toComputeNNDFor->GetLocationPtr(), currentNodePtr->GetLocationPtr() );
	
	if( thisSquaredDist != 0)
	{
		toComputeNNDFor->SortedInsert(currentNodePtr, thisSquaredDist );		
	}
	
}




