/* 
 * Jack Skupien
 * 3/24/22
 * Lab 6B: Quick Sort 2
 */

#include "sorting.hpp"
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

void recursive_sort(vector<double> &v, int start, int size, int print){
	int mid,last;//for middle/last elements
	int l,r,part=start;
	double tmp;
	if(size==0){
		cout<<start<<"  "<<size<<endl;
		return;
	}
	//print for call
	if(size>1){
		printf("S:%6d%6d      ",start,size);
		for(size_t i=0;i<v.size();i++)
			printf("%5.2lf", v[i]);
		cout<<endl;
	}

	if(size==1){//base case #1 (size is 1)
		//return
		return;
	}else if(size==2){//base case #2 (size is 2)
		if(v[start]>v[start+1]){//swaps if necessary
			tmp=v[start];
			v[start]=v[start+1];
			v[start+1]=tmp;
		}//does not change anything if already sorted
		return;
	}else{//if size is still greater than 2
		mid=start+(size/2);
		last=start+size-1;
		//find middle of the three
		if((v[mid]>v[last]&&v[mid]<v[start])||(v[mid]<v[last]&&v[mid]>v[start])){//if middle is partition, swap w/ start
			tmp=v[mid];
			v[mid]=v[start];
			v[start]=tmp;
		}else if((v[last]>v[mid]&&v[last]<v[start])||(v[last]<v[mid]&&v[last]>v[start])){//if last is partition, swap w/ start
			tmp=v[last];
			v[last]=v[start];
			v[start]=tmp;
		}

		//print with 'M'
		printf("M:%6d%6d%6.2lf",start,size,v[start]);
		for(size_t i=0;i<v.size();i++)
			printf("%5.2lf", v[i]);
		cout<<endl;

		//set left and right pointers and use a while loop to run through each element
		l=start+1;
		r=start+size-1;
		while(l<r){
			if((v[l]<v[start]))
				l++;
			if((v[r]>v[start]))
				r--;
			if(v[l]>=v[start]&&v[r]<=v[start]&&l<=r){//if pointers are in the right place, swap
				tmp=v[l];
				v[l]=v[r];
				v[r]=tmp;
				l++;
				r--;
			}
		}
		//once all swaps have been made, swap pivot with last element less than it
		for(int i=start+1;i<start+size;i++)
			if(v[i]>=v[start]){
				part=i-1;
				tmp=v[part];
				v[part]=v[start];
				v[start]=tmp;
				break;
			}
		//print and recurse
		printf("P:%6d%6d%6d",start,size,part);
		for(size_t i=0;i<v.size();i++)
			printf("%5.2lf", v[i]);
		cout<<endl;

		recursive_sort(v,start,part-start,print);//recurse left of partition
		recursive_sort(v,part+1,size-(part-start)-1,print);//recurse right of partition
	}
}

void sort_doubles(vector <double> &v, bool print){
	recursive_sort(v,0,v.size(),1);
	if(print){
		for(size_t i=0;i<v.size();i++)
			printf("%21.2lf",v[i]);
		cout<<endl;
	}
}
