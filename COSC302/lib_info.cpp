/*
 * Jack Skupien
 * CS302
 * 01/31/2022
 * Lab 1 - lib_info.cpp
 */

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<map>

using namespace std;

class Song{
	public:
		string title;
		int time;
		int track;
};

class Album{
	public:
		map <int, Song *> songs;
		string name;
		int time;
};

class Artist{
	public:
		map <string, Album *> albums;
		string name;
		int time;
		int nsongs;
};

void spaces(string& s){//replaces all occurences of '_' with ' '
	for(size_t i=0;i<s.size();i++)
		if(s[i]=='_')
			s[i]+=(' '-'_');
}

int main(int argc,char** argv){
	(void)argc;//to make warning about not using argc go away

	map <string, Artist *> a;
	map <string, Artist* >::iterator nit;//artist iterator
	map <string, Album* >::iterator ait;//album iterator
	map <int, Song *>::iterator sit;//song iterator

	string s="";//line of the file

	//initialize each scanned string to a char of length 100 to comply with sscanf
	char ti[100];
	char ar[100];
	char al[100];
	char ge[100];
	int min,sec,t;
	string title,artist,album;

	Song *s1;
	Artist *a1;
	Album *a2;
	//open the file
	ifstream file(argv[1]);

	while(getline(file,s)){//reads in lines of the file
		sscanf(s.c_str(), "%s %d:%d %s %s %s %d",ti,&min,&sec,ar,al,ge,&t);
		//overloading to strings and removing underscores
		title=ti;
		spaces(title);
		artist=ar;
		spaces(artist);
		album=al;
		spaces(album);
		//so there is a fresh one each time
		s1=new Song;
		a1=new Artist;
		a2=new Album;

		//create an instance of the song class
		s1->title=title;
		s1->time=(min*60)+sec;
		s1->track=t;

		//look to see if artist is already in map
		nit=a.find(artist);
		if(nit==a.end()){//if the artist is not in the map, create an instance and add to map
			a1->name=artist;
			a1->nsongs=0;
			a1->time=0;
			a.insert(make_pair(artist,a1));
		}

		//check to see if album is already in artist's map
		nit=a.find(artist);
		ait=nit->second->albums.find(album);
		if(ait==nit->second->albums.end()){//if not in album map, create and add
			a2->name=album;
			a2->time=0;
			nit->second->albums.insert(make_pair(album,a2));
		}

		//insert song
		ait=nit->second->albums.find(album);
		ait->second->songs.insert(make_pair(s1->track,s1));
		//iterate times and nsong
		nit->second->time+=s1->time;
		ait->second->time+=s1->time;
		nit->second->nsongs++;
	}
	//print
	for(nit=a.begin();nit!=a.end();nit++){//loop each artist
		cout<<nit->second->name<<": "<<nit->second->nsongs<<", "<<(nit->second->time/60)<<":";
		if((nit->second->time%60)<10)cout<<0;//check if a leading 0 is needed
		cout<<(nit->second->time%60)<<endl;
		for(ait=nit->second->albums.begin();ait!=nit->second->albums.end();ait++){//loop each album
			cout<<"        "<<ait->second->name<<": "<<ait->second->songs.size()<<", "<<(ait->second->time/60)<<":";
			if((ait->second->time%60)<10)cout<<"0";
			cout<<(ait->second->time%60)<<endl;
			for(sit=ait->second->songs.begin();sit!=ait->second->songs.end();sit++){//loop each song
				cout<<"                "<<sit->first<<". "<<sit->second->title<<": "<<(sit->second->time/60)<<":";
				if((sit->second->time%60)<10)cout<<"0";
				cout<<(sit->second->time%60)<<endl;
			}
		}
	}
	return 0;
}
