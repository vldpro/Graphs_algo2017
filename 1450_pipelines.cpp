#include <iostream>
#include <vector>
#include <forward_list>
#include <list>

using namespace std;

static const long INFINITY = 120000000;

struct Node {
	int parent = -1;
	long total_path_weight = INFINITY;
	forward_list< pair<int,int> > adj_nodes;
	long incl_nodes_count = 0;
};

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

class DirectedAcyclicGraph {
	private:
		Node* _adj;
		size_t _nodes_count;

	public:

		DirectedAcyclicGraph( size_t const nodes_count ) {
			_adj = new Node[ nodes_count ];
			_nodes_count = nodes_count;
		}

		void add_edge( int const node_a, int const node_b, int const weight ) {
			_adj[ node_a ].adj_nodes.push_front( pair<int,int>(node_b, weight) );
			_adj[ node_b ].incl_nodes_count++;
		}

		list<int> topology_sort() {
			list<int> queue;
			list<int> lst;

			for ( int i = 0; i < _nodes_count; i++ ) {
				if ( _adj[i].incl_nodes_count == 0 ) {
					queue.push_back(i);
				}
			}

			while ( !queue.empty() ) {
				int sealed_idx = queue.front();	
				queue.pop_front();
				lst.push_back( sealed_idx );

 				for ( auto it = _adj[ sealed_idx ].adj_nodes.begin(); it != _adj[sealed_idx].adj_nodes.end();  it++ ) {
						_adj[ it-> first ].incl_nodes_count--;	
						if ( _adj[ it-> first ].incl_nodes_count == 0 ) {
							queue.push_back( it-> first );
						}
				}

			}

			return lst;

		}

		int shortest_path_weight( int const source, int const dest ) {
			list<int> sorted_nodes = topology_sort(); 
			_adj[ source ].total_path_weight = 0;

			int current;

			while ( !sorted_nodes.empty() ) {
				current = sorted_nodes.front();
				sorted_nodes.pop_front();

				for ( auto it = _adj[current].adj_nodes.begin(); it != _adj[current].adj_nodes.end(); it++ ) {
					if ( _adj[it-> first].total_path_weight > _adj[current].total_path_weight + it-> second ) {
						_adj[it-> first].total_path_weight = _adj[current].total_path_weight + it-> second;
						_adj[it-> first].parent = current;
					}
				}
			}

			return _adj[ dest ].total_path_weight != INFINITY ?
				_adj[ dest ].total_path_weight : 1;
		}

		
};

int main() {
	int nodes_count = readInt();
	int edges_count = readInt();

	/*cin >> nodes_count
		>> edges_count;*/

	DirectedAcyclicGraph graph( nodes_count );	

	for ( size_t i = 0; i < edges_count; i++ ) {
		int edge_source = readInt();
		int edge_dest = readInt();
		int weight = readInt();

		/*cin >> edge_source 
			>> edge_dest
			>> weight;*/

		graph.add_edge( edge_source - 1, edge_dest - 1, -weight );
	}

	int path_source = readInt();
	int path_dest = readInt();

	/*cin >> path_source 
		>> path_dest; */

	int shortest_path = -graph.shortest_path_weight(
		path_source - 1,
		path_dest - 1
	); 


	if ( shortest_path > 0 ) {
		cout << shortest_path;
	} else {
		cout << "No solution";
	}
		
	return 0;
}
