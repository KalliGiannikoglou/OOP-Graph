
#ifndef _GRAPH_UI_
#define _GRAPH_UI_

template <typename T>
int graphUI() {
  
	string option, line;
	int distance;
	bool digraph = false;
	
	cin >> option;
	if(!option.compare("digraph"))
		digraph = true;
	Graph<T> g(digraph);
	
	while(true) {
    
		std::stringstream stream;
		cin >> option;
		
		if(!option.compare("av")) {
			getline(std::cin, line);
			stream << line;
			T vtx(stream);
			if(g.addVtx(vtx))
				cout << "av " << vtx << " OK\n";
			else
				cout << "av " << vtx << " NOK\n";
		}
		else if(!option.compare("rv")) {
			getline(std::cin, line);
			stream << line;
			T vtx(stream);
			if(g.rmvVtx(vtx))
				cout << "rv " << vtx << " OK\n";
			else
				cout << "rv " << vtx << " NOK\n";
		}
		else if(!option.compare("ae")) {
			getline(std::cin, line);
			stream << line;
			T f(stream);
			T t(stream);
			string word;
			stream >> word;
			distance = stoi(word);
			if(g.addEdg(f, t, distance))
				cout << "ae " << f << " " << t << " OK\n";
			else
				cout << "ae " << f << " " << t << " NOK\n";
		}
		else if(!option.compare("re")) {
		 	getline(std::cin, line);
		 	stream << line;
		 	T f(stream);
			T t(stream);
		 	if(g.rmvEdg(f, t))
		 		cout << "re " << f << " " << t << " OK\n";
		 	else
		 		cout << "re " << f << " " << t << " NOK\n";
		}
		// else if(!option.compare("dot")) {
		
		// }
		else if(!option.compare("bfs")) {
		
		 	cout << "\n----- BFS Traversal -----\n";
			getline(std::cin, line);
		 	stream << line;
		 	T info(stream);
			g.bfs(info);	
		 	cout << "\n-------------------------\n";
		}
		else if(!option.compare("dfs")) {
			
		 	cout << "\n----- DFS Traversal -----\n";
			getline(std::cin, line);
		 	stream << line;
		 	T info(stream);
			g.dfs(info);	
		 	cout << "\n-------------------------\n";
		}
		else if(!option.compare("dijkstra")) {
		 	getline(std::cin, line);
		 	stream << line;
		 	T from(stream);
		 	T to(stream);
		 	cout << "Dijkstra (" << from << " - " << to <<"): ";
			g.dijkstra(from, to);
		
		}
		else if(!option.compare("mst")) {

			cout << "\n--- Min Spanning Tree ---\n";
			g.mst();
		}
		else if(!option.compare("q")) {
			cerr << "bye bye...\n";
			return 0;
		}
		else if(!option.compare("#")) {
	 	string line;
	 	getline(cin,line);
	 	cerr << "Skipping line: " << line << endl;
		}
		else {
			cout << "INPUT ERROR\n";
		 	return -1;
		}
	}
  	return -1;  
}

#endif
