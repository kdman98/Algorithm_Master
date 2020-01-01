#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int func(int rowsum[], int m) {   
	int sum = 0, max = INT_MIN;

	int allnegflag = 1;   
	for (int i = 0; i < m; i++) { // n^3 rd
		sum += rowsum[i];
		if (sum < 0) {
			sum = 0;
		}
		else if (sum > max) {     
			max = sum;
			allnegflag = 0;
		}
	}

	if (allnegflag != 1)
		return max;
	max = rowsum[0];

	for (int i = 1; i < m; i++) {
		if (rowsum[i] > max) {
			max = rowsum[i];
		}
	}
	return max;
}
int main() { 
	char filename[30]; // FILENAME INPUT
	char resultname[50]="result_"; // result
	int m, n, input[100][100]; // TODO: needs change for larger inputs
	int allnegflag = 1;
	int max = INT_MIN, sum = 0, tempmaxsum = INT_MIN;
	int index;
	double elapsedtime;
	
	scanf("%s", filename);
	strcat(resultname, filename);
	scanf("%d", &index);
	FILE* in = fopen(filename, "r");
	FILE* out = fopen(resultname, "w");
	fscanf(in,"%d %d\n", &m,&n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			fscanf(in,"%d ", &input[i][j]);
			/*
			if (input[i][j] > 0) {
				allnegflag = 0;
			}
			*/ // must be better option for this.
		}
	}
	if (index == 1) {
		double startclock = clock();
		for (int i = 0; i < m*n; i++) { // n^2
			for (int j = i; j < m*n; j++) { // n^4
				for (int k = i/n; k <= j/n; k++) {// n^5
					for (int l = i%n; l <= (j % n); l++) {// n^6, O(n^6) algo
						sum = sum + input[k][l];
					}
				}
				if (sum > max) {
					max = sum;
				} 
				sum = 0;
			}
		}
		double finishclock = clock();
		elapsedtime = (finishclock - startclock);
	}
	if (index == 2) {
		clock_t startclock = clock();

		max = INT_MIN;
		int left, right;
		int *rowsum = (int*)malloc(sizeof(int)*m);
		
		for (left = 0; left < n; left++) { // n
			
				
			for (right = left; right < n; right++) { // n^2
				for (int i = 0; i<m; i++) rowsum[i] = 0; // init
				for (int i = 0; i < m; i++) {// n^3
					for (int j = left; j <= right; j++) { // n^4
						rowsum[i] += input[i][j];
					}
					
				}
				sum = func(rowsum, m);   

				if (sum > max) {
					max = sum;
				}
			}
		}

		clock_t finishclock = clock();
		elapsedtime = finishclock - startclock;


	}
	if (index == 3) {
		clock_t startclock = clock();
		
		max = INT_MIN;
		int left, right;
		int *rowsum = (int*)malloc(sizeof(int)*m);
		/*
		for (left = 0; left < n; left++) { 
			for (int i = 0; i < m; i++) {
				rowsum[i] = 0;
			}
			for (right = left; right < n; right++) { 
				for (int i = 0; i < m; i++) {
					*(rowsum+i) = *(rowsum + i) + input[i][right];
					tempmaxsum = INT_MIN; // start of init
					allnegflag = 1;
					for (int i = 0; i < m; i++) { 
						sum = sum + rowsum[i];
						if (sum < 0) {
							sum = 0;
						}
						else if (sum > max) {
							tempmaxsum = sum;
							allnegflag = 0;
						}
					}
					if (allnegflag == 1) { // ?
						tempmaxsum = rowsum[0];
						for (int i = 1; i < n; i++) {
							if (rowsum[i] > tempmaxsum);
							tempmaxsum = rowsum[i];

						}
					}
					else {
						// tempmaxsum?
					}
					if (tempmaxsum > max) {
						max = tempmaxsum;
					}
				}
			}
			
		} */
		

		for (left = 0; left < n; left++) { // n
			for (int i = 0; i<m; i++)
				rowsum[i] = 0; // init

			for (right = left; right < n; right++) { // n^2
				for (int i = 0; i < m; i++) { // n^3 , O(n^3) algo  
					rowsum[i] += input[i][right];
				}
				sum = func(rowsum,m);    // same, n^3 , O(n^3) algo

				if (sum > max) {   
					max = sum;
				}
			}
		}

		clock_t finishclock = clock();
		elapsedtime = finishclock - startclock;
	}
	
	fprintf(out, "%s\n%d\n%d\n%d\n", filename,index,m,n);
	fprintf(out,"%d\n", max);
	fprintf(out, "%lf\n", elapsedtime * 1000000); // ¿Ö °è¼Ó 0¶ä ¤Ð
	
	
	return 0;
}
