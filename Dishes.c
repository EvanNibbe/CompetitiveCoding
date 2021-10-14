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
	int set; //establishes whether a value has been checked for this position (making it a base case)
};

typedef struct Dish_stack_place dishes;

#define MAX_DISH 501


//dishes *max(dishes *arg1, dishes *arg2) {
//	if (arg1->n>arg2->n) {
//		return arg1;
//	} else {
//		return arg2;
//	}
//}
			//n=size array				//i=max # dishes	j=max index of dish
//dishes *recurback(int *array, int n, dishes table[MAX_DISH*MAX_DISH], int i, 		int j) {
//	plus(table[i*MAX_DISH+j], j, array[j]);
//	return max(recurback(array, table, )
//}

int buildTable(int array[MAX_DISH], int n) { //the first n numbers are used
	if (n==0) {
		exit(0);
	} else if (n==1) {
		return 1;
	} else if (n==2) {
		return 2; //you are guaranteed with 2 dishes total to be able to stack both
	}
	dishes table[MAX_DISH*MAX_DISH];
	for (int i=0; i<n; i++) {
		table[i].n=1;
		table[i].high=array[i];
		table[i].low=array[i];
		table[i].set=1; //base case of recursion
		if (i>=1) {
			//need to find the dish prior to i which is closest to array[i] in number (thus maximizing the amount of other dishes later that can be put with it)
			//second row of table
			table[n+i].n=2;
			table[n+i].set=1; //base case of recursion.
			int close_ind=0;
			for (int j=0; j<i; j++) {
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
			table[n*i].set=1; //base case of recursion.
			table[n*i].n=1;
			table[n*i].low=array[0];
			table[n*i].high=array[0];
	}
	//Now for determining the best path by proxy, starting with replacing 
	//dishes1 with what the line in the table where we can choose up to 3 dishes would be
	for (int i=2; i<n; i++) {
		for (int j=1; j<n; j++) {
			table[n*i+j].set=0;
			//we need to determine the first set of dishes in the previous row which gives us the ability to put in dish j (that does not already include dish j)
			//we have a primary preference to find one that has i dishes right now (or maximum number of dishes)
			//we have a preference to first find the position that is closest to array[j] that also has maximum dishes right now
			for (int k=0; k<j; k++) {
				if (table[n*(i-1)+k].low>=array[j] && table[n*(i-1)+k].set) { //&& table[n*(i-1)+k].n==i) {
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
				//if none fit this dish, then just copy the biggest prior set
				if (table[n*i+j].set==0) {
					table[n*i+j].n=0;
					for (int k=j-1; k>=0 && table[n*i+j].n<k+1; k--) {
						if (table[n*(i-1)+k].n>table[n*i+j].n && table[n*(i-1)+k].set) {
							table[n*i+j]=table[n*(i-1)+k];
						}
					}
				}
			}
		}
	}
	//Now, all we must do is find the tallest stack of dishes on the last rank of the matrix.
	int tallest_stack=2;
	for (int j=2; j<n; j++) {
		if (table[n*(n-1)+j].n>tallest_stack) {
			tallest_stack=table[n*(n-1)+j].n;
		}
	}
	return tallest_stack;
}

int main(int argc, char *argv[]) {
        int nonzero=1;
	int array[MAX_DISH];
        while (nonzero) {
                //check if first number in run is just "0\n"
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
                for (int i=0; i<numToSee; i++) {
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

        //The returned value is an integer

        return 0;
}
