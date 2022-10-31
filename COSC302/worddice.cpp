/*
 * Jack Skupien
 * 4/19/22
 * CS302 - Network Flow
 * Lab 9 - Letter Dice
 */
#include <iostream>
#include <vector>
#include <deque>//so we can pop front in BFS
#include <fstream>//for reading file
using namespace std;

class Node{
	public: 
		Node(string n, int num);		//constructor
		
		string name;					//string ID (letter for word nodes, "source," "sink," etc.)
		int ID;							//number ID 
		vector <class Edge*> al;		//adjacency list
		int maxflow;
		bool visited;					//to save a node from being visited twice in BFS
		Edge *back;
};

Node::Node(string n,int num){
	name=n;
	ID=num;
}

class Edge{
	public:
		Edge(Node *f, Node *t, bool og);		//constructor
		Node *to;								//node of destination
		Node *from;								//node of origin
		Edge *rev;								//reverse edge
		bool possible;							//works similarly to "original" from 2017 video on writeup
};

Edge::Edge(Node *f, Node *t, bool og){
	from=f;
	to=t;
	possible=og;
}

class Graph{
	public:
		~Graph();  						//destructor (delete all nodes and edges)
		void ClearGraph();              //resets graph before new iteration of each while loop in main
		int BFS();						//Breadth-first search
		void canISpell();				//Inspired by the video from 2017, calls BFS repeatedly and determines if the word can be spelled

		vector<Node*> nodes; 
		vector<Edge*> edges;
		string word;					//the word you're attempting to spell
		vector<Edge*> wordedges; 		//helps us determine whether the word can be spelled or not
		vector<Node*> letters;			//keeps track of nodes that are letters (for output) 			
		Node *source;
		Node *sink;
};

Graph::~Graph(){
	for(size_t i=0;i<nodes.size();i++)
		delete nodes[i];
	for(size_t i=0;i<edges.size();i++)
		delete edges[i];
}

void Graph::ClearGraph(){
	//clear everything
	source->al.clear();
	wordedges.clear();
	letters.clear();
	nodes.clear();
	nodes.push_back(source);//since it's cleared we're gonna have to put source back
}

int Graph::BFS(){
	Node *n, *n2;
	
	deque<Node*> allnodes;//I used a dequeue to allow me to pop front
	deque<Node*>::iterator it;

	//reset everything since this method will be called back to back
	for(size_t i=0;i<nodes.size();i++){
		nodes[i]->back=NULL;
		nodes[i]->visited=false;
		nodes[i]->maxflow=-1;
	}

	allnodes.push_back(source);
	
	while(allnodes.size()>0){//loops through until everything has been popped, including what is pushed on
		it=allnodes.begin();
		n=*it;//save front before it's popped
		allnodes.pop_front();
		for(size_t i=0;i<n->al.size();i++){//loop through adjacency list
			n2=n->al[i]->to;
			if(n->al[i]->possible&&!(n2->visited)){//follow path if possible and unvisited
				n->visited=true;//set to visited
				n2->back=n->al[i];
				n2->maxflow++;//iterate maxflow since were adding to path
				allnodes.push_back(n->al[i]->to);//push onto queue
			}
		}
	}
	return sink->maxflow;
}

void Graph::canISpell(){
	while(BFS()!=-1){
		Edge *e=sink->back;//set to end
		while(e!=NULL){//while not at the beginning, go back and reverse possibilities of paths

			source->back=NULL;
			e->possible=false;
			e->rev->possible=true;
			e=e->from->back;
		}
	}

	//Check if the word CANNOT be spelled
	for(size_t i=0;i<wordedges.size();i++){
		if(wordedges[i]->possible){//if unable to spell, print and return 0
			cout<<"Cannot spell "<<word<<endl;
			return;
		}
	}

	//If the word CAN be spelled
	for(size_t i=0;i<letters.size();i++){//loops through word nodes
		for(size_t j=0;j<letters[i]->al.size();j++){//loops through each adjacency list
			if(letters[i]->al[j]->possible){//if the possible is 1 that means it's being used, so print it out
				if(i!=0&&i!=word.length())//ensures format matches writeup
					cout<<',';
				cout<<letters[i]->al[j]->to->ID;
				if(i==word.length()-1)//ensures format matches writeup
					cout<<": "<<word<<endl;
			}
		}
	}
}

int main(int argc,char *argv[]){
	if(argc!=3)//make sure correct amount of arguments
		return -1;
	
	vector<Node*> dice;
	vector<Node*> Letters;
	ifstream read;//to read from files
	string s;//holds a line of input temporarily

	Graph g;//graph to hold everything
	Node *n;//for creating and pushing all nodes
	Edge *e,*re;//tmp edges too create and push all edges and reverse edges
	int it=0;//iterator for IDs

	//start by creating a source node and pushing it onto graph
	n=new Node("source",0);
	g.source=n;
	g.source->back=NULL;
	g.nodes.push_back(g.source);

	//start dealing with the dice in dicefile
	read.open(argv[1]);
	//create a node for each die and push it to graph and local dice vector
	while(getline(read,s)){
		n=new Node(s,it);
		dice.push_back(n);
		it++;
	}
	read.close();

	//START DEALING WITH WORDS
	read.open(argv[2]);
	while(getline(read,s)){//while each word
		//create edges (just between source and dice before we get letter nodes
		for(size_t i=0; i < dice.size(); i++){
			g.nodes.push_back(dice[i]);
			e=new Edge(g.source,dice[i],true);//create a new edge from source to each die
			re=new Edge(dice[i],g.source,false);//create a new reverse edge from each die to source and set
			e->rev=re;
			g.source->al.push_back(e);
		}
		g.word=s;//set the subject word of the graph
		for(size_t i=0; i < s.length(); i++){//loop through each letter of s and create a node for each
			n=new Node("",it+i);//empty name for letters, since they won't be searched
			for(size_t j=0;j<dice.size();j++){//loop through dice to find if it can attach to a letter in s
				//check to see if letter exists in each die
				if(dice[j]->name.find(s[i])!=string::npos){
					e=new Edge(dice[j],n,true);//create a new edge from die to matching letter node
					re=new Edge(n,dice[j],false);//create a reverse edge from letter to matching die
					e->rev=re;
					re->rev=e;
					n->al.push_back(re);
					dice[j]->al.push_back(e);
				}
			}
			//finally, push each letter to applicable vectors
			g.nodes.push_back(n);
			Letters.push_back(n);
			g.letters.push_back(n);
		}
		//create a sink node now that the graph is full, and make edges between sink and letters
		it++;
		n=new Node("sink",it);
		g.nodes.push_back(n);
		g.sink=n;
		for(size_t i=0;i<Letters.size();i++){//loops through letters
			e=new Edge(Letters[i],n,true);//create edge from letter to sink
			g.wordedges.push_back(e);//having this separate helps with determining when a word can't be spelled
			Letters[i]->al.push_back(e);
			re=new Edge(n,Letters[i],false);//create a reverse edge from sink to letters
			e->rev=re;
			re->rev=e;
		}
		//finally, see if the word can be spelled
		g.canISpell();
		//clear graph, al of each die, and word out before running again
		g.ClearGraph();
		for(size_t i=0;i<dice.size();i++)
			dice[i]->al.clear();
		Letters.clear();
	}
	delete n;
	delete e;
	delete re;
	return 0;
}