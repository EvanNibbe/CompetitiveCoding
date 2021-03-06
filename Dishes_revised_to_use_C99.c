/*
Evan Nibbe
Competitive Learning in CS 
Dishes.c
This is to solve the problem at https://onlinejudge.org/index.php?option=onlinejudge&Itemid=8&page=show_problem&problem=3153
An input file has numbers grouped into two lines,
the first line has one number between 1 and 500 (if this number is 0, the program ends) for the number of numbers in the next line
the second line has that many numbers separated by spaces
The output consists of, if the perfect set of choices are made, what is the longest list that can be made
of numbers as they appear in consecutive order where a new number found in the list is either placed at the 
start or end of the list (or ignored) such that all numbers placed at the beginning of the list (the 
"bottom" of the stack of dishes in the problem) are greater than or equal to the later numbers, 
and all numbers placed at the end of the list (the "top" of the stack of dishes) are less than or 
equal to the earlier numbers.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct Dish_stack_place {
	int n, high, low;
	int set; 
};
typedef struct Dish_stack_place dishes;

#define MAX_DISH 501



int buildTable(int array[MAX_DISH], int n) { 
	if (n==0) {
		exit(0);
	} else if (n==1) {
		return 1;
	} else if (n==2) {
		return 2; 
	}
	dishes table[MAX_DISH*MAX_DISH];
	int i, j, k;
	for (i=0; i<n; i++) {
		table[i].n=1;
		table[i].high=array[i];
		table[i].low=array[i];
		table[i].set=1; 
		if (i>=1) {
			
			table[n+i].n=2;
			table[n+i].set=1; 
			int close_ind=0;
			for (j=0; j<i; j++) {
				if (abs(array[j]-array[i])<abs(array[close_ind]-array[i])) {
					close_ind=j;
				}
			}
			if (array[close_ind]<array[i]) {
				table[n+i].low=array[close_ind];
				table[n+i].high=array[i];
			} else {
				table[n+i].low=array[i];
				table[n+i].high=array[close_ind];
			
			}
		} 
			table[n*i].set=1; 
			table[n*i].n=1;
			table[n*i].low=array[0];
			table[n*i].high=array[0];
	}
	
	for (i=2; i<n; i++) {
		for (j=1; j<n; j++) {
			table[n*i+j].set=0;
			for (k=0; k<j; k++) {
				if (table[n*(i-1)+k].low>=array[j] && table[n*(i-1)+k].set) { 
					if (table[n*i+j].set==0) {
						table[n*i+j].set=1;
						table[n*i+j].n=table[n*(i-1)+k].n+1;
						table[n*i+j].low=array[j];
						table[n*i+j].high=table[n*(i-1)+k].high;
					} else if (table[n*(i-1)+k].n>table[n*i+j].n-1) {
						table[n*i+j].set=1;
						table[n*i+j].n=table[n*(i-1)+k].n+1;
						table[n*i+j].low=array[j];
						table[n*i+j].high=table[n*(i-1)+k].high;
					} else if (table[n*(i-1)+k].n==table[n*i+j].n-1 && abs(table[n*(i-1)+k].high-array[j])<abs(table[n*i+j].high-table[n*i+j].low)) {
						table[n*i+j].set=1;
						table[n*i+j].n=table[n*(i-1)+k].n+1;
						table[n*i+j].low=array[j];
						table[n*i+j].high=table[n*(i-1)+k].high;
					}
				} else if (table[n*(i-1)+k].high<=array[j] && table[n*(i-1)+k].set) {
					if (table[n*i+j].set==0) {
						table[n*i+j].set=1;
						table[n*i+j].n=table[n*(i-1)+k].n+1;
						table[n*i+j].high=array[j];
						table[n*i+j].low=table[n*(i-1)+k].low;
					} else if (table[n*(i-1)+k].n>table[n*i+j].n-1) {
						table[n*i+j].set=1;
						table[n*i+j].n=table[n*(i-1)+k].n+1;
						table[n*i+j].high=array[j];
						table[n*i+j].low=table[n*(i-1)+k].low;
					} else if (table[n*(i-1)+k].n==table[n*i+j].n-1 && abs(table[n*(i-1)+k].low-array[j])<abs(table[n*i+j].high-table[n*i+j].low)) {
						table[n*i+j].set=1;
						table[n*i+j].n=table[n*(i-1)+k].n+1;
						table[n*i+j].high=array[j];
						table[n*i+j].low=table[n*(i-1)+k].low;
					}
				}
				
				if (table[n*i+j].set==0) {
					table[n*i+j].n=0;
					for (k=j-1; k>=0 && table[n*i+j].n<k+1; k--) {
						if (table[n*(i-1)+k].n>table[n*i+j].n && table[n*(i-1)+k].set) {
							table[n*i+j]=table[n*(i-1)+k];
						}
					}
				}
			}
		}
	}
	int tallest_stack=2;
	for (j=2; j<n; j++) {
		if (table[n*(n-1)+j].n>tallest_stack) {
			tallest_stack=table[n*(n-1)+j].n;
		}
	}
	return tallest_stack;
}

int main(int argc, char *argv[]) {
        int nonzero=1, i;
	int array[MAX_DISH];
        while (nonzero) {
                int decimal=0;
                double divisor=10;
                int numToSee=0, c;
                while (c=getchar()) {
                        if (c<='9' && c>='0') {
                                numToSee=numToSee*10+c-'0';
                        }
                        if (c==EOF || c=='\n') {
                                if (c==EOF) {
                                        exit(0);
                                }
                                break;
                        }
                }
                if (numToSee==0) {
                        return 0;
                }
                for (i=0; i<numToSee; i++) {
                        decimal=0;
                        divisor=10;
                        int curX=0;
                        while (c=getchar()) {
                                if (c<='9' && c>='0') {
                                        curX=curX*10+c-'0';
                                }
                                if (c==' ' || c==EOF || c=='\n') {
                                        break;
                                }
                        }
                        array[i]=curX;

                }
		int result=buildTable(array, numToSee);
                printf("%d\n", result);
        }

        

        return 0;
}
