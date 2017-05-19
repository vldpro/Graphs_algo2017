#include <iostream>
#include <limits>
#include <string.h>
#include <stack>
#include <queue>
#include <unordered_map>
#include <forward_list>

using namespace std;

template <class V> class FibonacciHeap;

template <class V> struct node {
private:
	node<V>* prev;
	node<V>* next;
	node<V>* child;
	node<V>* parent;
	V value;
	int degree;
	bool marked;
public:
	friend class FibonacciHeap<V>;
	node<V>* getPrev() {return prev;}
	node<V>* getNext() {return next;}
	node<V>* getChild() {return child;}
	node<V>* getParent() {return parent;}
	V getValue() {return value;}
	bool isMarked() {return marked;}

	bool hasChildren() {return child;}
	bool hasParent() {return parent;}
};

template <class V> class FibonacciHeap {
protected:
	node<V>* heap;
	node<V>* min;
public:

	FibonacciHeap() {
		heap=_empty();
		min = heap;
	}
	virtual ~FibonacciHeap() {
		if(heap) {
			_deleteAll(heap);
		}
	}
	node<V>* insert(V value) {
		node<V>* ret=_singleton(value);
		heap=_merge(heap,ret);
		return ret;
	}
	void merge(FibonacciHeap& other) {
		heap=_merge(heap,other.heap);
		other.heap=_empty();
	}

	bool isEmpty() {
		return heap==NULL;
	}

	V getMinimum() {
		return heap->value;
	}

	V removeMinimum() {
		node<V>* old=heap;
		heap=_removeMinimum(heap);
		V ret=old->value;
		delete old;
		return ret;
	}

	void decreaseKey(node<V>* n,V value) {
		heap=_decreaseKey(heap,n,value);
	}

	node<V>* find(V value) {
		return _find(heap,value);
	}
private:
	node<V>* _empty() {
		return NULL;
	}

	node<V>* _singleton(V value) {
		node<V>* n=new node<V>;
		n->value=value;
		n->prev=n->next=n;
		n->degree=0;
		n->marked=false;
		n->child=NULL;
		n->parent=NULL;
		return n;
	}

	node<V>* _merge(node<V>* a,node<V>* b) {
		if(a==NULL)return b;
		if(b==NULL)return a;
		if(a->value > b->value) {
			node<V>* temp=a;
			a=b;
			b=temp;
		}
		node<V>* an=a->next;
		node<V>* bp=b->prev;

		a->next=b;
		b->prev=a;
		an->prev=bp;
		bp->next=an;
		return a;
	}

	void _deleteAll(node<V>* n) {
		if(n!=NULL) {
			node<V>* c=n;
			do {
				node<V>* d=c;
				c=c->next;
				_deleteAll(d->child);
				delete d;
			} while(c!=n);
		}
	}
	
	void _addChild(node<V>* parent,node<V>* child) {
		child->prev=child->next=child;
		child->parent=parent;
		parent->degree++;
		parent->child=_merge(parent->child,child);
	}

	void _unMarkAndUnParentAll(node<V>* n) {
		if(n==NULL)return;
		node<V>* c=n;
		do {
			c->marked=false;
			c->parent=NULL;
			c=c->next;
		}while(c!=n);
	}

	node<V>* _removeMinimum(node<V>* n) {
		_unMarkAndUnParentAll(n->child);
		if(n->next==n) {
			n=n->child;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n=_merge(n->next,n->child);
		}
		if(n==NULL)return n;
		node<V>* trees[64]={NULL};
		
		while(true) {
			if(trees[n->degree]!=NULL) {
				node<V>* t=trees[n->degree];
				if(t==n)break;
				trees[n->degree]=NULL;
				if(n->value<t->value) {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					_addChild(n,t);
				} else {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					if(n->next==n) {
						t->next=t->prev=t;
						_addChild(t,n);
						n=t;
					} else {
						n->prev->next=t;
						n->next->prev=t;
						t->next=n->next;
						t->prev=n->prev;
						_addChild(t,n);
						n=t;
					}
				}
				continue;
			} else {
				trees[n->degree]=n;
			}
			n=n->next;
		}
		node<V>* min=n;
		do {
			if(n->value<min->value)min=n;
			n=n->next;
		} while(n!=n);
		return min;
	}

	node<V>* _cut(node<V>* heap,node<V>* n) {
		if(n->next==n) {
			n->parent->child=NULL;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n->parent->child=n->next;
		}
		n->next=n->prev=n;
		n->marked=false;
		return _merge(heap,n);
	}

	node<V>* _decreaseKey(node<V>* heap,node<V>* n,V value) {
		if(n->value < value)return heap;
		n->value=value;

		if(n-> parent && n->value < n->parent->value) {
			heap=_cut(heap,n);
			node<V>* parent=n->parent;
			n->parent=NULL;

			while(parent!=NULL && parent->marked) {
				heap=_cut(heap,parent);
				n=parent;
				parent=n->parent;
				n->parent=NULL;
			}

			if(parent!=NULL && parent->parent!=NULL)
				parent->marked=true;

		} else if ( !n-> parent && heap-> value > value ) {
			return n;
		}

		return heap;
	}

	node<V>* _find(node<V>* heap,V value) {
		node<V>* n=heap;
		if(n==NULL)return NULL;
		do {
			if(n->value==value)return n;
			node<V>* ret=_find(n->child,value);
			if(ret)return ret;
			n=n->next;
		}while(n!=heap);
		return NULL;
	}
};

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
				
			struct meta {
				uint32_t key = 0;
				uint32_t value = 0;

				bool operator< ( struct meta const& b ) {
					return key < b.key;
				}

				
				bool operator> ( struct meta const& b ) {
					return key > b.key;
				}
			};

			/*priority_queue <
				pair<uint32_t, uint32_t>,
				vector< pair<uint32_t, uint32_t> >,
				decltype(&compare)
			> pqueue(&compare);

			pqueue.push( pair<uint32_t, uint32_t>(0, 0) ); */

			FibonacciHeap<meta> fibheap;
			node<meta>* mapping[_nodes_count];

			for ( size_t i = 0; i < _nodes_count; i++ ) {
				meta mt;
				mt.key = _nodes[i].shortest_path;
				mt.value = i;
				mapping[i] = fibheap.insert( mt );
			}

			while( !fibheap.isEmpty() ) {
				//uint32_t i = pqueue.top().second;
				//pqueue.pop();
				uint32_t i = fibheap.removeMinimum().value;

				if ( _nodes[i].discovered ) continue;
				_nodes[i].discovered = true;

				for ( auto it = _nodes[i].adjacents.begin(); it != _nodes[i].adjacents.end(); it++ ) {
					if ( _nodes[it-> first].discovered ) continue;

					if ( _nodes[it-> first].shortest_path > _nodes[i].shortest_path + it-> second ) {
						_nodes[it-> first].shortest_path = _nodes[i].shortest_path + it-> second;
						_nodes[it-> first].parent = i;
						//pqueue.push( pair<uint32_t, uint32_t>(_nodes[it->first].shortest_path, it-> first) );
						meta mt;
						mt.key = _nodes[it->first].shortest_path;
						mt.value = it-> first; 
						fibheap.decreaseKey( mapping[it->first], mt );
					}
				}
			}
		}

		void answer() {
			/*if ( !_nodes[_nodes_count - 1].discovered ) {
				cout << "-1" << endl;
				return;
			}*/

			stack<int64_t> stk;

			int64_t i = _nodes_count - 1;
			stk.push(i);

			while ( i != 0 ) {
				i = _nodes[i].parent; stk.push(i);

				if ( i == -1 ) {
					cout << i << endl;
					return;
				}
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
