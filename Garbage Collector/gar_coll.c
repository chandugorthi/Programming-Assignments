#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#define HEAP_SIZE 67108864
#define ROOT_SPACE 67108864
#define HEAP_HEAD 16
#define ROOT_HEAD 16

int REM_HEAP = HEAP_SIZE;

//The address I return should be divisible by 8

typedef struct root_hdr{
	size_t root_size;
	void *p;
} root_hdr;

typedef struct heap_hdr{
	size_t chunk_size;
	struct heap_hdr* fwd_ptr;
}heap_hdr;

void* root = NULL;
void* heap = NULL;
void* free_heap = NULL;
void* root_base = NULL;
void* heap_base = NULL;
void* free_base = NULL;
static int count = 0;
static int initialize = 0;

void init(){

	root = mmap(NULL, ROOT_SPACE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	heap = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	free_heap = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	root_base = root;
	heap_base = heap;
	free_base = free_heap;

}

void add_root(void* addr, size_t len){
	if ( initialize == 0 ){
		init();
		initialize++;
	}
	root_hdr* newRoot = root;
	newRoot->root_size = len;
	newRoot->p = addr;
	void* temp = newRoot;
	temp = temp + ROOT_HEAD;
	root = temp;
}

void del_root(void* addr){
	void* temp = root_base;
	root_hdr* rootItr = temp;
	int count = 0;
	while( rootItr->p != addr && (void*)rootItr < root){
		count++;
		temp = temp + ROOT_HEAD;
		rootItr = temp;
	}
	rootItr->root_size = 0;
	rootItr->p = NULL;
}

void *searchAndMove(void* ptr){
	void* start = heap_base;
	if ( ptr >= start && ptr < heap ){
		heap_hdr* present = start;
		void* end = start + present->chunk_size;
		while ( ptr > end ){
			start = end;
			present = start;
			end = start + present->chunk_size;
		}
		int offset = ptr - start - HEAP_HEAD;
		if ( present->fwd_ptr == NULL ){
			heap_hdr* newAlloc = free_heap;
			newAlloc->chunk_size = present->chunk_size;
			newAlloc->fwd_ptr = NULL;
			void* fwd = (void*)newAlloc + HEAP_HEAD;
			present->fwd_ptr = fwd;
			void* temp1 = start + HEAP_HEAD;
			void* temp2 = (void*) newAlloc + HEAP_HEAD;
			memcpy(temp2, temp1, newAlloc->chunk_size - (size_t)HEAP_HEAD);
			free_heap = (void*) newAlloc + newAlloc->chunk_size;
			count++;
			return temp2 + offset;
		} else {
			void* temp = present->fwd_ptr;
			return temp + offset;
		}
	}
	return ptr;
}

//Reset free, heap, free_base and heap_base pointers after garbage collection -- DONE
//Make count global variable (for counting number of live objects moved) -- DONE
void collect(){
	void* temp = root_base;
	while ( temp < root ){
		root_hdr* tempRoot = temp;
		//Add Code *************************************************
		if ( tempRoot->root_size == 0){
			temp = temp + ROOT_HEAD;
			continue;
		}
		if ( tempRoot->root_size < 8 ){
			long z = *(long*)tempRoot->p;
			void* z1;
			z1 = (void*)z;
			if ( z1 >= heap_base && z1 < heap ){
				long* ab = (long*)searchAndMove(z1);
				long abc = (long)ab;
				*((long*)tempRoot->p) = abc;
			}
		} else {
			//Modify this section
				int len = tempRoot->root_size;
				void* tempPtr;
				long x;
				void* tempPtr1;
				tempPtr = tempRoot->p;
				while ( len >= 8 ){
					x = *(long*)tempPtr;
					tempPtr1 = (void*)x;
					if ( tempPtr1 >= heap_base && tempPtr1 < heap ) {
						long* y = (long*)searchAndMove(tempPtr1);
						long yz = (long)y;
						*((long*)tempPtr) = yz;
					}
					len = len - 8;
					tempPtr = tempPtr + 8;
				}
		}
		temp = temp + ROOT_HEAD;
	}
	temp = free_base;
	while ( temp < free_heap ){
		heap_hdr* alloc = temp;
		temp = temp + HEAP_HEAD;
		if ( alloc->chunk_size - HEAP_HEAD >=8 ){
			int len = alloc->chunk_size - HEAP_HEAD;
			long x;
			void* temp1;
			while ( len >= 8 ){
				x = *(long*)temp;
				temp1 = (void*)x;
				if ( temp1 >= heap_base && temp1 < heap ) {
					long* y = (long*)searchAndMove(temp1);
					long yz = (long)y;
					*((long*)temp) = yz;
				}
				len = len-8;
				temp = temp + 8;
			}

		}
		temp = (void*) alloc + alloc->chunk_size;
	}
	temp = free_base;
	free_base = heap_base;
	heap_base = temp;
	heap = free_heap;
	free_heap = free_base;
	REM_HEAP = HEAP_SIZE - (heap - heap_base);
}

void *allocate(size_t request, size_t* collected){
	if (initialize == 0){
		init();
		initialize++;
	}
	if ( request <= REM_HEAP - HEAP_HEAD ){
		*collected = 0;
		heap_hdr* newHead = heap;
		void* temp = heap;
		int offset = 0;
		if ( request%8 != 0 ){
			offset = 8 - request%8;
		}
		newHead->chunk_size = HEAP_HEAD + request + offset;
		newHead->fwd_ptr = NULL;
		REM_HEAP = REM_HEAP - HEAP_HEAD - request - offset ;
		temp = temp + HEAP_HEAD;
		void* ret = temp;
		heap = temp + newHead->chunk_size - HEAP_HEAD;
		return ret;
	} else {
		int prevRem = REM_HEAP;
		collect();
		if ( prevRem == REM_HEAP ) {
			//Should We write 0 into collected in this case ?
			*collected = 0;
			count = 0;
			return NULL;
		} else if ( request > REM_HEAP - HEAP_HEAD ) {
			*collected = 0;
			count = 0;
			return NULL;
		}
		*collected = count;
		count = 0;
		heap_hdr* newHead = heap;
		void* temp = heap;
		int offset = 0;
		if ( request%8 != 0 ){
			offset = 8 - request%8;
		}
		newHead->chunk_size = HEAP_HEAD + request + offset;
		newHead->fwd_ptr = NULL;
		REM_HEAP = REM_HEAP - HEAP_HEAD - request - offset;
		temp = temp + HEAP_HEAD;
		void* ret = temp;
		heap = temp + newHead->chunk_size - HEAP_HEAD;
		return ret;
	}
}

size_t heap_max(){
	return REM_HEAP - HEAP_HEAD;
}
