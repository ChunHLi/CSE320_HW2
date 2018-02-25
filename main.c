#include "helper.h"
#include <inttypes.h>

int main(int argc, char** argv) {
    if (*(argv + 1) == NULL) {
        printf("You should provide name of the test file.\n");
        return 1;
    }
    void* ram = cse320_init(*(argv + 1));
    void* tmp_buf = cse320_tmp_buffer_init();
    int ret = 0;
    /*
     * You code goes below. Do not modify provided parts
     * of the code.
     */
	/*
	 * uint64_t orders the bits from highest to lowest bit
	 * the ram orders the bytes from lowest to highest byte
	 * size is in words
	 */
	
	/*
	 * struct to store the block's header, payload and footer
	 * since each block can hold a min of 32 bytes and the max
	 * size of the ram is 1024 bytes, we initialize the ramArray
	 * to 32;
	 * no blocks will have the same size, application id and allocation flag
	 */
	typedef struct block{ 
		uint64_t header;
		uint64_t payload[124]; //(1024-16-16)/8 max size of ram		
		uint64_t footer;
		int size;
		int id;
		int flag;
	} block;
	
	block ramArray[32]; 
	
	int numBlock = 0;	
 	
	if (ram == NULL){
		printf("INIT_ERROR\n");
    	}
	/*
	 * Storing the ram into an array to make it easier to access and sort data
	 */
	
	int i;
	for (i=0; i<32; i++){
		ramArray[i].header = 0;
		int j;
		for (j=0;j<124;j++){
			ramArray[i].payload[j] = 0;	
		}
		ramArray[i].footer = 0;
	}	
	int j = 0;
	int k = 0;
	while (((uint64_t*)ram)[k]!=0){
		uint64_t h = ((uint64_t*)ram)[k];
		ramArray[j].header = h;
		//printf("%"PRIu64"\n",ramArray[j].header);
		ramArray[j].id = h & 0x01;
		//printf("%d\n",ramArray[j].id);
		ramArray[j].flag = (h>>1) & 0x03;
		//printf("%d\n",ramArray[j].flag);
		ramArray[j].size = h>>3;
		h >>= 3;
		k++;
		int l;
		for (l = 1; l < h-1; l++){
			ramArray[j].payload[l-1] = ((uint64_t*)ram)[k];		
			//printf("%"PRIu64"\n",ramArray[j].payload[l-1]);
			k++;
		}
		ramArray[j].footer = ((uint64_t*)ram)[k];
		//printf("%"PRIu64"\n",ramArray[j].footer);
		k++;
		j++;
	}

	numBlock = j;
	//printf("Number of blocks recorded: %d",numBlock);
	
	/*
	 * Sorting specifications
	 * 1. Application ID  
	 * 2. Allocated blocks before free blocks
	 * 3. Blocks with lowest size goes first
	 */
	
	/* sorting via Application ID first */
	/* sorting via Allocated blocks */
	/* sorting via lowest size blocks */

	/*
	 * Adjacent free blocks must be coalesced if they are part of the same application
	 */

	/*
	 * Block with size of 16 bytes to be added
	 */

    /*
     * Do not modify code below.
     */
    cse320_check();
    cse320_free();
    return ret;
}
