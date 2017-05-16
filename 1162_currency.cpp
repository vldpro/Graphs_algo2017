#include <iostream>
#include <map>

using namespace std;

static inline double cash( double const cash, double const rate, double const comission ) {
	return (cash - comission) * rate;
}

struct Edge {
	size_t src_node, dest_node;
	double rate, comission;
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

	size_t const EDGES_COUNT = 2 * exchange_points_count;
	Edge edges[ EDGES_COUNT ];

	int j = 0;
	for ( int i = 0; i < exchange_points_count; i++ ) {
		size_t cur_a, cur_b;
		double exchange_ab_rate, exchange_ab_comission;
		double exchange_ba_rate, exchange_ba_comission;
		
		cin >> cur_a >> cur_b
			>> exchange_ab_rate >> exchange_ab_comission
			>> exchange_ba_rate >> exchange_ba_comission;

		edges[j++] = { cur_a - 1, cur_b - 1, exchange_ab_rate, exchange_ab_comission };
		edges[j++] = { cur_b - 1, cur_a - 1, exchange_ba_rate, exchange_ba_comission };
	}

	double nodes[nodes_count] = {};
	nodes[nick_currency_num - 1] = nick_cash;

	// Bellman-ford

	for ( size_t i = 0; i < nodes_count - 1; i++ ) {
		for ( size_t j = 0; j < EDGES_COUNT; j++ ) {
			nodes[ edges[j].dest_node ] = 
				max( 
					nodes[ edges[j].dest_node ], 
					cash( nodes[edges[j].src_node], edges[j].rate, edges[j].comission ) 
				);
		}
	}

	for ( size_t i = 0; i < EDGES_COUNT; i++ ) {
		if ( nodes[ edges[i].dest_node ] + 0.000001 <  cash( nodes[edges[i].src_node], edges[i].rate, edges[i].comission ) ) {
			cout << "YES";		
			return 0;
		}
	}

	cout << "NO";
}


