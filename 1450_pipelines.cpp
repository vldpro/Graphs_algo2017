#include <iostream>
#include <set>
#include <vector>
#include <unordered_set>
#include <forward_list>

using namespace std;

static const long INFINITY = 120000000;

struct Node {
	int parent = -1;
	long total_path_weight = INFINITY;
	forward_list< pair<int,int> > adj_nodes;
	long incl_nodes_count = 0;
};


class LinkedList {
	private:
		struct ListNode {
			int node_idx;
			ListNode* next;
		};

		ListNode* head = NULL;
		ListNode* tail = NULL;
		size_t size = 0;

	public:
		LinkedList() { }

		void push_back( int const node_idx ) {

			if ( size == 0 ) {
				head = tail = new ListNode;
				tail-> next = NULL;
				tail-> node_idx = node_idx;

			} else {
				tail-> next = new ListNode;
				tail = tail-> next;

				tail-> node_idx = node_idx;
				tail-> next = NULL;
			}

			size++;
		}

		int pop_head() {
			int to_return = head-> node_idx;	
			head = head-> next;
			size--;

			return to_return;
		}

		bool is_empty() {
			return size == 0;
		}

		int get_size() {
			return size;
		}

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

		void init_single_source( int const node_idx ) {
			_adj[ node_idx ].total_path_weight = 0;
		}

		void inline relax( int const node_a, int const node_b ) {
			
		}

	public:

		DirectedAcyclicGraph( size_t const nodes_count ) {
			_adj = new Node[ nodes_count ];
			_nodes_count = nodes_count;
		}

		void add_edge( int const node_a, int const node_b, int const weight ) {
			_adj[ node_a ].adj_nodes.push_front( pair<int,int>(node_b, weight) );
			_adj[ node_b ].incl_nodes_count++;
		}

		LinkedList topology_sort() {
			LinkedList list;
			LinkedList queue;

			for ( size_t i = 0; i < _nodes_count; i++ ) {
				if ( _adj[i].incl_nodes_count == 0 ) {
					queue.push_back(i);
				}
			}

			while ( !queue.is_empty() ) {
				int sealed_idx = queue.pop_head();	
				list.push_back( sealed_idx );

 				for ( auto it = _adj[ sealed_idx ].adj_nodes.begin(); it != _adj[sealed_idx].adj_nodes.end();  it++ ) {
						_adj[ it-> first ].incl_nodes_count--;	
						if ( _adj[ it-> first ].incl_nodes_count == 0 ) {
							queue.push_back( it-> first );
						}
				}

			}

			return list;

		}

		int shortest_path_weight( int const source, int const dest ) {
			LinkedList sorted_nodes = topology_sort(); 

			init_single_source( source );

			int current;

			while ( !sorted_nodes.is_empty() ) {
				current = sorted_nodes.pop_head();

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
