#include <iostream>
#include <set>
#include <string>
#include <unordered_set>

using namespace std;

int readInt () {
	bool minus = false;
	int result = 0;
	char ch;
	ch = getchar();
	while (true) {
		if (ch == '-') break;
if (ch >= '0' && ch <= '9') break;
ch = getchar();
}
if (ch == '-') minus = true; else result = ch-'0';
while (true) {
ch = getchar();
if (ch < '0' || ch > '9') break;
result = result*10 + (ch - '0');
}
if (minus)
return -result;
else
return result;
}


class Graph {
	private:
		unordered_set<int>* adj;
		size_t count;

	public:
		Graph( size_t const nodes_count ) {
			adj = new unordered_set<int>[ nodes_count ];	
			count = nodes_count;
		}

		void add_edge( int const node_a, int const node_b ) {
			adj[ node_a ].insert( node_b );
			adj[ node_b ].insert( node_a );
		}

		void delete_edge( int const node_a, int const node_b) {
			adj[ node_a ].erase( node_b );
			adj[ node_b ].erase( node_a );
		}

		unordered_set<int> get_adjacent_nodes( int const node_idx ) {
			return adj[ node_idx ]; 
		}

		void bfs() {
			bool discovered[ count ];  
			discovered[0] = true;

			for ( size_t i = 1; i < count; i++ ) {
				discovered[i] = false;
			}

			bool const red = true;
			bool const blue = false;

			bool colors[ count ];
			colors[0] = red;

			unordered_set<int> current_nodes;
			current_nodes.rehash(30);

			unordered_set<int> next_nodes;
			next_nodes.rehash(30);

			current_nodes.insert(0);

			size_t level = 1;

			while ( !current_nodes.empty() ) {
				for ( auto it = current_nodes.begin(); it != current_nodes.end(); it++ ) {

					for ( auto iter = adj[*it].begin(); iter != adj[*it].end(); iter++ ) {
						if ( discovered[ *iter ] == true ) continue;

						colors[ *iter ] = level % 2 == 0 ? red : blue;

						discovered[ *iter ] = true;
						next_nodes.insert( *iter );
					}

				}

				current_nodes = next_nodes;
				next_nodes.clear();
				level++;
			}

			current_nodes.clear();
			current_nodes.insert(0);

			next_nodes.clear();

			for ( size_t i = 1; i < count; i++ ) {
				discovered[i] = false;
			}

			while ( !current_nodes.empty() ) {
				for ( auto it = current_nodes.begin(); it != current_nodes.end(); it++ ) {
					bool cur_color = colors[ *it ];

					for ( auto iter = adj[*it].begin(); iter != adj[*it].end(); iter++ ) {
						if ( cur_color == colors[ *iter ] ) {
							cout << "-1";
							return;
						}

						if ( !discovered[ *iter ] ) 
							next_nodes.insert( *iter );
					}

					discovered[ *it ] = true;

				}

				current_nodes = next_nodes;
				next_nodes.clear();
			}

			for ( size_t i = 0; i < count; i++ ) {
				cout << !colors[i];
			}

			cout << endl;

		}
};

int main() {
	int nodes_count = readInt();	
	//cin >> nodes_count;

	Graph graph( nodes_count );	

	for ( int node = 0; node < nodes_count; node++ ) {
		int adj_node = readInt();
		//cin >> adj_node;
		while ( adj_node ) {
			graph.add_edge( node, adj_node - 1 );
			//cin >> adj_node;
			adj_node = readInt();
		}
	}

	graph.bfs();

	return 0;
}
