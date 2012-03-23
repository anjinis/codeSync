/*
 *  liar.h
 *  testCmdTool
 *
 *  Author anjini shukla on 7/11/11.
 *  Copyright 2011 All code Anjini Shukla, all rights reserved
 *
 */


#include<string>
#include<map>
#include<cmath>
#include<ext/hash_map>
#include<vector>
#include <list>
#include <iostream>

using namespace __gnu_cxx;

_GLIBCXX_BEGIN_NAMESPACE(__gnu_cxx)

template<> struct hash< ::std::string >
{
	size_t operator()( const ::std::string& x) const
	{
		return hash< const char* >()( x.c_str() );
	}
};

_GLIBCXX_END_NAMESPACE

struct RevisitNode
{
	std::string accuserName;
	std::vector<std::string> accusedList;	
};


/*******************************************
 Group
 key: name of the person
 value: bool
 
 ******************************************/
class Group
{
public:
	Group( );
	
	bool DoesExists( std::string toFindName );
	
	void Insert( std::string toInsertName );
	void Insert( std::vector<std::string> toInsertVector );
	void Print( );
	
	int GetSize( )
	{
		return m_hashTable.size( );
	}
	
private:
	hash_map<std::string, bool>	m_hashTable;
};


class Bipartite
{
public:
	Bipartite( );
	
	bool InsertToGroup( std::string accuser, std::vector<std::string> accused );
	void Insert( std::string accuser, std::vector<std::string> accused );
	
	void SeedInsert( std::string accuser, std::vector<std::string> accused );
	void RevisitNow( );
	void PrintFormattedOutput( );
	void PrintVisitLatterList( );
	void FinalVisit( );
	
	Group						m_honest;
	Group						m_liar;
	std::list< RevisitNode >	m_visitLatterList;	
};