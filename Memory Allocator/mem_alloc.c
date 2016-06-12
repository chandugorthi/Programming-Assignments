#include <stdio.h>
#include <sys/mman.h>

#define HEAP_SIZE 67108864
#define MIN_SIZE 24
#define DELIM_SIZE 8
long REM_HEAP = HEAP_SIZE - DELIM_SIZE;
static char heap[HEAP_SIZE];

typedef struct Node{
	struct Node* next;
	size_t chunk_size;
	size_t alloc_size;
} Node;

typedef struct MemAlloc{
	long addr;
	size_t len;
	struct MemAlloc* next;
}MemAlloc;

Node* heap_list = NULL;
Node* free_list = NULL;
MemAlloc* dealloc_addr = NULL;

void init(){
	void* init = mmap(NULL,HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	free_list = init;
	heap_list = init;
	free_list->next = NULL;
	free_list->chunk_size = HEAP_SIZE;
	free_list->alloc_size = HEAP_SIZE;
}

size_t heap_max(){
	size_t max = 0;
	Node* temp = free_list;
	while(temp != NULL){
		if ( max < temp->chunk_size - MIN_SIZE - DELIM_SIZE){
			max = temp->chunk_size - MIN_SIZE - DELIM_SIZE;
		}
		temp = temp->next;
	}
	return max;
}

void *allocate (size_t request, size_t *info){
	static int count = 0;
	if (count == 0){
		count++;
		init();
	}
	if (request > heap_max()){
		*info = 0;
		return NULL;
	} else {
		Node* iterTemp = free_list;
		Node* prevTemp = NULL;
		void* ret = NULL;
		void* delim = NULL;
		while ( request > iterTemp->chunk_size - MIN_SIZE - DELIM_SIZE){
			prevTemp = iterTemp;
			iterTemp = iterTemp->next;
		}
		if (request % 8 == 0){
			if ( iterTemp->chunk_size > 2*MIN_SIZE + request + 2*DELIM_SIZE){
				Node* newAlloc = iterTemp;
				void* shift = iterTemp;
				size_t tempChunk = newAlloc->chunk_size;
				Node* tempNext = newAlloc->next;
				newAlloc->chunk_size = MIN_SIZE + request + DELIM_SIZE;
				shift = shift + newAlloc->chunk_size;
				newAlloc->alloc_size = request;
				newAlloc->next = NULL;
				ret = (void*)newAlloc + MIN_SIZE;
				delim = ret + request;
				long* x = delim;
				*x = 0xfefefefefefefefe;
				if ( prevTemp == NULL ){
					free_list = shift;
				} else {
					prevTemp->next = shift;
				}
				Node* newFree = shift;
				newFree->chunk_size = tempChunk - newAlloc->chunk_size;
				newFree->alloc_size = HEAP_SIZE;
				newFree->next = tempNext;
				*info = newAlloc->chunk_size - MIN_SIZE;
			} else {
				Node* newAlloc = iterTemp;
				newAlloc->alloc_size = request;
				if ( prevTemp == NULL ){
					free_list = newAlloc->next;
				} else {
					prevTemp->next = newAlloc->next;
				}
				newAlloc->next = NULL;
				ret = (void*)newAlloc + MIN_SIZE;
				delim = ret + request;
				long* x = delim;
				*x = 0xfefefefefefefefe;
				*info = newAlloc->chunk_size - MIN_SIZE;
			}
		} else {
			int add = 8 - request % 8;
			if ( iterTemp->chunk_size > 2*MIN_SIZE + request + add + 2*DELIM_SIZE ) {
				Node* newAlloc = iterTemp;
				void* shift = iterTemp;
				size_t tempChunk = newAlloc->chunk_size;
				Node* tempNext = newAlloc->next;
				newAlloc->chunk_size = MIN_SIZE + request + add + DELIM_SIZE;
				shift = shift + newAlloc->chunk_size;
				newAlloc->alloc_size = request;
				newAlloc->next = NULL;
				ret = (void*) newAlloc + MIN_SIZE;
				delim = ret + request;
				long* x = delim;
				*x = 0xfefefefefefefefe;
				if ( prevTemp == NULL ){
					free_list = shift;
				} else {
					prevTemp->next = shift;
				}
				Node* newFree = shift;
				newFree->chunk_size = tempChunk - newAlloc->chunk_size;
				newFree->alloc_size = HEAP_SIZE;
				newFree->next = tempNext;
				*info = newAlloc->chunk_size - MIN_SIZE;
			} else {
				Node* newAlloc = iterTemp;
				newAlloc->alloc_size = request;
				if ( prevTemp == NULL ) {
					free_list = newAlloc->next;
				} else {
					prevTemp->next = newAlloc->next;
				}
				newAlloc->next = NULL;
				ret = (void*) newAlloc + MIN_SIZE;
				delim = ret + request;
				long* x = delim;
				*x = 0xfefefefefefefefe;
				*info = newAlloc->chunk_size - MIN_SIZE;
			}
		}
		return ret;
	}
}

int deallocate(void *ptr){
	if ( ptr == NULL ) {
		return -1;
	}
	void *temp1 = heap_list;
	if ( ptr < temp1 || ptr >= temp1 + HEAP_SIZE){
		return -1;
	}
	Node* temp = heap_list;
	temp1 = temp->chunk_size + temp1;
	while ( ptr > temp1 ) {
		temp = temp1;
		temp1 = temp->chunk_size + temp1;
	}
	void* prevFree = NULL;
	void* nextFree = free_list;
	void* third = NULL;
	Node* iterFree = NULL;
	if ( free_list == NULL || ptr < nextFree ) {
		nextFree = free_list;
	} else {
		iterFree = free_list;
		while ( ptr >= nextFree ){
			third = nextFree;
			prevFree = nextFree;
			nextFree = nextFree + iterFree->chunk_size;
			if ( ptr >= prevFree && ptr < nextFree ){
				return -2;
			}
			prevFree = nextFree;
			nextFree = iterFree->next;
			iterFree = iterFree->next;
		}
	}
	temp1 = temp;
	if ( ptr == temp1 + MIN_SIZE ) {
		long* x = temp1 + MIN_SIZE + temp->alloc_size;
		int ret = 0;
		if ( *x != 0xfefefefefefefefe ){
			ret = -4;
		}
		if ( free_list == NULL ){
			temp->alloc_size = HEAP_SIZE;
			temp->next = NULL;
			free_list = temp;
			return ret;
		}
		Node* tempMerge = NULL;
		void* merge = NULL;
		if ( prevFree == NULL ){
			merge = (void*)temp + temp->chunk_size;
			if ( nextFree == merge ) {
				tempMerge = merge;
				temp->chunk_size = temp->chunk_size + tempMerge->chunk_size;
				temp->alloc_size = HEAP_SIZE;
				temp->next = tempMerge->next;
				free_list = temp;
			} else {
				temp->alloc_size = HEAP_SIZE;
				temp->next = nextFree;
				free_list = temp;
			}
		} else {
			merge = (void*)temp + temp->chunk_size;
			if ( prevFree == temp ) {
				tempMerge = third;
				if ( nextFree == merge ) {
					tempMerge->chunk_size = tempMerge->chunk_size + temp->chunk_size + iterFree->chunk_size;
					tempMerge->alloc_size = HEAP_SIZE;
					tempMerge->next = iterFree->next;
				} else {
					tempMerge->chunk_size = tempMerge->chunk_size + temp->chunk_size;
					tempMerge->alloc_size = HEAP_SIZE;
					tempMerge->next = nextFree;
				}
			} else {
				tempMerge = third;
				if ( nextFree == merge ) {
					temp->chunk_size = temp->chunk_size + iterFree->chunk_size;
					temp->alloc_size = HEAP_SIZE;
					temp->next = iterFree->next;
					tempMerge->next = temp;
				} else {
					temp->alloc_size = HEAP_SIZE;
					temp->next = nextFree;
					tempMerge->next = temp;
				}
			}
		}
		return ret;
	} else {
		return -3;
	}
}
