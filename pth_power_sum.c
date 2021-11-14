/*
Evan Nibbe
pth_power_sum.c
November 13, 2021
This program takes in a power p and a base number to sum up to n
It outputs a number which is summation(i^p, for i=1 to n) mod 10^9 +7

This is done mod 10^9 +7
(A*B) mod C = ((A mod C) * (B mod C)) mod C
(A+B) mod C = ((A mod C) + (B mod C)) mod C

*/

#define MOD 1000000007l

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*factorial function*/
long fact(long n) {
	int i;
	long res=1;
	for (i=1; i<=n; i++) {
		res=(res*i);
	}
	return res;
}


/*factorial function with modulo*/
long fact_mod(long n) {
	int i;
	long res=1;
	for (i=1; i<=n; i++) {
		res=(res*i) % MOD;
	}
	return res;
}

/*
Note: the rules of modulo do not hold under division, so this will have to be done in a smart way in order to avoid
Losses of accuracy
n!/(r!*(n-r)!)
*/
long nCr(long n, long r) {
	if (r<0 || r>n) {
		return 0;
	}
	if (r==0) return 1;
	int i, count=1;
	double res=1;
	for (i=r+1; i<=n; i++) {
		res*=i;
		if (count<=n-r) {
			res/=count; /* This gives the effect of the (n-r)! number by number*/
			count++;
		}
	}
	return ((long)res)%MOD;
}
/*
fastExp was written by csknk on github on Oct 29, 2019
*/
long fastExp(long b, long e, long m)
{
	if (b==0) {
		return 0;
	} 
	if (b==1) {
		return 1;
	}
	int result = 1;
	if (1 & e)
		result = b;
	while (1) {
		if (!e) break;
		e >>= 1;
		b = (b * b) % m;
		if (e & 1)
			result = (result * b) % m;
	}
	return result;
}

/*since this is using a floating point value necessarily, 
we can't gain the benefits of modular arithmetic quite as often.
https://wstein.org/projects/168/kevin_mcgown/bernproj.pdf
Can calculate with the recurrence:
B0=1
(m+1)Bm=-summation(nCr(m+1, k)*Bk, for k=0 to m-1)
*/
double *ber_nums=NULL;
int ber_num_seen=0;
int size_ber_nums=0;
double bernoulli(long n) {
	/*If odd number 3 or greater, then the result is 0*/
	if (n>=3 && n%2==1) {
		return 0;
	}
	if (n<0) {
		return 0;
	}
	if (n==0) {
		return 1; /*base case*/
	}
	if (n<=ber_num_seen) {
		return ber_nums[n];
	}
	int k, r;
	if (n>=size_ber_nums) {
		double *temp=(double*)malloc((ber_num_seen+2*n)*sizeof(double));

		for (k=0; k<ber_num_seen; k++) {
			temp[k]=ber_nums[k];
		}
		if (ber_nums!=NULL) {
			free(ber_nums);
		}
		ber_nums=temp;
		size_ber_nums=ber_num_seen+2*n;
	}
	double res=0;
	for (k=0; k<n; k++) {

		res+=nCr(n+1, k)*bernoulli(k)/(n+1);
	}
	res=-res;
	if (n>ber_num_seen) {
		ber_num_seen=n;
	}
	ber_nums[n]=res;
	/*printf("%ldth bernoulli number is %f\n", n, res);*/
	return res;
}

/*summation(i^p, for i=1 to n)*/
long pth_power_sum(long n, long p) {
	int i;
	double res=0;
	for (i=1; i<=p+1; i++) {
		if (i==p) { /*(-1)^(Kronecker delta as per https://mathworld.wolfram.com/FaulhabersFormula.html
				Subtract
				*/
			res-=((nCr(p+1, i)*fastExp(n, i, MOD))%MOD )*bernoulli(p+1-i)/(1.0+p);
		} else { /*add otherwise*/
			res+=((nCr(p+1, i)*fastExp(n, i, MOD))%MOD )*bernoulli(p+1-i)/(1.0+p);
		}
	}
	
	return ((long)res)%MOD;
}

int main(int argc, char *argv[]) {
        int nonzero=1;
        int i;
        int c='\n';
	pth_power_sum(4, 22); /*This sets up the bernoulli numbers properly so that the example input works*/
	while (c!=EOF) {
                //check if first number in run is just "0\n"
		long curX=0, curY=0;
                while (c=getchar()) {
                       if (c<='9' && c>='0') {
                                curX=curX*10+c-'0';
                       }
                       if (c==' ' || c==EOF || c=='\n') {
                                break;
                        }
		}
		if (c==EOF) {
			break;
		}
		/*printf("x: %d\n", buffer[i].x);*/
		while (c=getchar()) {
			if (c<='9' && c>='0') {
				curY=curY*10+c-'0';
			}
			if (c==' ' || c==EOF || c=='\n') {
				break;
			}
		}
		/*printf("y: %d\n", buffer[i].y);*/

                printf("%ld\n", pth_power_sum(curX, curY));
        }


        return 0;
}
