/*
 * Jack Skupien
 * 04/07/2022
 * Lab 8 - city_map.cpp
 */
#include<iostream>
#include "city_map.hpp"
#include<list>
#include<set>
#include<vector>
#include<cmath>

using namespace std;

City_Map::City_Map(){//constructor
	int street=0,avenue=0,bigave=0,bigstr=0;
	double greenstr=0,greenave=0,x=0,y=0,diffx=0,diffy=0;
	Intersection *i;//for scanning and pushing
	Road_Segment *r;//for adding road to intersection's adjacencies
	//while(scanf("%d %d %lf %lf %lf %lf",&street,&avenue,&x,&y,&greenstr,&greenave)){//scanning input
	while(cin>>street>>avenue>>x>>y>>greenstr>>greenave){//scan input
		i=new Intersection;//reset intersection pointer
		//check for first
		first=(street==0&&avenue==0)?i:first;
		//give known data and some default data to pointer
		i->street=street;
		i->avenue=avenue;
		i->x=x;
		i->y=y;
		i->green[STREET]=greenstr;
		i->green[AVENUE]=greenave;
		i->best_time=-1;
		i->backedge=NULL;
		i->bfsq_ptr=bfsq.end();
		//check for last
		if(street>=bigstr&&avenue>=bigave){
			last=i;
			bigstr=street;
			bigave=avenue;
		}
		//Finally, push pointer onto all
		all.push_back(i);
	}
	//create a tmp 2d vector of Intersection pointers and fill with elements avenues and streets of all
	vector<vector<Intersection* >> v(bigave+1,vector<Intersection* >(bigstr+1,NULL));

	for(list<Intersection* >::iterator it=all.begin();it!=all.end();it++)
		v[(*it)->avenue][(*it)->street]=(*it);

	//loop through and set up road segments and adjacency lists
	for(list<Intersection* >::iterator lit=all.begin();lit!=all.end();lit++){
		//first tackle streets
		//add a road segment (east to west) to the adjacency list if: street is multiple of 5 (goes both ways) or street is even
		if((*lit)->avenue<bigave&&(((*lit)->street%5==0)||(*lit)->street%2==0)){//in case of biggest, don't go out of range
			//set up road segment values
			r=new Road_Segment;
			r->type=STREET;
			r->number=(*lit)->street;
			r->to=v[(*lit)->avenue+1][(*lit)->street];//east to west
			r->from=(*lit);//will always start at node
			//calculate distance
			diffx=(r->to->x)-(r->from->x);
			diffy=(r->to->y)-(r->from->y);
			r->distance=sqrt(diffx*diffx+diffy*diffy);//distance formula
			//push to adjacency list
			(*lit)->adj.push_back(r);
		}
		//add a road segment (west to east) to the adjacency list if: multiple of 5 (goes both ways) or street is odd
		if((*lit)->avenue>0&&(((*lit)->street%5==0)||((*lit)->street%2==1))){//makes sure ave is greater than 0, so it doesn't go off edge
			r=new Road_Segment;
			r->type=STREET;
			r->number=(*lit)->street;
			r->to=v[(*lit)->avenue-1][(*lit)->street];//west to east
			r->from=(*lit);
			diffx=(r->to->x)-(r->from->x);
			diffy=(r->to->y)-(r->from->y);
			r->distance=sqrt(diffx*diffx+diffy*diffy);
			(*lit)->adj.push_back(r);
		}

		//start dealing with road segments of type AVENUE
		//add road segment going south to north to adjaceny list if: mult of 5 or biggest (goes both ways) or if avenue is even
		if(((*lit)->street<bigstr)&&((*lit)->avenue%5==0||(*lit)->avenue%2==0||(*lit)->avenue==bigave)){//makes sure street isn't biggest, so segment doesn't go out of range
			r=new Road_Segment;
			r->type=AVENUE;
			r->number=(*lit)->avenue;
			r->to=v[(*lit)->avenue][(*lit)->street+1];
			r->from=(*lit);
			diffx=(r->to->x)-(r->from->x);
			diffy=(r->to->y)-(r->from->y);
			r->distance=sqrt(diffx*diffx+diffy*diffy);
			(*lit)->adj.push_back(r);
		}
		//add a road segment going north to south to adjacency list if: mult of 5 or biggest (goes both ways) or if avenue is odd
		if(((*lit)->street>0)&&((*lit)->avenue%5==0||(*lit)->avenue==bigave||(*lit)->avenue%2==1)){//makes sure road segment doesn't go out of range of edge
			r=new Road_Segment;
			r->type=AVENUE;
			r->number=(*lit)->avenue;
			r->to=v[(*lit)->avenue][(*lit)->street-1];
			r->from=(*lit);
			diffx=(r->to->x)-(r->from->x);
			diffy=(r->to->y)-(r->from->y);
			r->distance=sqrt(diffx*diffx+diffy*diffy);
			(*lit)->adj.push_back(r);
		}
	}
}



double City_Map::Dijkstra(int avg_best_worst){//Dijkstra's shortest path algorithm from node first to node last
	double tmp=0,div=0,time=0,d=0;//for calculating time/distance
	const double mph=30,s=3600;//speed limit, seconds in an hour
	bool v=false;//visited
	first->best_time=0;//default
	first->bfsq_ptr=bfsq.insert(pair<double, Intersection*>(0,first));//adds first to bfsq
	Intersection *i,*i2,*i3;//temps for while loop
	list<Road_Segment* >::iterator rit;//for adjacency lists
	while(bfsq.size()!=0){
		//check for base case (we've reached the end)
		if((bfsq.begin()->second)==last){
			bfsq.erase(bfsq.begin());//remove smallest node
			//make path w/ backedges and return
			i3=last;
			while(i3->backedge!=NULL){
				path.push_front(i3->backedge);
				i3=i3->backedge->from;
			}
			return last->best_time;
		}else{//if we haven't reached base case, keep going
			i=bfsq.begin()->second;//"pops" smallest time node, then delete it from map
			bfsq.erase(bfsq.begin());
			rit=i->adj.begin();
			
			while(rit!=i->adj.end()){//loop through adjacency list
				d=(*rit)->distance;
				i2=(*rit)->to;//get the intersection it's pointing to
				v=i2->best_time!=-1;
				tmp=(d/mph)*s;//default time before accounting for avg_best_worst
				
				switch(avg_best_worst){//check avg_best_worst
					case 'A'://'A' for avg
						//time=R2/(2(R+G)) seconds
						div=(2*(i2->green[STREET]+i2->green[AVENUE]));//divisor
						tmp+=((*rit)->type==STREET)?(pow(i2->green[AVENUE],2))/div:(pow(i2->green[STREET],2))/div;//ternary operator that sets time based on type STREET or AVENUE
						break;

					case 'B'://'B' for best (default time)
						break;

					case 'W'://'W' for worst
						tmp+=((*rit)->type==STREET)?(i2->green[AVENUE]):(i2->green[STREET]);//add time at light (opposite of green time going other way)
						break;
				}
				//add time to previous best time
				time=(tmp+(i->best_time));
				if(!v||i2->best_time>time){//if not visited or or no longer best time
					if(v)//delete pointer if already visited
						bfsq.erase(i2->bfsq_ptr);
					i2->backedge=(*rit);
					i2->best_time=time;//give new best time
					//push to bfsq
					i2->bfsq_ptr=bfsq.insert(pair<double,Intersection*>(time,i2));
				}
				rit++;//iterate
			}
		}
	}
	return -1;//shouldn't reach this part if while goes well
}
