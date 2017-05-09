#include <iostream>
#include <set>
#include <vector>

using namespace std;

static const long long INFINITY = 120000000;

struct Node {
	int parent;
	long long total_path_weight;
	set<int> adj_nodes;
	long incl_nodes_count = 0;
};

struct ListNode {
	int node_idx;
	ListNode* next;
};

class LinkedList {
	private:
		ListNode* head;
		ListNode* tail;
		size_t size;

	public:
		LinkedList() {
			size = 0;
			head = tail = NULL;
		}

		void push_back( int const node_idx ) {
			if ( tail == NULL ) {
				head = tail = new ListNode;
				tail-> next = NULL;
				tail-> node_idx = node_idx;

			} else {
				tail-> next = new ListNode;
				tail-> next-> node_idx = node_idx;

				tail-> next-> next = NULL;
				tail = tail-> next;
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

};

class Queue {
	private:
		vector<int> container;
		int tail;
		int head;

	public:
		Queue() { 
			tail = -1;
			head = 0;
		}

		void push_back( int const i ) {
			container.push_back(i);
			tail++;
		}

		int get() {
			return container[ head++ ];
		}

		bool is_empty() {
			return tail < head;
		}

		void print() {
			for ( size_t i = 0; i < container.size(); i++ ) {
				cout << container[i] << " ";
			}
			cout << endl;
		}
};

class DirectedAcyclicGraph {
	private:
		int16_t** _weight;
		Node* _adj;
		size_t _nodes_count;

		void init_single_source( int const node_idx ) {
			for ( size_t i = 0; i < _nodes_count; i++ ) {
				_adj[i].parent = -1;
				_adj[i].total_path_weight = INFINITY;
			}
			_adj[ node_idx ].total_path_weight = 0;
		}

		void relax( int const node_a, int const node_b ) {
			if ( _adj[node_b].total_path_weight > _adj[node_a].total_path_weight + _weight[node_a][node_b] ) {
				_adj[node_b].total_path_weight = _adj[node_a].total_path_weight + _weight[node_a][node_b];
				_adj[node_b].parent = node_a;
			}
		}

	public:

		DirectedAcyclicGraph( size_t const nodes_count ) {
			_adj = new Node[ nodes_count ];
			_nodes_count = nodes_count;
			_weight = new int16_t* [ nodes_count ];
			
			for ( size_t i = 0; i < nodes_count; i++ ) {
				_weight[i] = new int16_t[ nodes_count ];
			}
		}

		void add_edge( int const node_a, int const node_b, int16_t const weight ) {
			_adj[ node_a ].adj_nodes.insert( node_b );
			_weight[ node_a ][ node_b ] = weight;
		}

		LinkedList topology_sort() {
			bool discovered[_nodes_count];

			LinkedList list;
			Queue queue;

			for ( size_t i = 0; i < _nodes_count; i++ ) {
				Node cur = _adj[i];

				for ( auto it = cur.adj_nodes.begin(); it != cur.adj_nodes.end(); it++ ) {
					_adj[ *it ].incl_nodes_count++;	
				}
			}

			for ( size_t i = 0; i < _nodes_count; i++ ) {
				if ( _adj[i].incl_nodes_count == 0 ) {
					discovered[i] = true;
					queue.push_back(i);
				}
			}

			while ( !queue.is_empty() ) {
				int sealed_idx = queue.get();	
				list.push_back( sealed_idx );

 				for ( auto it = _adj[ sealed_idx ].adj_nodes.begin(); it != _adj[sealed_idx].adj_nodes.end();  it++ ) {
					if ( !discovered[*it] ) {
					
						_adj[ *it ].incl_nodes_count--;	
						if ( _adj[ *it ].incl_nodes_count == 0 ) {
							discovered[ *it ] = true;
							queue.push_back( *it );
						}
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
					relax( current, *it );
				}
			}

			return _adj[ dest ].total_path_weight != INFINITY ?
				_adj[ dest ].total_path_weight : 1;
		}

		
};

int main() {
	size_t nodes_count;
	size_t edges_count;

	cin >> nodes_count
		>> edges_count;

	DirectedAcyclicGraph graph( nodes_count );	

	for ( size_t i = 0; i < edges_count; i++ ) {
		int edge_source;
		int edge_dest;
		int16_t weight;

		cin >> edge_source 
			>> edge_dest
			>> weight;

		graph.add_edge( edge_source - 1, edge_dest - 1, -weight );
	}

	int path_source;
	int path_dest;

	cin >> path_source 
		>> path_dest;

	int shortest_path = -graph.shortest_path_weight(
		path_source - 1,
		path_dest - 1
	); 


	if ( shortest_path > 0 ) {
		cout << shortest_path << endl;
	} else {
		cout << "No solution" << endl;
	}
		
	return 0;
}
