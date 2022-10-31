/* 
 * Jack Skupien
 * 3/24/22
 * Lab 6A: Merge Sort
 */

#include "sorting.hpp"
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

void recursive_sort(vector<double> &v, vector<double> &temp, int start, int size, int print){
	//declare indexes and iterators for merge part
	int it=start;
	int first=start;
	int second=start+size/2;
	double tmp;

	//print call
	if(size>1){
		printf("B:%6d%6d  ",start,size);
		for(size_t i=0;i<v.size();i++)
			printf("%5.2lf", v[i]);
		cout<<endl;
	}

	if(size==1){//base case #1 (size is 1)
		return;
	}else if(size==2){//base case #2 (size is 2)
		if(v[start]>v[start+1]){//swaps if necessary
			tmp=v[start];
			v[start]=v[start+1];
			v[start+1]=tmp;
		}

		//print and return
		printf("E:%6d%6d  ",start,size);
		for(size_t i=0;i<v.size();i++)
			printf("%5.2lf", v[i]);
		cout<<endl;
		return;
	}else{//if size is still greater than 2
		//call reecursively on first and second half
		recursive_sort(v,temp,start,size/2,print);
		recursive_sort(v,temp,start+size/2,size-(size/2),print);
		//now that recursion has sorted, merge
		while(first<start+size/2||second<start+size){//loops through
			if((v[first]<v[second]||second==start+size)&&first<start+size/2){//if element from first half is less, put it in first position
				temp[it]=v[first];
				first++;//iterate to next element of first half
			}else{
				temp[it]=v[second];
				second++;
			}
			it++;
		}
		for(int i=start;i<start+size;i++)//copy back onto original vector
			v[i]=temp[i];
		//print when reaches the end of each call
		printf("E:%6d%6d  ",start,size);
		for(size_t i=0;i<v.size();i++)
			printf("%5.2lf", v[i]);
		cout<<endl;
	}
}

void sort_doubles(vector <double> &v, bool print){
	vector<double> temp(v.size(),0);
	recursive_sort(v,temp,0,v.size(),1);
	if(print){
		for(size_t i=0;i<v.size();i++)
			printf("%21.2lf",v[i]);
		cout<<endl;
	}
}
