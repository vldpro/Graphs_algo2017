#include <iostream>
#include <limits>
#include <string.h>
#include <stack>
#include <queue>
#include <unordered_map>
#include <forward_list>

using namespace std;

static const uint32_t INFINITY = numeric_limits<uint32_t>::max();

bool compare( pair<uint32_t, uint32_t> const& a, pair<uint32_t, uint32_t> const& b ) {
	return a.first > b.first;
}

long long read_llong () {
	bool minus = false;
	long long result = 0;
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


struct GraphNode {
	int64_t parent = -1;
	uint32_t shortest_path = INFINITY;
	bool discovered = false;
	forward_list< pair<uint32_t, uint32_t> > adjacents;
};

class Graph {
	private:
		GraphNode* _nodes;
		uint32_t _nodes_count;

	public:
		Graph( uint32_t const nodes_count ) {
			_nodes = new GraphNode[ nodes_count ];	
			_nodes_count = nodes_count;
		}

		void add_edge( uint32_t const src, uint32_t const dest, uint32_t const weight ) {
			_nodes[src].adjacents.push_front( pair<uint32_t, uint32_t>(dest, weight) );
		}

		void shortest_path() {
			_nodes[0].shortest_path = 0;

			priority_queue <
				pair<uint32_t, uint32_t>,
				vector< pair<uint32_t, uint32_t> >,
				decltype(&compare)
			> pqueue(&compare);

			pqueue.push( pair<uint32_t, uint32_t>(0, 0) );

			while( !pqueue.empty() ) {
				uint32_t i = pqueue.top().second;
				pqueue.pop();

				if ( _nodes[i].discovered ) continue;
				_nodes[i].discovered = true;

				for ( auto it = _nodes[i].adjacents.begin(); it != _nodes[i].adjacents.end(); it++ ) {
					if ( _nodes[it-> first].discovered ) continue;

					if ( _nodes[it-> first].shortest_path > _nodes[i].shortest_path + it-> second ) {
						_nodes[it-> first].shortest_path = _nodes[i].shortest_path + it-> second;
						_nodes[it-> first].parent = i;
						pqueue.push( pair<uint32_t, uint32_t>(_nodes[it->first].shortest_path, it-> first) );
					}
				}
			}
		}

		void answer() {
			if ( !_nodes[_nodes_count - 1].discovered ) {
				cout << "-1" << endl;
				return;
			}

			stack<int64_t> stk;

			uint32_t i = _nodes_count - 1;
			stk.push(i);

			while ( i != 0 ) {
				i = _nodes[i].parent; stk.push(i);
			}

			cout <<	_nodes[_nodes_count - 1].shortest_path << endl;
			cout << stk.size() << endl;

			while ( !stk.empty() ) {
				cout << stk.top() + 1 << " "; 
				stk.pop();
			}

			cout << endl;
		}
};

static unordered_map<long long, uint32_t> devices;
static uint32_t weights[10];
static long long const power[] = {
	1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000     
};

static inline void search_compatible_devices( long long const device_id, Graph* const graph ) {

	for ( int i = 9; i >= 0; i-- ) {
		for ( size_t j = 0; j < 10; j++ ) {
			long long tmp = device_id - ( (device_id / power[i]) % 10 - j ) * power[i];

			if ( tmp != device_id && devices.find(tmp) != devices.end() ) {
				uint32_t dev_a = devices[tmp];
				uint32_t dev_b = devices[device_id];

				graph-> add_edge( dev_a, dev_b, weights[9 - i] );
				graph-> add_edge( dev_b, dev_a, weights[9 - i] );
			}
		}
	}

	for ( int i = 9; i >= 0; i-- ) {
		long long inum = (device_id / power[i]) % 10;

		for ( int j = i - 1; j >= 0; j-- ) {
			long long jnum = ((device_id / power[j]) % 10);
			if ( jnum == inum ) continue;

			long long tmp = device_id - (power[i] - power[j]) * (inum - jnum);

			if ( tmp != device_id && devices.find(tmp) != devices.end() ) {
				uint32_t dev_a = devices[tmp];
				uint32_t dev_b = devices[device_id];

				graph-> add_edge( dev_a, dev_b, weights[9 - i] );
				graph-> add_edge( dev_b, dev_a, weights[9 - i] );
			}

		}
	}
}

int main() {
	uint32_t nodes_count;
	cin >> nodes_count;
	devices.rehash(nodes_count);

	Graph graph(nodes_count);
	
	for ( size_t i = 0; i < 10; i++ ) {
		cin >> weights[i];
	}

	for ( size_t i = 0; i < nodes_count; i++ ) {
		long long device = read_llong();
		
		devices.insert( pair<long long, uint32_t>(device, i) );
		search_compatible_devices( device, &graph );
	}

	graph.shortest_path();
	graph.answer();

	return 0;

}
