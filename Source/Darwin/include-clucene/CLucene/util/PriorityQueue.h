#ifndef _lucene_util_PriorityQueue_
#define _lucene_util_PriorityQueue_

#include "CLucene/StdHeader.h"
namespace lucene{ namespace util{

//Porting notes:
//For some strange reason, that i couldn't figure out lessThan works the exact opposite
//that it should. Meaning the queue is returning high values first.
//
// A PriorityQueue maintains a partial ordering of its elements such that the
// least element can always be found in constant time.  Put()'s and pop()'s
//  require log(size) time. 

template <class _type> class PriorityQueue {
	private:
		_type* heap; //(was object[])
		int_t size;
		bool dk;

		void upHeap(){
			int_t i = size;
			_type node = heap[i];			  // save bottom node (WAS object)
			int_t j = ((uint_t)i) >> 1;		//TODO: chek this, was >>> not >>
			while (j > 0 && lessThan(node,heap[j])) {
				heap[i] = heap[j];			  // shift parents down
				i = j;
				j = ((uint_t)j) >> 1;	//TODO: chek this, was >>> not >>
			}
			heap[i] = node;				  // install saved node
		}
		void downHeap(){
			int_t i = 1;
			_type node = heap[i];			  // save top node
			int_t j = i << 1;				  // find smaller child
			int_t k = j + 1;
			if (k <= size && lessThan(heap[k], heap[j])) {
				j = k;
			}
			while (j <= size && lessThan(heap[j],node)) {
				heap[i] = heap[j];			  // shift up child
				i = j;
				j = i << 1;
				k = j + 1;
				if (k <= size && lessThan(heap[k], heap[j])) {
					j = k;
				}
			}
			heap[i] = node;				  // install saved node
		}

	protected:
		// Determines the ordering of objects in this priority queue.  Subclasses
		//	must define this one method. 
		virtual bool lessThan(_type a, _type b)=0;

		// Subclass constructors must call this. 
		void initialize(const int_t maxSize, bool deleteOnClear){
			size = 0;
			dk = deleteOnClear;
			int_t heapSize = (maxSize * 2) + 1;
			heap = new _type[heapSize];
		}

	public:
		virtual ~PriorityQueue(){
			clear();
			_DELETE_ARRAY(heap);
		}

		// Adds an object to a PriorityQueue in log(size) time.  
		void put(_type element){
			size++;	
			heap[size] = element;		
			upHeap();
		}

		// Returns the least element of the PriorityQueue in constant time. 
		_type top(){
			if (size > 0)
				return heap[1];
			else
				return NULL;
		}

		// Removes and returns the least element of the PriorityQueue in log(size)
		//	time.  
		_type pop(){
			if (size > 0) {
				_type result = heap[1];			  // save first value
				heap[1] = heap[size];			  // move last to first

				heap[size] = NULL;			  // permit GC of objects
				size--;
				downHeap();				  // adjust heap
				return result;
			} else
				return NULL;
		}

		// Should be called when the object at top changes values.  Still log(n)
		// worst case, but it's at least twice as fast to <pre>
		//  { pq.top().change(); pq.adjustTop(); }
		// </pre> instead of <pre>
		//  { o = pq.pop(); o.change(); pq.push(o); }
		// </pre>
		
		void adjustTop(){
			downHeap();
		}
		    

		// Returns the number of elements currently stored in the PriorityQueue. 
		int_t Size(){
			return size;
		}
		  
		// Removes all entries from the PriorityQueue. 
		void clear(){
			for (int_t i = 1; i <= size; i++){
				if ( dk ){
					_DELETE(heap[i]);
				    }
			}
			size = 0;
		}
	};
}}
#endif
