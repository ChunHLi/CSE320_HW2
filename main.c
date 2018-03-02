#include "helper.h"
#include <inttypes.h>
#include <errno.h>

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
	if (ram == NULL){
		errno = ENOENT;
		printf("INIT_ERROR\n");
		exit(errno);
	}
	void* ptr = cse320_sbrk(1024-128);
	if (ptr == NULL){
		errno = ENOENT;
		printf("SBRK_ERROR\n");
		exit(errno);
	}
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
	/*for (i=0;((uint64_t*)ram)[i] != 0 || ((uint64_t*)ram)[i+1] != 0;i++){
		int j;
		for (j=0; j<8; j++){
			printf("%02x",((uint8_t*)ram)[i*8+j]);
		}
		printf("\n");
	}*/
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
	while (((uint64_t*)ram)[k]+((uint64_t*)ram)[k+1]!=0){
		uint64_t h = ((uint64_t*)ram)[k];
		ramArray[j].header = h;
		//printf("%"PRIu64"\n",ramArray[j].header);
		ramArray[j].flag = h & 0x01;
		//printf("%d\n",ramArray[j].id);
		ramArray[j].id = (h>>1) & 0x03;
		//printf("%d\n",ramArray[j].flag);
		ramArray[j].size = h>>3;
		//printf("%"PRIu64"\n",((uint64_t)ramArray[j].size)<<3);
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
	
	void swap(block *x, block *y){
    		block tmp = *x;
    		*x = *y;
    		*y = tmp;
	}

	/* sorting via Application ID first */
	if (numBlock < 2){
	} else {
		/* sorting via Application ID first */		
		/* sorting via Allocated blocks */
		/* sorting via lowest size blocks */
		for (i = 0; i < numBlock - 1; i++){
			int min = i;
			for(j = i + 1; j < numBlock; j++){
				if (ramArray[j].id < ramArray[min].id){
					min = j;
				} else if (ramArray[j].id == ramArray[min].id){
					if (ramArray[j].flag > ramArray[min].flag){
						min = j;
					} else if (ramArray[j].flag == ramArray[min].flag){
						if (ramArray[j].size < ramArray[min].size){
							min = j;
						}
					}	
				}
			}
			swap(&ramArray[min],&ramArray[i]);	
		}
		/*
	 	* Adjacent free blocks must be coalesced if they are part of the same application
	 	*/
	}
	i = 0;
	while (i < numBlock - 1){
		if ((ramArray[i].flag == 0 && ramArray[i + 1].flag == 0) &&
			(ramArray[i].id == ramArray[i + 1].id)){
			ramArray[i].size = ramArray[i].size + ramArray[i + 1].size;
			//printf("%"PRIu64"\n",((uint64_t)ramArray[i].size)<<3);
			ramArray[i].header = (((uint64_t)ramArray[i].size)<<3) | (((uint64_t)ramArray[i].id)<<1);
			//printf("%"PRIu64"\n",ramArray[i].header);
			int l = 0;
			while (ramArray[i].payload[l] != 0){
				l++;
			}
			ramArray[i].payload[l] = ramArray[i].footer;
			l++;
			ramArray[i].payload[l] = ramArray[i + 1].header;
			l++;
			int n = 0;
			while (ramArray[i + 1].payload[n] != 0){
				ramArray[i].payload[l] = ramArray[i + 1].payload[n];
				l++;
				n++;
			}
			
			ramArray[i].footer = (((uint64_t)ramArray[i].size)<<3) | (((uint64_t)ramArray[i].id)<<1);
			int m;
			for (m = i + 1; m < numBlock; m++){
				ramArray[m] = ramArray[m+1];
			}
			i--;
			numBlock--;
		}
		i++;
	}
	//printf("%d\n",numBlock);
	ramArray[numBlock].size = 2;
	ramArray[numBlock].header = 0;
	ramArray[numBlock].header = ((uint64_t)ramArray[numBlock].size)<<3;
	//printf("%"PRIu64"\n",(ramArray[numBlock].header)>>3);
	ramArray[numBlock].footer = ((uint64_t)ramArray[numBlock].size)<<3;
	/*
	* Block with size of 16 bytes to be added
	*/
	/*for (i = 0; i < numBlock; i++){
		printf("\n");
		printf("Application ID: %d\n", ramArray[i].id);
		printf("Allocation Flag: %d\n", ramArray[i].flag);
		printf("Block Size: %d\n", ramArray[i].size);
	}*/
	int r;
	int s = 0;
	for (r = 0; ramArray[r].size != 0; r++){	
		((uint64_t*)ram)[s] = ramArray[r].header;
		//printf("%"PRIu64"\n",((uint64_t*)ram)[s]);
		s++;
		int q;
		for (q = 0; q < ramArray[r].size - 2; q++){
			//printf("%"PRIu64"\n",ramArray[r].payload[q]);
			((uint64_t*)ram)[s] = ramArray[r].payload[q];
			s++;
		}
		((uint64_t*)ram)[s] = ramArray[r].footer;
		//printf("%"PRIu64"\n",((uint64_t*)ram)[s]);
		s++;
	}
	/*
	printf("\n");

	for (i=0;((uint64_t*)ram)[i] != 0 || ((uint64_t*)ram)[i+1] != 0;i++){
		int j;
		for (j=0; j<8; j++){
			printf("%02x",((uint8_t*)ram)[i*8+j]);
		}
		printf("\n");
	}
	*/	
    /*
     * Do not modify code below.
     */
    cse320_check();
    cse320_free();
    return ret;
}
