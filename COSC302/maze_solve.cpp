/*
 * Jack Skupien
 * CS302
 * 4/1/22
 * Lab 7 - maze_solve
 */
#include<iostream>
#include<vector>
using namespace std;

class Node{
	public:
		Node(int id1);//constructor
		int id;
		bool visited;//becomes true if node has already been visited in DFS
		vector<int> AL;//adjacency list
};

class Graph{
	public:
		~Graph();//destructor
		void build_maze();//reads in maze and walls
		bool DFS(int index);//recursive DFS
		int r,c;//rows,columns
		vector<Node*> nodes;
		vector<int> path;//final path to exit, if possible
};

Node::Node(int id1){//constructor
	//set up id and default to not visited
	id=id1;
	visited=false;
	//set up AL vector and fill with -1 by default
	for(int i=0;i<4;i++) AL.push_back(-1);
}

Graph::~Graph(){//destructor
	//loop through, deleting each node
	for(int i=0;i<nodes.size();i++) delete nodes[i];
}

void Graph::build_maze(){
	Node *n;
	int tempid,w1,w2;//id and walls
	string rc;
	//read in rows and cols
	cin>>rc>>r>>rc>>c;
	cout<<"ROWS "<<r<<" COLS "<<c<<endl;//first line of output
	//build the maze
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++){
			//create an id for new node
			tempid=i*c+j;
			n=new Node(tempid);
			//set up each nodes adjacency list to contain nearby nodes (ternary operators ensure they don't point out of bounds)
			n->AL[0]=(i!=0)?(tempid-c):-1;
			n->AL[1]=(j!=0)?(tempid-1):-1;
			n->AL[2]=(i!=(r-1))?(tempid+c):-1;
			n->AL[3]=(j!=(c-1))?(tempid+1):-1;
			//push onto vector
			nodes.push_back(n);
			tempid=0;
		}

	//read in walls and set boundary identifiers (-1)
	while(cin>>rc>>w1>>w2){
		//print each wall and turn w1,w2 into cols and rows
		cout<<"WALL "<<w1<<" "<<w2<<endl;
		int col1=w1%c;
		int col2=w2%c;
		int row1=w1/c;
		int row2=w2/c;
		//check for each case where a wall could be (-1 indicates unavailable for DFS)
		if(row1==row2){//same row
			if(col1<col2){//w1 is left of w2
				nodes[w1]->AL[3]=-1;
				nodes[w2]->AL[1]=-1;
			}else{//w1 is not left of w2
				nodes[w1]->AL[1]=-1;
				nodes[w2]->AL[3]=-1;
			}
		}else if(col1==col2){//same column
			if(row1<row2){//w1 is above w2
				nodes[w1]->AL[2]=-1;
				nodes[w2]->AL[0]=-1;
			}else{//w1 is not above w2
				nodes[w1]->AL[0]=-1;
				nodes[w2]->AL[2]=-1;
			}
		}else if(row1>row2){
			if(col1<col2){
				nodes[w1]->AL[0]=-1;
				nodes[w2]->AL[1]=-1;
			}else if(col1>col2){
				nodes[w1]->AL[0]=-1;
				nodes[w2]->AL[3]=-1;
			}
		}else{//w1 is above w2
			if(col1<col2){//w1 is left of w2
				nodes[w1]->AL[2]=-1;
				nodes[w2]->AL[0]=-1;
			}else if(col1>col2){//w1 is not left of w2
				nodes[w1]->AL[2]=-1;
				nodes[w2]->AL[3]=-1;
			}
		}
	}
}

bool Graph::DFS(int index){
	//make a pointer to the node at parameter index
	Node *n;
	n=nodes[index];
	//push index to path (will later be removed if it returns false
	path.push_back(index);
	//base case 1: Goal reached
	if(index==(int)nodes.size()-1)
		return true;
	//base case 2: already visited
	if(n->visited==true){
		path.pop_back();//remove from path
		return false;
	}
	n->visited=true;
	//recursively call for all of the nodes in n's AL except for the one you came from
	for(int i=0;i<n->AL.size();i++)
		if(n->AL[i]!=-1)
			if(DFS(n->AL[i]))
				return true;
	path.pop_back();//remove from path, since to get here, path must be false
	return false;
}

int main(){
	Graph graph;
	graph.build_maze();
	if(graph.DFS(0))//print path
		for(int i=0;i<(graph.path.size());i++)cout<<"PATH "<<graph.path[i]<<endl;
	return 0;
}
