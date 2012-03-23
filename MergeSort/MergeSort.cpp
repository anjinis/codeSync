#include <string>
#include <iostream>
#include <vector>

using namespace std;

void printVector( vector<int> toPrint )
{
	cout<<"\n";
	for (vector<int>::iterator it = toPrint.begin( ); it != toPrint.end( ); ++it) 
	{
		cout<<"\t"<< *it;
	}
	cout<<"\n";
}


vector<int> Merge( vector<int> &left, vector<int> &right )
{
	vector<int>::iterator leftIt = left.begin( );
	vector<int>::iterator rightIt = right.begin( );
	vector<int> result;
	
	while (leftIt != left.end( ) && rightIt != right.end( ) ) 
	{
		if( *leftIt < *rightIt)
		{
			result.push_back( *leftIt );
			++leftIt;
		}
		else 
		{
			result.push_back( *rightIt );
			++rightIt;
		}
	}
	
	 while (leftIt != left.end( ) ) 
	 {
	 result.push_back( *leftIt );
	 ++leftIt;
	 }
	 while ( rightIt != right.end( ) ) 
	 {
	 result.push_back( *rightIt );
	 ++rightIt;
	 }
	cout<<"\n Finished Merge";
	return result;
}

vector<int> MergeSort(vector<int> &toSortList)
{
	int toSortListSize = (int)toSortList.size( );
	
	if (toSortListSize <= 1 ) 
	{
		return toSortList;
	}
	else 
	{
		int middle = toSortListSize / 2;
		vector<int> left, right;
		
		vector<int>::iterator it = toSortList.begin( );
		for (int i = 0; i < middle; ++i) 
		{
			left.push_back(*it);
			++it;
		}
		while (it != toSortList.end( ) )
		{
			right.push_back(*it);
			++it;
		}
		
		left = MergeSort(left);
		right = MergeSort(right);
		
		printVector( left );
		printVector( right );
		
		return Merge(left, right);	
	}
	
}

int main( )
{
	vector<int> list;
	
	list.push_back( 5 );
	list.push_back( 3 );
	list.push_back( 7 );
	list.push_back( 8 );
	
	list.push_back( 2 );
	list.push_back( 9 );
	list.push_back( 1 );
	list.push_back( 10 );

	vector<int> ans = MergeSort( list );
	
	printVector( list );
	printVector( ans );
	
	return 0;
}

