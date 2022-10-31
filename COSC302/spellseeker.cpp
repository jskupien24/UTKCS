/*
 * Jack Skupien
 * 5/8/2022
 * CS302 - Dynamic Programming
 * Lab AC - Spellseeker
 */
#include<iostream>
#include<vector>
#include<map>
#include<sstream>
#include<cmath>
using namespace std;

class Answer{//Answer class from lab writeup
	public:
		int length;
		int r;
		int c;
		string key;
		Answer *nexta;
};

class SpellSeeker{//spellseeker class from lab writeup
	public:
		vector <string> grid;
    	map <string, Answer *> cache;
    	int total_r;
    	int total_c;

    Answer *MaxPathLen(int r, int c);
};

Answer *SpellSeeker::MaxPathLen(int r,int c){//recursive function that returns the longest path that can stem from a cell 
	Answer *a1=new Answer;
	Answer *a2=new Answer;
	//make an id
	stringstream ss;
	ss<<r<<' '<<c;
	string tmpid=ss.str();
	//if(cache.find(tmpid)!=cache.end())//if already in cache
	//	return cache[tmpid];
	//if not in cache, set up new *Answer
	a1->r=r;
	a1->c=c;
	a1->length=0;
	a1->nexta=NULL;
	a1->key=tmpid;
	//store cell val and remove from grid
	int val=(int)(grid[r][c]);
	grid[r][c]='-';
	//start checking if each of the 6 possible adjacent cells are within 1 of the current cell
	//if not at top and above cell is still in grid, see if above cell is within 1
	if((r!=0)&&(grid[r-1][c]!='-')&&(((int)grid[r-1][c]==val-1)||((int)grid[r-1][c]==val+1))){
		a2=MaxPathLen(r-1,c);
		if(a2->length+1>a1->length){
			//cout<<a2->length<<endl;
            a1->length=a2->length+1;
			a1->nexta=a2;
		}
	}
	//if not at bottom and below cell is still in grid, see if below cell is within 1
	if((r!=(int)grid.size()-1)&&(grid[r+1][c]!='-')&&(((int)grid[r+1][c]==val-1)||((int)grid[r+1][c]==val+1))){
		a2=MaxPathLen(r+1,c);
		if(a2->length+1>a1->length){
            //cout<<a2->length<<endl;
			a1->length=a2->length+1;
			a1->nexta=a2;
		}
	}
	//if not all the way left and left cell is still in grid, see if left cell is within 1
	if((c!=0)&&(grid[r][c-1]!='-')&&(((int)grid[r][c-1]==val-1)||((int)grid[r][c-1]==val+1))){
		a2=MaxPathLen(r,c-1);
		if(a2->length+1>a1->length){
            //cout<<a2->length<<endl;
			a1->length=a2->length+1;
			a1->nexta=a2;
		}
	}
	//if not all the way right and right cell is still in grid, see if right cell is within 1
	if((c!=(int)grid[0].size()-1)&&(grid[r][c+1]!='-')&&(((int)grid[r][c+1]==val-1)||((int)grid[r][c+1]==val+1))){
		a2=MaxPathLen(r,c+1);
		if(a2->length+1>a1->length){
            //cout<<a2->length<<endl;
			a1->length=a2->length+1;
			a1->nexta=a2;
		}
	}
    //if c is even and possible, go down and right
    if((c%2==0)&&(r!=(int)grid.size()-1)&&(c!=(int)grid[0].size())&&abs(val-grid[r+1][c+1])==1){
       a2=MaxPathLen(r+1, c+1);
       if(a2->length+1>a1->length){
            //cout<<a2->length<<endl;
			a1->length=a2->length+1;
			a1->nexta=a2;
		}
    }
    //If c is even and its possible, go down and left
    if((c%2==0)&&(r!=(int)grid.size()-1)&&(c!=0)&&abs(val-grid[r+1][c-1])==1){
       a2=MaxPathLen(r+1,c-1);
       if(a2->length+1>a1->length){
            //cout<<a2->length<<endl;
			a1->length=a2->length+1;
			a1->nexta=a2;
		}
    }
    //if c is odd and its possible, go up and right
    if((c%2!=0)&&(r!=0)&&(c!=(int)grid[0].size()-1)&&abs(val-grid[r-1][c+1])==1){
       a2=MaxPathLen(r-1,c+1);
       if(a2->length+1>a1->length){
            //cout<<a2->length<<endl;
			a1->length=a2->length+1;
			a1->nexta=a2;
		}
    }
    //if c is odd and its possible, go up and left
    if((c%2!=0)&&(r!=0)&&(c!=0)&&abs(val-grid[r-1][c-1])==1){
       a2=MaxPathLen(r-1,c-1);
       if(a2->length+1>a1->length){
            //cout<<a2->length<<endl;
			a1->length=a2->length+1;
			a1->nexta=a2;
		}
    }
	grid[r][c]=(char)val;//put the original cell back in the graph
	//add to cache and return
	cache[tmpid]=a1;
	return a1;
}

int main(){
	Answer *a=new Answer;
	a->length=0;
	Answer *a2=new Answer;
	a2->length=0;
	SpellSeeker s;
	string line;//line of file
	//start reading input line by line
	while(getline(cin,line))
		s.grid.push_back(line);//push each line onto the grid
	//set up spellseeker values
	s.total_c=s.grid[0].size();
	s.total_r=s.grid.size();
	//call MaxPathlen on all cells and update max if necessary-- print along the way
	for(int i=0;i<s.total_r;i++){
		for(int j=0;j<s.total_c;j++){
			cout<<s.grid[i][j];
			a=s.MaxPathLen(i,j);
			if(a->length>a2->length)
				a2=a;
		}
		cout<<endl;
	}
	//output max path by following the next pointers in each answer
	cout<<"PATH"<<endl;
	while(a2!=NULL){
		cout<<a2->r<<' '<<a2->c<<endl;
		a2=a2->nexta;
	}
	delete a;
    delete a2;
	return 0;
}

