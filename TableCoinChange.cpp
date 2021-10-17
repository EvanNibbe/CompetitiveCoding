/*
Evan Nibbe
CoinChange.cpp
This program takes in two lines of numbers (on a cycle)
The first line contains two numbers separated by a space, the value of change to make and the number of denominations of coins to use
The second line contains the space-delimited numbers representing the values of coins available (each coin may be used infinitely many times).
The output of the program is the number of ways that amount of change can be made.

Edit: Instead of using strings as the basis for the keys in the unordered map, since the problem states that the maximum number of coins is 50, I can just multiply the 
value remaining by 100 and add it to the value of j to get a key to a unique place in the map.

Edit: I need to use a table in order for this to be fast enough.
*/
#include <iostream>
using namespace std;
#include <unordered_map>
#include <string>
#include <sstream>
#include <cstdlib>

//check if a spot in what would have been a matrix has been chosen before
//int exists(unordered_map<int, long> basis, int v, int j) {
//	int key= v*100 + j;
//	//unordered_map<int, long>::const_iterator got = basis.find (key);
//	try  {
//		basis.at(key);
//		return 1;
//	} catch (...)  {
//		return 0;
//	}
//}

struct intlong {
	int visited;
	long ways;
};

typedef intlong tind;
long num_ways(tind *basis, int, int, int, int*);
//basis is the set of how a given value and choice of how many possible coins from values, the number of ways of making that change exactly.
tind call_num_ways_buffer[250*51]; //slightly more room than the problem required.
long call_num_ways(int v, int j, int *values) {
	tind *temp=call_num_ways_buffer;//(tind*)malloc(v*j*sizeof(tind));
	for (int i=0; i<v*j; i++) {
		temp[i].visited=0;
	}
	long res=num_ways(temp, v, j, j, values);
	//free(temp);
	return res;
}
long num_ways(tind *basis, int v, int j, int n, int *values) {
	int visit=v*n +j;
	if (basis[visit].visited==1) {
		return basis[visit].ways;
	}
	
	if (v==0) {
		basis[visit].visited=1;
		basis[visit].ways=1; //there is only one way to get correct change when there is nothing to do to get correct change.
		return 1;
	} else if (v<0) {
		basis[visit].visited=1;
		basis[visit].ways=0;
		return 0;
	} else if (j==0) {
		if (v-values[j]==0) {
			basis[visit].visited=1;
			basis[visit].ways=1;
			return 1;
		} else if (v-values[j]<0) {
			basis[visit].visited=1;
			basis[visit].ways=0;
			return 0;
		} else { //see whether v is evenly divided by j to see whether a solution exists
			if (v%values[j]==0) {
				basis[visit].visited=1;
				basis[visit].ways=1;
				return 1;
			} else {
				basis[visit].visited=1;
				basis[visit].ways=0;
				return 0;
			}
		}
	} else if (values[j]>v) {
		basis[visit].visited=1;
		basis[visit].ways=num_ways(basis, v, j-1, n, values); //disclude this coin.
		return basis[visit].ways;
	} else { //check number of ways that involve discluding this coin and including this coin.
		basis[visit].visited=1;
		basis[visit].ways=num_ways(basis, v, j-1, n, values)+num_ways(basis, v-values[j], j, n, values);
		return basis[visit].ways;
	}
}

int compare(const void *p1, const void *p2) {
	const int *a=(const int*)p1, *b=(const int *)p2;
	return a[0]-b[0];
}

int main() {
	int num_cents, num_coins, *array=NULL, length=0;
	while ((cin >> num_cents >> num_coins) && num_coins>0) {
		if (length<num_coins && array==NULL) {
			array=(int*)malloc(num_coins*sizeof(int));
			length=num_coins;
		} else if (length<num_coins) {
			free(array);
			array=(int*)malloc((num_coins+2*length)*sizeof(int));
			length=2*length+num_coins;
		}
		for (int i=0; i<num_coins; i++) {
			cin >> array[i];
		}
		qsort(array, num_coins, sizeof(int), compare);
		cout << call_num_ways(num_cents, num_coins, array) << "\n";
	}
	if (length>0)
		free(array);
	return 0;
}
