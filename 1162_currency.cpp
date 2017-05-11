#include <iostream>
#include <map>

using namespace std;

struct exc_point {
	double rate;
	double comission;
};

class DirectedGraph {
	private:
		
		struct GraphNode {
			int parent = -1;
			double d = 0;
			double w = 0;
			map<int,exc_point> adjacents;
		};

		GraphNode* _nodes;
		long _nodes_count;

		void relax( int const src, int const dest ) {
			if ( _nodes[dest].d > _nodes[src].d + edge_weight(src,dest) ) {
				_nodes[dest].d = _nodes[src].d + edge_weight(src, dest);
				_nodes[dest].w = edge_weight(src, dest);
				_nodes[dest].parent = src;
			}
		}

	public:

		DirectedGraph( long const nodes_count ) {
			_nodes_count = nodes_count;	
			_nodes = new GraphNode [ _nodes_count ];
		}

		void add_edge( int const src, int const dest, double const rate, double const comission ) {
			exc_point expt;
			expt.rate = rate;
			expt.comission = comission;

			_nodes[ src ].adjacents.insert( 
				pair<int, exc_point> (dest, expt) 
			);
		}

		double edge_weight( int const src, int const dest ) {
			return (_nodes[src].w - _nodes[src].adjacents[dest].comission) * _nodes[src].adjacents[dest].rate;
		}

		bool bellman_ford( int start, double init_weight ) {
			_nodes[start].w = init_weight;
			_nodes[start].d = init_weight;

			for ( int i = 0; i < _nodes_count; i++ ) {
				for ( int j = 0; j < _nodes_count; j++ ) {
					for ( auto it = _nodes[j].adjacents.begin(); it != _nodes[j].adjacents.end(); it++ ) {
						relax( j, it-> first );
					}
				}
			}

			/*for ( int j = 0; j < _nodes_count; j++ ) {
				for ( auto it = _nodes[j].adjacents.begin(); it != _nodes[j].adjacents.end(); it++ ) {
					if ( _nodes[it->first].parent == j ) 
						cout << j << " " << it-> first << " " << _nodes[it-> first].w << endl;
				}
			}

			cout << _nodes[2].w << endl; */

			//if ( _nodes[_nodes_count - 1].w < init_weight ) return false;

			for ( int j = 0; j < _nodes_count; j++ ) {
				for ( auto it = _nodes[j].adjacents.begin(); it != _nodes[j].adjacents.end(); it++ ) {
					if ( _nodes[it->first].d > _nodes[j].d + edge_weight(j, it-> first) ) {
						//cout << j << " " << it-> first << " " << _nodes[it-> first].w << endl;
						return false;
					}
				}
			}

			return true;
		}

		
		
};

int main() {
	int nodes_count;
	int	exchange_points_count; 
	int nick_currency_num;
	double nick_cash;

	cin >> nodes_count 
		>> exchange_points_count
		>> nick_currency_num
		>> nick_cash;

	DirectedGraph graph(nodes_count);

	for ( int i = 0; i < exchange_points_count; i++ ) {
		int cur_a, cur_b;
		double exchange_ab_rate, exchange_ab_comission;
		double exchange_ba_rate, exchange_ba_comission;
		
		cin >> cur_a >> cur_b
			>> exchange_ab_rate >> exchange_ab_comission
			>> exchange_ba_rate >> exchange_ba_comission;

		/*if ( cur_a == nick_currency_num ) {
			graph.add_edge( cur_a - 1, cur_b - 1, exchange_ab_rate, -exchange_ab_comission );	
			graph.add_edge( cur_b - 1, nodes_count, exchange_ba_rate, -exchange_ba_comission );	

		} else if ( cur_b == nick_currency_num ) {
			graph.add_edge( cur_a - 1, nodes_count, exchange_ab_rate, -exchange_ab_comission );	
			graph.add_edge( cur_b - 1, cur_a - 1, exchange_ba_rate, -exchange_ba_comission );	

		} else {*/
			graph.add_edge( cur_a - 1, cur_b - 1, exchange_ab_rate, -exchange_ab_comission );	
			graph.add_edge( cur_b - 1, cur_a - 1, exchange_ba_rate, -exchange_ba_comission );	
		//}
	}

	string answer = graph.bellman_ford( nick_currency_num - 1, -nick_cash ) ? "NO" : "YES";
	cout << answer << endl;

	return 0;
}
