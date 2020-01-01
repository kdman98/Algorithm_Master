#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define INT_MIN -2147483646
void swap(int * a, int * b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void makeheap(int *arr, int left, int right) {
	for (int i = left; i <= right; i++) {
		int now = i;
		while (now > 0) {
			int par = now - 1 >> 1;
			if (arr[par] < arr[now]) swap(arr + par, arr + now);
			now = par;
		}
	}
}

void heapsort(int *arr, int left, int right) {
	makeheap(arr, left, right);
	for (int i = right; i>left; i--) {
		swap(arr, arr + i);
		int left = 1, right = 2;
		int sel = 0, par = 0;
		while (1) {
			if (left >= i) break;
			if (right >= i) sel = left;
			else {
				if (arr[left] < arr[right]) sel = right;
				else sel = left;
			}
			if (arr[sel] > arr[par]) {
				swap(arr + sel, arr + par);
				par = sel;
			}
			else break;
			left = (par << 1) + 1;
			right = left + 1;
		}
	}
}

void insertionsort(int arr[], int left, int right) {
	for (int i = left; i<right; i++) {
		int key = arr[i + 1];
		int j;
		for (j = i; j >= left; j--) {
			if (arr[j] > key) arr[j + 1] = arr[j];
			else break;
		}
		arr[j + 1] = key;
	}
}


void myquicksort(int arr[], int left, int right, int depth) {
	if (depth == 0) {
		int size = right - left + 1;
		if (size > 16) {
			heapsort(arr, left, right);
		}
		return;
	}

	int i = left, j = right;
	int pivot = arr[(left + right) / 2];
	int temp;
	do {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			swap(arr + i, arr + j);
			i++;
			j--;
		}
	} while (i <= j);

	if (left < j)
		myquicksort(arr, left, j, depth - 1);

	if (i < right)
		myquicksort(arr, i, right, depth - 1);

}

void introsort(int arr[], int n) {
	int limit = 2 * ceil(log2(n));
	if (n <= 16) {
		insertionsort(arr, 0, n - 1);
		return;
	}
	myquicksort(arr, 0, n - 1, limit);
	insertionsort(arr, 0, n - 1);
} // 여기까지 index4 용 함수들

void merge(int list[], int left, int mid, int right,int sorted[]){
	  int i, j, k, l;
	    i = left;
	  j = mid+1;
	    k = left;

			  /* 분할 정렬된 list의 합병 */
		  while(i<=mid && j<=right){
		      if(list[i]<=list[j])
			        sorted[k++] = list[i++];
		      else
			        sorted[k++] = list[j++];
		   }

	    		   if(i>mid){
				    for(l=j; l<=right; l++)
			      sorted[k++] = list[l];
			 }
				 		  else{
			     for(l=i; l<=mid; l++)
				      sorted[k++] = list[l];
		   }
		     for(l=left; l<=right; l++){
			    list[l] = sorted[l];
			  }
}
void merge_sort(int list[],int left,int right,int sorted[]){
	int mid;
	if(left<right){
		mid=(left+right)/2;
		merge_sort(list,left,mid,sorted);
		merge_sort(list,mid+1,right,sorted);
		merge(list,left,mid,right,sorted);
	}
}
int partition(int input[], int left, int right){
	int pivot=input[left];
	int low=left+1;
	int high=right;
	int temp;
	while (low <= high) 
		    {
			    while (pivot >= input[low] && low<=right){
					low++;
				}
				while(pivot >= input[low] && low <= right){
					low++;
				}
				while(pivot<=input[high]&&high >= (left+1)){
					high--;
				}
				if(low<=high){
					temp=input[low];
					input[low]=input[high];
					input[high]=temp;
				}
			}
		temp=input[left];
		input[left]=input[high];
		input[high]=temp;
		return high;
}
void quicksort(int input[],int left,int right){
	if(left<=right){
		int pivot=partition(input,left,right);
		quicksort(input,left,pivot-1);
		quicksort(input,pivot+1,right);
	}
}
int main(int argc, char* argv[]) { 
	char filename[50]; // FILENAME INPUT
	char resultname[50]="result_"; // resultname 
	char algoindex[3];
	int n, input[1000]; // 
	int allnegflag = 1;
	int max = INT_MIN, sum = 0, tempmaxsum = INT_MIN;
	int index;
	double elapsedtime;
	
	strcpy(filename,argv[1]);
	index=(int)argv[2][0]-(int)'0';
	algoindex[0]=argv[2][0];
	strcat(resultname,algoindex);
	strcat(resultname,"_");
	strcat(resultname, filename);

	FILE* in = fopen(filename, "r");
	FILE* out = fopen(resultname, "w");
	fscanf(in,"%d\n", &n);
	for (int i = 0; i < n; i++) { // TODO: FIX THIS 
			fscanf(in,"%d ", &input[i]);
			/*
			if (input[i][j] > 0) {
				allnegflag = 0;
			}
			*/ // must be better option for this. >> found below using allnegflag
	}
	if (index == 1) {
		clock_t startclock=clock();
		int i,j,least,temp;
		for(i=0;i<n-1;i++){
			least=i;
			for(j=i+1;j<n;j++){
				if(input[j]<input[least])
					least=j;
			}
			if(i != least){
				temp=input[i];
				input[i]=input[least];
				input[least]=temp;
			}
		}

		double finishclock = clock();
		elapsedtime = (finishclock - startclock);//checks time! works same for later code.
	}
	if (index == 2) {
		clock_t startclock = clock();
		quicksort(input,0,n-1);
		
		clock_t finishclock = clock();
		elapsedtime = finishclock - startclock;


	}
	if (index == 3) {
		int sorted[1000];
		clock_t startclock = clock();
		merge_sort(input,0,n-1,sorted);
		clock_t finishclock = clock();
		elapsedtime = finishclock - startclock;
		fprintf(out, "%s\n%d\n%d\n", filename,index,n);
			for(int i=0;i<n;i++){
		fprintf(out, "%d ",sorted[i]);
			}
		fprintf(out, "\n%.3lf\n", elapsedtime/1000); // PRINT ALL THINGS
		return 0;

	}
	if(index==4){ //introsort?
		introsort(input, n);
	}
	fprintf(out, "%s\n%d\n%d\n", filename,index,n);
	for(int i=0;i<n;i++){
		fprintf(out, "%d ",input[i]);
	}
	fprintf(out, "\n%.3lf\n", elapsedtime/1000); // PRINT ALL THINGS
	
	
	return 0;
}
