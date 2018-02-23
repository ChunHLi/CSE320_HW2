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
	
	typedef struct block{ 
		uint64_t header;
		uint64_t payload[124]; //(1024-16-16)/8 max size of ram		
		uint64_t footer;
	} block;

	block ramArray[32]; 	
	/*
	int i;
	for (i=0;i<512;i++){
		if (i % 8 == 0){
			printf("\n");
		}
		printf("%02x",((uint8_t*)ram)[i]);
	}
	*/
	//printf("%"PRIu64"\n",((uint64_t*)ram)[0]);
	//printf("%"PRIu64"\n",((uint64_t*)ram)[1]);
    /*
     * Do not modify code below.
     */
    cse320_check();
    cse320_free();
    return ret;
}
