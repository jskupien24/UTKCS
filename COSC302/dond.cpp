/*
 * Jack Skupien
 * 4/27/2022
 * CS302 - Dynamic Programming
 * Lab AA - Dice or No Dice
 */
#include<iostream>
#include<sstream>//for creating unique ID for iterations in cache
#include<map>//for cache
using namespace std;

class dond{
	public:
		double calcDond(int s,int t,int lr);
		map<string,double> cache;
};

double dond::calcDond(int s,int t,int lr){
	double odds=0;//return value
	stringstream ss;
	string id="";

	if(t==1){//base case
		if(lr==-1)//If -1, return the probability of rolling t times succesfully (1 in this case)
			return 1;
		else
			return (lr==(s-1)||lr==0)?((double)(s-2)/s):((double)(s-3)/s);
	}
	//loop through for the amount of sides and call necessary recursion
	for(int i=0;i<s;i++){
		//create a unique id for the cache with t and i
		ss<<t<<' '<<i;
		id=ss.str();

		if((lr==-1)||((i!=lr-1)&&(i!=lr)&&(i!=lr+1))){//if within 1 of last roll don't count towards odds unless lr=-1
			if(cache.find(id)==cache.end())//if not already in cache, calculate recursively and add it to it
				cache[id]=(calcDond(s,t-1,i))/s;
			odds+=cache[id];
		}
		ss.clear();
	}
	return odds;
}

int main(int argc, char** argv){
	if(argc!=4)//error check # parameters
		return -1;

	dond d;
	int s=atoi(argv[1]);//number of sides
	int t=atoi(argv[2]);//number of times player must roll
	int lr=atoi(argv[3]);//last roll
	
	cout<<d.calcDond(s,t,lr)<<endl;
	return 0;
}
