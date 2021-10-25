/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include "ma_malloc.h"
#include <stdio.h>
#include <stdlib.h>
    


#define MEM_POOL_SIZE 600   //in bytes




//! An enum of statuses that can be used in the header

typedef enum {
    ALLOCATED, FREE
} mem_status;

//! Every item in the pool will have at least a header with size and status of the following data
typedef struct {
    int size;
    mem_status status;
} mem_chunk_header;

typedef unsigned char byte;


static byte mem_pool[MEM_POOL_SIZE];

/**
 * Allocates array of bytes (memory pool) and initializes the memory allocator.
 * If some bytes have been used after calling ma_malloc(size), calling to ma_init() will result in clearing up the memory pool.
 */
 
 
void ma_init() {

	int counter = 0;

	for (int i=0;i<600;i++){
		*((byte*) mem_pool+i) =counter +i;
	}
	
	(*((mem_chunk_header*) &mem_pool)).status = 1; 
	((mem_chunk_header*) &mem_pool) -> size = 600 - sizeof (mem_chunk_header);
	// It is crucial that laer to extract the information of the chunk header that you type 		cast it at all times to (mem_chunk_header)*.	
}


/**
 * Requesting for the tsize bytes from memory pool.
 * If the request is possible, the pointer to the first possible address byte (right after its header) in memory pool is returned.
 */
 
 
 
void* ma_malloc(size tsize) {
	
	int counter= 0;
	
	byte* newChunk= NULL;
	
	while(counter+ tsize< MEM_POOL_SIZE){
		
		byte* ref= &mem_pool[counter];
		
		if (((mem_chunk_header*) ref) -> status == FREE)
		{
			int oldSize = ((mem_chunk_header*) ref) -> size ;
			
			((mem_chunk_header*) ref) -> size = tsize;
			((mem_chunk_header*) ref) -> status = ALLOCATED; 
			 
			newChunk = ref +tsize + sizeof(mem_chunk_header);
			((mem_chunk_header*) newChunk) -> status = FREE; 
			((mem_chunk_header*) newChunk) -> size = oldSize - tsize -sizeof(mem_chunk_header);
			return newChunk + sizeof(mem_chunk_header);///new address after header.
				
		}
		else
		{
			counter = counter + ((mem_chunk_header*) ref) -> size + sizeof(mem_chunk_header);
		}
	}
	 
	return newChunk; 
}


/**
 * Releasing the bytes in memory pool which was hold by ptr, meaning makes those bytes available for other uses.
 * Implement also the coalescing behavior.
 */
 
 
 /*
void ma_free(void *ptr) {

    // add your code here

}
*/


/**
 * This function is only for debugging. It prints out the entire memory pool.
 * Print info on headers and content of all elements in your pool
 */
 
 
 /*
void ma_print(void) {

    // add your code here

}
*/

 
  
