#!/usr/bin/env python
import collections
import sys

#dict of activities, caloricimpact
dictionary = collections.defaultdict(lambda: [])
#List of calories
calorieCostList = []
#stores true/false for each row-col
hashTable = {}

#
def fillMatrix(row, col):
    #create uniq key
    key = str(row) + str( '.' ) + str(col)
    #base case
    if key in hashTable:
        return hashTable[key]
    elif row == 0:
        #0 is the sum we are looking for
        val = ( calorieCostList[0] == col )
    else:
        val = ( fillMatrix(row - 1, col) or calorieCostList[row] == col or fillMatrix(row - 1, col - calorieCostList[row]) )
    hashTable[key] = val
    return val

def derive_subset_for(i, currentSum = 0, resultSet  = []):
    if i >= 0:
        if calorieCostList[i] == currentSum:
            resultSet.append(dictionary[calorieCostList[i]].pop())
            currentSum = 0
            return derive_subset_for(i - 1, currentSum, resultSet)
        elif fillMatrix(i-1, currentSum):
            derive_subset_for(i - 1, currentSum, resultSet)
        elif fillMatrix(i - 1, currentSum - calorieCostList[i]):
            resultSet.append(dictionary[calorieCostList[i]].pop())
            currentSum -= calorieCostList[i]
            return derive_subset_for(i - 1, currentSum, resultSet)
        return resultSet

#Prints diet plan if a sol exits, else prints no solution
def getDietPlan(matrixTotalRows):
    if fillMatrix(matrixTotalRows, 0):
        resultList = derive_subset_for(matrixTotalRows)
        for result in resultList:
            print result
    else:
        print "no solution"

def main():
    count = 0
    for line in sys.stdin.readlines():
        if count == 0:
            count = 1
            numberOfItems = int( line )
            
            #assert preconditions
            assert( numberOfItems <= 50)
            assert( numberOfItems >= 1)
        else:
            [activity, calories]= line.split(" ")
            calories = int(calories)
            dictionary[calories].append(activity)
            calorieCostList.append(calories)
            
    assert( numberOfItems == len(calorieCostList) )
    
    #TBD: optimize input: remove 1 for any -1
    getDietPlan(numberOfItems -1)
#if __name__ == '__main__':
main()
