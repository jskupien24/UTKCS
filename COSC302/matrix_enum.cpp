/*
 * Jack Skupien
 * CS302
 * 02/21/2022
 * Lab 4 - Enumeration
 */
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<stdlib.h>
#include<vector>

using namespace std;

class Matrix {
	public:
		int W;
		int E;
		int P;     								 /* This is 'x' or 'h' */
		vector <int> Perm;					     /* Permutation of 0 .. (W-1), for the 'X' elements. */
		vector <int> Non_X;					     /* This is the row/col id of each of the non-X elements. */
		vector <int> E_ID;					     /* This is the row/col id of the E elements */
		void Print();						     /* Print the matrix defined by W, Perm and E_ID */
		void Permute(int index);				 /* This is the recursive permuting method. */
		void Choose(int index,size_t toAdd);     /* This is the recursive n-choose-k method.*/
};

void Matrix::Permute(int index){//recursive method based on example from enumeration lecture notes
	int num;
	if(index==W){//check for base case, fill Non_X with IDs, and return if it is reached
		Non_X.clear();
		for(int i=0;i<W;i++)//these for loops seearch to see if elements are not in Perm and adds them
			for(int j=0;j<W;j++)
				if(Perm[i]!=j)
					Non_X.push_back((i*W)+j);
		Choose(0,E);
		return;
	}else{//if not base case
		for(int i=index;i<W;i++){//loops through vector elements and swaps with index
			//swap
			num=Perm[i];
			Perm[i]=Perm[index];
			Perm[index]=num;
			//call recursion
			Permute(index+1);
			//swap back
			num=Perm[i];
			Perm[i]=Perm[index];
			Perm[index]=num;
		}
	}
}

void Matrix::Choose(int index, size_t toAdd){//recursive n-choose-k method resembling the n-choose-k method from the enumeration lecture notes
	//check for base cases
	if(toAdd==0){//first base case finished adding
		Print();
		return;
	}else if(toAdd>Non_X.size()-index)//second base case: more to add with not enough spots
		return;
	//push Non_X[index] onto E_ID and recurse, then take it back out
	E_ID.push_back(Non_X[index]);
	Choose(index+1,toAdd-1);
	E_ID.pop_back();

	//call recursively again without putting anything in E_ID
	Choose(index+1,toAdd);
}


void Matrix::Print(){
	unsigned int hexline;
	bool missing=true;
	for(int i=0;i<W;i++){
		hexline=0;//reset hexline
		for(int j=0;j<W;j++){
			for(int k=0;k<E;k++)
				if(E_ID[k]==i*W+j){//finds element in E_ID
					hexline=(hexline|(1<<j));//updates hexline
					missing=false;
					if(P=='x')cout<<'E';
					break;//exit the loop
				}
			if(missing){
				if(Perm[i]==j){
					hexline=(hexline|1<<j);
					if(P=='x')cout<<'X';
				}else if(P=='x')
					cout<<'.';
			}
			missing=true;
		}
		if(P=='h')
			cout<<hex<<hexline;
		cout<<endl;
	}
	cout<<endl;//extra line after each matrix
}

int main(int argc, char** argv){
	(void)argc;
	Matrix mat;//create object
	//give command line args to matrix
	mat.W=atoi(argv[1]);
	mat.E=atoi(argv[2]);
	mat.P=argv[3][0];
	//fill perm
	for(int i=0;i<mat.W;i++)
		mat.Perm.push_back(i);
	mat.Permute(0);
	return 0;
}
