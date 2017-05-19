#include <iostream>
#include <unordered_map>
#include <forward_list>
#include <queue>
#include <map>
#include <sstream>

using namespace std;

static const int INFINITY = 12000000;
static const int ORPHAN_NODE = -1;

bool compare( pair<int,int> a, pair<int,int> b ) {
	return a.first > b.first;	
}

class UndirectedGraph {
	private:	

		struct GraphNode {
			int key = INFINITY;
			int parent = ORPHAN_NODE;
			forward_list< pair<int, int> > adjacents;
		};

		GraphNode* _nodes;
		int _nodes_count;

	public:

		UndirectedGraph( int const nodes_count ) {
			_nodes = new GraphNode[ nodes_count ];
			_nodes_count = nodes_count;
		}

		void add_edge( int const node_a, int const node_b, int const weight ) {
			_nodes[ node_a ].adjacents.push_front( pair<int,int>(node_b, weight) );
			_nodes[ node_b ].adjacents.push_front( pair<int,int>(node_a, weight) );
		}


		/* Prim algorithm */
		void minimal_spanning_tree() {
			bool discovered[ _nodes_count ] = {};
			_nodes[0].key = 0;

			priority_queue< 
				pair<int,int>, 
				vector<pair<int,int>>, 
				decltype(&compare) 
			> pqueue(&compare);

			pqueue.push( pair<int,int> (0, 0) );

			while ( !pqueue.empty() ) {
				int i = pqueue.top().second;
				pqueue.pop();

				if ( discovered[i] ) continue;
				discovered[i] = true;

				for ( auto it = _nodes[i].adjacents.begin(); it != _nodes[i].adjacents.end(); it++ ) {
					if ( discovered[it-> first] ||  it-> second > _nodes[it-> first].key ) continue;

					_nodes[it-> first].parent = i;
					_nodes[it-> first].key = it-> second;

					pqueue.push( pair<int,int> (it-> second, it-> first) );
				}
			}
		}

		string traverse_mst() {
			stringstream ss;
			int max = 0;

			for ( int i = 0; i < _nodes_count; i++ ) {
				for ( auto it = _nodes[i].adjacents.begin(); it != _nodes[i].adjacents.end(); it++ ) {
					if ( _nodes[ it-> first ].parent == i ) {
						if ( _nodes[ it->first ].key > max ) max = _nodes[ it-> first ].key;

						ss << i + 1 << " " << it-> first + 1 << endl;
					}
				}
			}

			cout << max << endl;
			return ss.str();
		}
		
};

int read_int () {
	bool minus = false;
	int result = 0;
	char ch;
	ch = getchar();

	while (true) {
		if (ch == '-') break;
		if (ch >= '0' && ch <= '9') break;
		ch = getchar();
	}

	if (ch == '-') minus = true; else result = ch - '0';
	while (true) {
		ch = getchar();
		if (ch < '0' || ch > '9') break;
		result = result*10 + (ch - '0');
	}

	return minus ? -result : result;
}

int main() {
	int nodes_count = read_int();
	int edges_count = read_int();
	//cin >> nodes_count >> edges_count;

	UndirectedGraph graph( nodes_count );

	for ( int i = 0; i < edges_count; i++ ) {
		int source = read_int();		
		int dest = read_int();
		int weight = read_int();

		//cin >> source >> dest >> weight;

		graph.add_edge( source - 1, dest - 1, weight );
	}

	graph.minimal_spanning_tree();
	string out = graph.traverse_mst();

	cout << nodes_count - 1 << endl;
	cout << out;

	return 0;
}
