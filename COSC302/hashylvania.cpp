/*
 * Jack Skupien
 * 02/07/2022
 * Lab 2 - Hashylvania
 */

#include<iostream>
#include<sstream>
#include<map>
#include<vector>

using namespace std;

struct Person{
	public:
		string name;
		string id;
};

unsigned long long jsp_hash(const string &s){
	size_t i;
	unsigned long long h, t;

	h = 0x6bd300f29f1505ULL;

	for (i = 0; i < s.size(); i++) {
		t = h & 0x3ffffffffffffffULL;
		h = (t << 5) ^ h ^ s[i];
	}
	return h;
};

int main(){
	//variable declaration
	stringstream ss;//for hex
	string hexa,conc,s1,s2;
	vector<Person* > pop;//population
	map<unsigned long long,map<unsigned long long, Person* > > map1;
	map<unsigned long long,Person*>::iterator it1;//iterator for inner
	map<unsigned long long,map<unsigned long long, Person* > >::iterator it2, upper, lower;//iterator for the outer and bounds
	unsigned long long hash1,hash2,mod;

	//start reading and check first word
	while(cin>>s1){
		if(s1=="PERSON"){
			//if person is detected, make a new instance of person and assign it the value read in
			Person* tmp=new Person;
			cin>>tmp->name;
			cin>>tmp->id;
			//add to vector
			pop.push_back(tmp);
		}else{//if order
			//read rand string and mod
			cin>>s2;
			cin>>mod;
			//concatenate name, ids, and random strings for hash value
			for(size_t i=0;i<pop.size();i++){
				conc=pop[i]->name+pop[i]->id+s2;
				hash1=jsp_hash(conc);
				//insert into map, accounting for what to mod by
				map1[hash1%mod][hash1]=pop[i];
			}
			//calculate second hash with random string
			hash2=jsp_hash(s2);
			pop.clear();
			while(map1.empty()!=true){//find bounds
				//outer iterator
				//assigns higher val to person closest to modded hash
				it2=map1.find(hash2%mod);
				if(it2==map1.end()){//upper
					lower=map1.lower_bound(hash2%mod);
					upper=map1.lower_bound(hash2%mod);
					//decrement lower each time the while loop runs
					/*while*/if(lower!=map1.begin())
						lower--;
					//figure out order
					if(upper->first-(hash2%mod)>(hash2%mod)-lower->first)
						it2=lower;
					else
						it2=upper;
				}
				//avoid seg fault due to bounding
				if(it2==map1.end())
					it2--;

				//inner iterator
				it1=it2->second.lower_bound(hash2);//tries to get hash value after hash1
				
				if(it1==it2->second.end())//if it can't find it, it assigns largest hash
					it1--;
				pop.push_back(it1->second);//pushes onto vector of people

				//convert to hex with stringstream
				ss.str("");
				ss<<hex<<hash2;
				hexa=ss.str();
				hash2=jsp_hash(it1->second->id+hexa);

				//delete maps
				it2->second.erase(it1);
				if(it2->second.empty())
					map1.erase(it2);
			}
			//print
			cout<<"Last: "<<pop.back()->name<<" "<<pop.back()->id<<endl;
			cout<<"First: "<<pop.front()->name<<" "<<pop.front()->id<<endl;
		}
	}
	return 0;
}
