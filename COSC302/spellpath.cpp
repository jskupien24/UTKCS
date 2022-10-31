/*
 * Jack Skupien
 * 5/5/2022
 * CS302 - Dynamic Programming
 * Lab AB - Spellpath
 */
#include<iostream>
#include<vector>
using namespace std;

int MaxPathLen(int r,int c, vector<string> grid){//recursive function that returns the longest path that can stem from a cell 
	int maxpath=0;
	vector<int> adjpaths;//holds the longest path of the 6 possible adjacent cells
	for(int i=0;i<6;i++)//initialize each path to 1
		adjpaths.push_back(1);
	//store cell val and remove from grid
	int val=(int)(grid[r][c]);
	grid[r][c]='-';
	//start checking if each of the 6 possible adjacent cells are within 1 of the current cell
	//if not at top and above cell is still in grid, see if above cell is within 1
	if((r!=0)&&(grid[r-1][c]!='-')&&(((int)grid[r-1][c]==val-1)||((int)grid[r-1][c]==val+1)))
		adjpaths[0]=MaxPathLen(r-1,c,grid)+1;
	//if not at bottom and below cell is still in grid, see if below cell is within 1
	if((r!=(int)grid.size()-1)&&(grid[r+1][c]!='-')&&(((int)grid[r+1][c]==val-1)||((int)grid[r+1][c]==val+1)))
		adjpaths[1]=MaxPathLen(r+1,c,grid)+1;
	//if not all the way left and left cell is still in grid, see if left cell is within 1
	if((c!=0)&&(grid[r][c-1]!='-')&&(((int)grid[r][c-1]==val-1)||((int)grid[r][c-1]==val+1)))
		adjpaths[2]=MaxPathLen(r,c-1,grid)+1;
	//if not all the way right and right cell is still in grid, see if right cell is within 1
	if((c!=(int)grid[0].size()-1)&&(grid[r][c+1]!='-')&&(((int)grid[r][c+1]==val-1)||((int)grid[r][c+1]==val+1)))
		adjpaths[3]=MaxPathLen(r,c+1,grid)+1;
	//if c is even 
	if(c%2==0){
		//if cell is not all the way down or right, and below/right cell is still in grid, see if below/right cell is within 1
		if((r!=(int)grid.size()-1)&&(c!=(int)grid[0].size()-1)&&(grid[r+1][c+1]!='-')&&(((int)grid[r+1][c+1]==val-1)||((int)grid[r+1][c+1]==val+1)))
			adjpaths[4]=MaxPathLen(r+1,c+1,grid)+1;
		//if cell is not all the way down or left, and below/left cell is still in grid, see if below/left cell is within 1
		if((r!=(int)grid.size()-1)&&(c!=0)&&(grid[r+1][c-1]!='-')&&(((int)grid[r+1][c-1]==val-1)||((int)grid[r+1][c-1]==val+1)))
			adjpaths[5]=MaxPathLen(r+1,c-1,grid)+1;
	}else{//if odd
		//if cell is not all the way up or right, and above/right cell is still in grid, see if above/right cell is within 1
		if((r!=0)&&(c!=(int)grid[0].size()-1)&&(grid[r-1][c+1]!='-')&&(((int)grid[r-1][c+1]==val-1)||((int)grid[r-1][c+1]==val+1)))
			adjpaths[4]=MaxPathLen(r-1,c+1,grid)+1;
		//if cell is not all the way up or left, and above/leftt cell is still in grid, see if above/left cell is within 1
		if((r!=0)&&(c!=0)&&(grid[r-1][c-1]!='-')&&(((int)grid[r-1][c-1]==val-1)||((int)grid[r-1][c-1]==val+1)))
			adjpaths[5]=MaxPathLen(r-1,c-1,grid)+1;
	}
	grid[r][c]=(char)val;//put the original cell back in the graph
	//see which path was the longest and give it to return val
	for(int i=0;i<6;i++)
		if(adjpaths[i]>maxpath)
			maxpath=adjpaths[i];
	return maxpath;
}

int main(){
	vector<string> grid1;//full grid
	string line;//line of file
	int max=0,tmp=0;
	//start reading input line by line
	while(getline(cin,line))
		grid1.push_back(line);//push each line onto the grid
	//call MaxPathlen on all cells and update max if necessary
	for(size_t i=0;i<grid1.size();i++)
		for(size_t j=0;j<grid1[0].size();j++){
			tmp=MaxPathLen(i,j,grid1);
			if(tmp>max)
				max=tmp;
		}
	//output max path length (caculated by recursive function)
	cout<<max<<endl;
	return 0;
}
