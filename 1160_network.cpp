#include <iostream>

using namespace std;

template< typename T, class Greater = greater<T> >
class BinaryHeap {
	private:
		long heap_size;
		T* buffer;
		
		int parent( int const node_idx) {
			return node_idx / 2;
		}

		int left_child( int const node_idx ) {
			return 2*node_idx + 1;
		}

		int right_child( int const node_idx ) {
			return 2*node_idx + 2;
		}

		void restore_heap( int const node_idx ) {
			int lchild = left_child( node_idx );
			int rchild = right_child( node_idx );
			int largest = 0;

			if ( lchild < heap_size && Greater(buffer[lchild], buffer[node_idx]) ) {
				largest = lchild;
			} else largest = node_idx;

			if ( rchild < heap_size && Greater( buffer[rchild], buffer[node_idx]) ) {
				largest = rchild;
			}

			if ( largest != node_idx ) {
				T tmp = buffer[largest];
				buffer[largest] = buffer[node_idx];
				buffer[node_idx] = tmp;

				restore_heap( largest );
			}
		}

	public:
		BinaryHeap( T* buffer_in, long const size ) {
			buffer = buffer_in;
			heap_size = size;

			for ( long i = size / 2; i >= 0; i-- ) {
				restore_heap(i);
			}
		}

		T maximum() {
			return buffer[0];
		}

		T extract_max() {
			T max = buffer[0];	

			buffer[0] = buffer[ heap_size - 1 ];
			heap_size--;

			restore();

			return max;
		}
		
		void increase_key( T key, int const i ) {
			buffer[i] = key;		

			while ( i > 0 && !Greater(buffer[parent(i)], buffer[i]) ) {
				swap( buffer[i], buffer[parent(i)] );
				i = parent(i);
			}
		}

		void restore() { restore_heap(0); }
};


int main() {
	
	return 0;
}
