/***************************************************
 Author: Anjini Shukla
 June 2011
 All code Anjini Shukla, all rights reserved
 
 **************************************************/

#include "liar.h"

/*********************************************************
 Group
 
 **********************************************************/
Group::Group( )
: m_hashTable( )
{
}

bool Group::DoesExists( std::string toFindName )
{
	hash_map<std::string, bool>::iterator it;
	
	it = m_hashTable.find( toFindName );
	
	if( it == m_hashTable.end( )  )
	{
		return false;
	}
	else 
	{
		return true;
	}
}

void Group::Insert( std::string toInsertName )
{
	m_hashTable[toInsertName] = false;
}

void Group::Insert( std::vector<std::string> toInsertVector )
{
	for ( std::vector<std::string>::iterator it = toInsertVector.begin( ); 
		 it != toInsertVector.end( ); 
		 ++it )
	{
		m_hashTable[ *it ] = false;
	}
}

void Group::Print( )
{
	for ( hash_map<std::string, bool>::iterator it = m_hashTable.begin( ); 
		 it != m_hashTable.end( ); 
		 ++it )
	{
		std::cout<< it->first<<"\t \n";
	}
}

/*********************************************************
 Bipartite
 
**********************************************************/
Bipartite::Bipartite( )
		   :m_honest( ),
		    m_liar( ),
		    m_visitLatterList( )
{
	
}

void Bipartite::SeedInsert( std::string accuser, std::vector<std::string> accused )
{
	// pick one group arbit
	m_honest.Insert( accuser );
	m_liar.Insert( accused );
}

void Bipartite::Insert( std::string accuser, std::vector<std::string> accused )
{	
	bool didSuccesfullInsert = InsertToGroup( accuser, accused );
	
	if( didSuccesfullInsert == false )
	{		
		RevisitNode* newRevisitNode = new RevisitNode( );
		newRevisitNode->accuserName = accuser;
		newRevisitNode->accusedList = accused;
	
		m_visitLatterList.push_back( *newRevisitNode );
	}
	
	if( m_visitLatterList.size( ) >= 5 )
	{
		RevisitNow( );
	}
}


bool Bipartite::InsertToGroup( std::string accuser, std::vector<std::string> accused )
{
	bool foundAMacth = false;
	
	if( m_honest.DoesExists( accuser ) == true )
	{
		m_liar.Insert( accused );
		foundAMacth = true;
	}
	else if( m_liar.DoesExists( accuser ) == true )
	{
		m_honest.Insert( accused );
		foundAMacth = true;
	}
	// locate first accused
	else
	{
		for( std::vector<std::string>::iterator it = accused.begin( ); 
			it !=  accused.end( ); 
			++it )
		{
			if( m_honest.DoesExists( *it ) == true  )
			{
				m_liar.Insert( accuser );
				m_honest.Insert( accused );
				
				foundAMacth = true;
				break;
			}
			else if( m_liar.DoesExists( *it ) == true  )
			{	
				m_honest.Insert( accuser );
				m_liar.Insert( accused );
				
				foundAMacth = true;
				break;
			}
		}//eo for
	}
	return foundAMacth;
}


void Bipartite::RevisitNow( )
{
	std::list<RevisitNode>::iterator it = m_visitLatterList.begin( );
	
	while( it != m_visitLatterList.end( ) )
	{
		std::list<RevisitNode>::iterator currentIt = it;
		//move forward before deleting
		++it;
		
		bool didSuccesfullInsert = InsertToGroup( currentIt->accuserName, currentIt->accusedList );
		
		if( didSuccesfullInsert )
		{
			//pop this node off the list
			m_visitLatterList.erase( currentIt );
		}
	}
}


void Bipartite::FinalVisit( )
{
	size_t visitLatterListBeforeSize = 0;
	size_t visitLatterListAfterSize  = 0;
	
	do
	{
		if( m_visitLatterList.empty( ) == true ) 
		{
			//nothing to do
			return;
		}
		
		visitLatterListBeforeSize = m_visitLatterList.size( );
		
		RevisitNow();
		
		visitLatterListAfterSize = m_visitLatterList.size( );
	}
	//size of m_visitLatterList should decrease by 1 each iteration
	while ( visitLatterListBeforeSize > visitLatterListAfterSize );
	
	if ( m_visitLatterList.empty( ) != true ) 
	{
		std::cout<<" Failed to add all nodes to two buckets, possible bad input ";
	}
}

void Bipartite::PrintFormattedOutput( )
{
	int honestCount = m_honest.GetSize( );
	int liarCount = m_liar.GetSize( );
	
	if( honestCount >= liarCount )
	{
		std::cout<<honestCount<<" "<<liarCount<<"\n";
	}
	else
	{
		std::cout<<liarCount<<" "<<honestCount<<"\n";
	}
}

void Bipartite::PrintVisitLatterList( )
{
	std::cout<<"PRINTING REVIST LATTER LIST...!! \n  ";
	
	for (std::list<RevisitNode>::iterator it = m_visitLatterList.begin( );
		 it != m_visitLatterList.end( );
		 ++it )
	{
		std::cout<<"\n accuserName = "<<it->accuserName<<" \n \t accused List ";
		
		for( std::vector<std::string>::iterator listIterator = it->accusedList.begin( ); 
			listIterator !=  it->accusedList.end( ); 
			++listIterator )
		{
			std::cout<< *listIterator<<"\t";
		}
	}	
}