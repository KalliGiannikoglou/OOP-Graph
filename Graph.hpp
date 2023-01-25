#ifndef _GRAPH_HPP_ 
#define _GRAPH_HPP_

#include <list>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

template<typename T>
struct Edge {
	T from;
	T to;
	int dist;
	Edge(T f, T t, int d): from(f), to(t), dist(d) { }
	// bool operator<(const Edge<T>& e) const;
	// bool operator>(const Edge<T>& e) const;
	template<typename U>
	friend std::ostream& operator<<(std::ostream& out, const Edge<U>& e);

	bool operator<(const Edge<T>& e) const{
		return (this->dist < e->dist);
	}
	bool operator>(const Edge<T>& e) const{
		return (this->dist > e->dist);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& e) {
	out << e.from << " -- " << e.to << " (" << e.dist << ")";
	return out;
}

template <typename T>
class Graph {
	public:
		vector< vector<T> > adjLists;
		bool isDirected;
		vector< Edge<T> > edges;

	public:
		Graph(bool isDirectedGraph = true);
		~Graph();
		bool contains(const T& info);
		bool addVtx(const T& info);
		bool rmvVtx(const T& info);
		bool addEdg(const T& from, const T& to, int distance);
		bool rmvEdg(const T& from, const T& to);
		list<T> dfs(const T& info) const;
		list<T> bfs(const T& info) const;
		list<Edge<T>> mst();
		
		void print2DotFile(const char *filename) const;
		list<T> dijkstra(const T& from, const T& to);

		//help functions
		void printAdjList();
		int containsEdg(int pos, const T& info) const;
		int findPos(const T& info) const;
		int findEdg( const T& info) const;
		void rmvStruct(const T& from, const T& to);
		bool isVisited(vector<bool> vect, const T& info) const;
		tuple< list<T>, vector<bool>> recursDFS( tuple< list<T>, vector<bool>> recursTuple, int pos) const;
		int edgCost(const T& from, const T& to);
		void printMst(vector<Edge<T>> mstList);
};

template <typename T>
Graph<T>:: Graph(bool isDirectedGraph){

	this->isDirected = isDirectedGraph;
}

template <typename T>
Graph<T>:: ~Graph(){ }


template <typename T>
void Graph<T>::printAdjList(){

	for(int i = 0; i < (int)adjLists.size(); i++){
		for(int j = 0; j < (int)adjLists[i].size(); j++){
			cout << adjLists[i][j] << " ->" ;
		}
		cout << endl;
	}
}

template <typename T>
bool Graph<T>:: contains(const T& info){
	//iterate through the list
  	for(int i = 0; i < (int)adjLists.size(); i++){
		//if info exists already the list contains the object
		if(adjLists[i][0] == info){
			return true;
		}
  	}
	return false;
}

//return the pos of info in AdjList
template <typename T>
int Graph<T>:: findPos(const T& info) const{
	//iterate through the list
  	for(int i = 0; i < (int)adjLists.size(); i++){
		//if info exists already the list contains the object
		if(adjLists[i][0] == info){
			return i;
		}
  	}
	return -1;
}

//check if theres an edge between i and info
template <typename T>
int Graph<T>:: containsEdg(int pos, const T& info) const{

	//iterate through a specific pos of outer vector 
  	for(int i = 0; i < (int)adjLists[pos].size(); i++){
		//if info exists already the list contains the object
		if(adjLists[pos][i] == info){
			return i;
		}
  	}
	return -1;
}


//match info with its position in adjLists
template <typename T>
int Graph<T>:: findEdg( const T& info) const{
	
	//iterate through a specific pos of outer vector 
  	for(int i = 0; i < (int)adjLists.size(); i++){
		//if info exists already the list contains the object
		if(adjLists[i][0] == info){
			return i;
		}
  	}
	return -1;
}


template <typename T>
bool Graph<T>::addVtx(const T& info){

	//check if info is already contained
	if(contains(info) == true){
	 	return false;
	}
	//create a new node and add it to the outer vector
	vector<T> newVtx(1, info);
	adjLists.push_back(newVtx);
	return true;
}

template <typename T>
bool Graph<T>::rmvVtx(const T& info){
	
	//check if info is already contained
	if(contains(info) == false){
	 	return false;
	}

	for(int i = 0; i < (int)adjLists.size(); i++){
		//erase the links 'info' has with other vtx
		for(int j = 1; j < (int)adjLists[i].size(); j++){
			if(adjLists[i][j] == info){
				//erase all the instances of this node 
				adjLists[i].erase( adjLists[i].begin() + j);
			}
		}
		
		//erase all the links starting from info
		if(adjLists[i][0] == info){
			for(int j = 1; j < (int)adjLists[i].size(); j++){
				adjLists[i].erase( adjLists[i].begin() + j);
			}
			//erase the vertex
			adjLists.erase(adjLists.begin() + i);
			i--;
		}
	}

	for(int i=0; i< (int)edges.size(); i++){
		if(edges[i].from == info || edges[i].to == info){
			edges.erase( edges.begin() + i);
			i--;
		}
	}

	return true;
}


template <typename T>
bool Graph<T>::addEdg(const T& from, const T& to, int distance){

	//check if 'from' AND 'to' exist in the graph and the edge does not already exist (edge cost != 9999)
	if(contains(from) == false || contains(to) == false || edgCost(from, to) != 9999){
	 	return false;
	}

	//ADD: from->to, if it is not contained
  	for(int i = 0; i < (int)adjLists.size(); i++){
		//if we are in 'from' position AND 'to' is not already in the vector
		if(adjLists[i][0] == from && containsEdg(i, to) == -1){
			
			int posTo = findPos(to);
			int j;
			for(j=1; j < (int) adjLists[i].size(); j++){
				int posJ = findPos(adjLists[i][j]);
				if(posJ > posTo){
					
					adjLists[i].insert(adjLists[i].begin()+j, to);
					break;
				}
			}
			//if we have checked all the existing, we add at the end
			if((int) adjLists[i].size() == 1 || j == (int) adjLists[i].size()){
				adjLists[i].push_back(to);
			}
			//add 'to' as a new node in 'from' list
			Edge<T> newEdg(from, to, distance);
			edges.push_back(newEdg);
			break;
		}
  	}

	//if it is not a digraph, add in 'from' list the 'to' node  
	if(isDirected == false){
		//ADD: to->from, if it is not contained
		for(int k = 0; k < (int)adjLists.size(); k++){

			if(adjLists[k][0] == to && containsEdg(k, from) == -1){
				int posFrom = findPos(from);
				int l;
				
				for(l=1; l < (int) adjLists[k].size(); l++){
					int posL = findPos(adjLists[k][l]);
					if(posL > posFrom){

						adjLists[k].insert(adjLists[k].begin()+l, from);
						break;
					}
				}
				//if we have checked all the existing, we add at the end
				if((int) adjLists[k].size() == 1 || l == (int) adjLists[k].size()){
					adjLists[k].push_back(from);
				}
				//add 'to' as a new node in 'from' list
				Edge<T> newEdg2(to, from, distance);
				edges.push_back(newEdg2);
				break;
			}
  		}
	}
	return true; 
}


template <typename T>
void Graph<T>::rmvStruct(const T& from, const T& to){

	for(int i=0; i < (int)edges.size(); i++){
		if(edges[i].from == from && edges[i].to == to){
			edges.erase( edges.begin() + i);
		}
	}
}

template <typename T>
bool Graph<T>::rmvEdg(const T& from, const T& to){

	//check if 'from' AND 'to' exist in the graph 
	if(contains(from) == false || contains(to) == false){
	 	return false;
	}

	//RMV: from->to, if it is contained
  	for(int i = 0; i < (int)adjLists.size(); i++){
		int res	= containsEdg(i, to); 
		//if we are in 'from' position AND 'to' is in the vector
		if(adjLists[i][0] == from && res != -1){
			//erase
			adjLists[i].erase( adjLists[i].begin() + res);
			rmvStruct(from, to);
		}
  	}
	//RMV: from->to, if it is contained
  	for(int i = 0; i < (int)adjLists.size(); i++){
		int res	= containsEdg(i, from); 
		//if we are in 'to' position AND 'from' is in the vector
		if(adjLists[i][0] == to && res != -1){
			//erase
			adjLists[i].erase( adjLists[i].begin() + res);
			rmvStruct(to, from);
		}
  	}

	return true;
}

template <typename T>
list<T> Graph<T>::dfs(const T& info) const{
	int start;
	tuple< list<T>, vector<bool>> recursTuple;

	for(int i=0; i< (int) adjLists.size(); i++){
		//add all the existing nodes to the visited Vector
		get<1>(recursTuple).push_back(false);
	
		//find the position of the start of DFS
		if(adjLists[i][0] == info){
			start = i;
		}
	}
	recursTuple = recursDFS( recursTuple, start);

	//print the list by poping the front elements
	while(!get<0>(recursTuple).empty()){
		cout << get<0>(recursTuple).front();
		get<0>(recursTuple).pop_front();
		if(!get<0>(recursTuple).empty()){
			cout << " -> ";
		}
	}

	return get<0>(recursTuple);		
}


template <typename T>
tuple< list<T>, vector<bool>> Graph<T>::recursDFS( tuple< list<T>, vector<bool>> recursTuple, int pos) const{

	//boolean vector
	get<1>(recursTuple)[pos] = true;
	//dfsList
	get<0>(recursTuple).push_back(adjLists[pos][0]);
	
	for(int i = 1; i < (int) adjLists[pos].size(); i++){
		if(isVisited(get<1>(recursTuple), adjLists[pos][i]) == false){
			int next = findEdg(adjLists[pos][i]);
			recursTuple = recursDFS( recursTuple, next);
		}
	}
	return recursTuple;
}


template <typename T>
list<T> Graph<T>::bfs(const T& info) const{
	vector<bool> visited, enqueued;
	int start;
	list<T> bfsList;
	vector<T> queue;

	for(int i=0; i< (int) adjLists.size(); i++){
		//init visited and enqueued to 'false'
		visited.push_back(false);
		enqueued.push_back(false);

		//find the position of the start of DFS
		if(adjLists[i][0] == info){
			start = i;
			queue.push_back(adjLists[i][0]);
		}
	}

	int res;
	while(!queue.empty()){
		start = findPos(queue[0]);
		visited[start] = true;
		bfsList.push_back(adjLists[start][0]);
		//cout << adjLists[start][0] << " -> ";


		for(int j=1; j < (int)adjLists[start].size(); j++){
			
			res = findPos(adjLists[start][j]);
			if(!visited[res] && !enqueued[res]){
				queue.push_back(adjLists[res][0]);
				enqueued[res] = true;
			}
		}
		queue.erase( queue.begin());
	}
	//print the list by poping the front elements
	while(!bfsList.empty()){
		cout << bfsList.front();
		bfsList.pop_front();
		if(!bfsList.empty()){
			cout << " -> ";
		}
	}

	return bfsList;
}

//Prim's algo for minimum spanning tree
template <typename T>
list<Edge<T>> Graph<T>::mst(){
	vector<bool> visited;
	vector <Edge<T>> queue, mstVect;
	T curr = adjLists[0][0];


	if(isDirected == true){
		list<Edge<T>> mstList(mstVect.begin(), mstVect.end());
		return mstList;
	}

	for(int i=0; i< (int) adjLists.size(); i++){
		//init visited to 'false'
		visited.push_back(false);
	}


	while(mstVect.size() < adjLists.size()-1){
		int pos = findPos(curr);
		//mark curr Vtx as 'visited'
		visited[pos] = true;
		//for all the adjacents of curr
		for(int j=1; j < (int)adjLists[pos].size(); j++){
			//search the from-to edge in the edges array
			for(int k=0; k < (int) edges.size(); k++){
				if(edges[k].from == adjLists[pos][0] &&  edges[k].to == adjLists[pos][j]){
					//add in the queue 
					queue.push_back(edges[k]);
					break;
				}
			}
		}

		int minCost = INT32_MAX;
		T f, t;
		//search the queue for the min cost edge
		for(int i = 0; i<(int) queue.size(); i++){
			if(queue[i].dist < minCost && !visited[findPos(queue[i].to)]){
				minCost = queue[i].dist;
				f = queue[i].from;
				t = queue[i].to;
			}
		}
		Edge<T> minEdge(f, t, minCost);
		mstVect.push_back(minEdge);
		curr = minEdge.to;
		
	}
	printMst(mstVect);
	list<Edge<T>> mstList(mstVect.begin(), mstVect.end());
	
	return mstList;
}

template <typename T>
void Graph<T>::printMst(vector<Edge<T>> mstList){
	int minCost,totalCost=0, pos;

	while(!mstList.empty()){
		minCost = mstList[0].dist;
		pos = 0;
		//find the edge with the minimum cost 
		for(int i=0; i < (int) mstList.size(); i++){
			if( mstList[i].dist < minCost){
				pos = i;
				minCost =  mstList[i].dist;
			}
		}
		int posA = findPos( mstList[pos].from);
		int posB = findPos( mstList[pos].to);
		//print the vertexes in insertion order
		if(posA < posB){
			cout <<  mstList[pos].from << " -- " <<  mstList[pos].to << " (" <<  mstList[pos].dist << ")\n";
		}
		else{
			cout << mstList[pos].to << " -- " <<  mstList[pos].from << " (" <<  mstList[pos].dist << ")\n";
		}

		totalCost +=  mstList[pos].dist;
		//remove from the list
		mstList.erase(  mstList.begin() + pos);

	}
	cout << "MST Cost: " << totalCost << endl;
}


template <typename T>
list<T> Graph<T>::dijkstra(const T& from, const T& to){
	vector<int> prev;	//keep the position of the previous node
	vector<int> cost;	//keep the cost from the start up to that vtx
	list<T> dijList;
	list<int> queue;	//next vtx to visit
	vector<bool> visited;	

	int fromPos = findPos(from);
	//initiate costs to "inf" and prev positions to -1
	for(int i=0; i< (int)adjLists.size(); i++){
		cost.push_back(9999);
		if(i == fromPos){
			cost[i] = 0;
		}
		prev.push_back(-1);
		visited.push_back(false);
	}

	int i;
	//starting with 'from' vtx
	queue.push_back(fromPos);

	while(!queue.empty()){
		//get next node from priority queue
		i = queue.front();
		queue.pop_front();
		visited[i] = true;

		for(int j=1; j< (int)adjLists[i].size(); j++){
			//pos of curr vtx
			int posAdj = findPos(adjLists[i][j]); 
			int newCost;
			//9999 is the init value and not a real cost 
			if(cost[i] != 9999){ 
				//newCost = edgCost + curr node's cost
				newCost = edgCost(adjLists[i][0], adjLists[i][j]) + cost[i];
			}
			else{	
				newCost = edgCost(adjLists[i][0], adjLists[i][j]);
			}
			
			if( newCost < cost[posAdj] ){
				//update with min cost
				cost[posAdj] = newCost;
				//keep the previous node 
				prev[posAdj] = i;
			}
			//if the node is not visited yet, add it to the queue
			if(!visited[posAdj]){
				queue.push_back(posAdj);
			}
		}
		
	}

	int k =  findPos(to);
	//form the list according to "prev" vector that keeps the connection between nodes
	while(k != -1){
		dijList.push_front(adjLists[k][0]);
		k = prev[k];
	}
	
	//if "to" is not connected with "from" return an empty list
	if((int)dijList.size() == 1){
		cout <<  endl;
		return dijList;
	}

	//print the list by poping the front elements
	while(!dijList.empty()){
		cout << dijList.front();
		dijList.pop_front();
		if(!dijList.empty()){
			cout << ", ";
		}
		else{
			cout << endl;
		}
	}
	return dijList;
}


template <typename T>
int Graph<T>::edgCost(const T& from, const T& to){

	for(int i=0; i< (int) edges.size(); i++){
		if(edges[i].from == from && edges[i].to == to){
			return edges[i].dist;
		}
	}
	return 9999;
}


//returns true if info is not in the vector, so it is already visited
template <typename T>
bool Graph<T>::isVisited(const vector<bool> vect, const T& info) const{

	for(int i=0; i< (int)vect.size(); i++){
		if(adjLists[i][0] == info && vect[i] == false){
			return false;
		}
	}
	return true;
}


#endif