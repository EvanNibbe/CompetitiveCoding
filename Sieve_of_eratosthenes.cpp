//#define DEBUG
/*
Evan Nibbe
December 8, 2021
Sieve_of_eratosthenes.cpp
This program lists the primes that occur between two numbers.
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>

int prev_max=-1;
int num_primes_possible=-1; //will be a multiple of 16 minus 1, establishes the size-1 of array_primes
char *array_primes=NULL; //this is used to hold the bitwise information where 0 means "not marked" and 1 means "composite" indexing all of the numbers from 0 to num_primes_possible
			//this only considers odd numbers.
int is_marked(int to_check) { //returns 1 if composite, returns -1 on error
	if (to_check>num_primes_possible || to_check<2) {
		return -1;
	}
	int index=to_check/16;
	int mask=1<<((to_check/2)%8);
	return (array_primes[index] & mask)>=1;
}
void mark(int to_check) {
	if (to_check<=3 || to_check>num_primes_possible) {
		return ;
	} 
	int index=to_check/16;
	int mask=1<<((to_check/2)%8); //only counting odd numbers, so 3 becomes 1, 5 becomes 2
	array_primes[index] =array_primes[index] | mask; //causes the marked spot to become 1.
}
void sieve(int max, int min) { //print primes from min to max
	int to_check=max;
	if (to_check>num_primes_possible && to_check<2000000000) {
		if (num_primes_possible==-1) {
			num_primes_possible=((to_check+15)/16)*16-1;
			array_primes=(char*)malloc(sizeof(char)*(num_primes_possible/16+1));
			memset(array_primes, 0, num_primes_possible/16+1);
		} else {
			int temp=num_primes_possible;
			if (temp+to_check<=1000000000) {
				num_primes_possible=((to_check+num_primes_possible+15)/16)*16-1;
			} else {
				num_primes_possible=((to_check+15)/16)*16-1;
			}
			char *temp_array_primes=(char*)malloc(sizeof(char)*(num_primes_possible/16+1));
			memset(temp_array_primes, 0, num_primes_possible/16+1);
			memcpy(temp_array_primes, array_primes, temp/16+1);
			free(array_primes);
			array_primes=temp_array_primes;
		}
	} else if (to_check>=2000000000 || to_check<2) {
		return ;
	}
	if (2>=min) {
		printf("2\n");
	}

	int max_div=(int)sqrt(max), i, k;
	for (i=3; i<=max_div; i+=2) {
#ifdef DEBUG
			printf("Line %d; i=%d\n", __LINE__, i);
#endif
		if (is_marked(i)==0) {
#ifdef DEBUG
			printf("Line %d; i=%d\n", __LINE__, i);
#endif
			if (i>=min) {
				printf("%d\n", i);
			}
			int start=prev_max;
			if (start<i*i) {
				start=i*i;
			} else {
				start=((start+i-1)/i)*i; //the next multiple of i above start, or start itself if start is a multiple of i
			}
			for (k=start; k<=max; k+=i) {
				mark(k);
			}
		}
		 //increment twice because we know that evens are not prime
	}
	for (; i<=max; i+=2) {
#ifdef DEBUG
			printf("Line %d; i=%d\n", __LINE__, i);
#endif
		if (is_marked(i)==0 && i>=min) {
#ifdef DEBUG
			printf("Line %d; i=%d\n", __LINE__, i);
#endif
			printf("%d\n", i);
		}
	}
	prev_max=max;
	return ;
}
int main(int argc, char *argv[]) {
	int tests=0, i;
	std::cin >> tests;
	for (i=0; i<tests; i++) {
		int min, max;
		std::cin >> min;
		std::cin >> max;
		sieve(max, min);
		printf("\n");
	}
	
	return 0;
}
