#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	srand(time(NULL));  // Seed ONCE
	const int L1_CACHE_SIZE =  32768/sizeof(int);
	const int L2_CACHE_SIZE =  262144/sizeof(int);
	const int L3_CACHE_SIZE =  6587392/sizeof(int);
	const int SECONDS_PER_NS = 1000000000;
	int* arrayAccess = calloc(L1_CACHE_SIZE,sizeof(int));
	int* arrayInvalidateL1= calloc(L1_CACHE_SIZE,sizeof(int));	//arrays for invalidating cache data
	int* arrayInvalidateL2= calloc(L2_CACHE_SIZE,sizeof(int));
	int* arrayInvalidateL3= calloc(L3_CACHE_SIZE,sizeof(int));
	int count=0;
	int index=0;
	int i=0;
	struct timespec startAccess, endAccess;
	double mainMemAccess, L1Access, L2Access, L3Access;
	int readValue=0;


	index = 0;
	count=0;
	clock_gettime(CLOCK_REALTIME, &startAccess); //start clock
	while (index < L1_CACHE_SIZE) {
	    int tmp = arrayAccess[index];               //Access Value from Main Memory
	    index = (index + tmp + ((index & 4) ? 28 : 36));   //variable based strides of 32
	    count++;                                           //number of accesses 
	}
	clock_gettime(CLOCK_REALTIME, &endAccess); //end clock
	mainMemAccess = ((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec);
	mainMemAccess /= count;

	printf("Main Memory Access %lf\n", mainMemAccess);

	index = 0;
	count=0;
	clock_gettime(CLOCK_REALTIME, &startAccess); //start clock
	while (index < L1_CACHE_SIZE) {
	    int tmp = arrayAccess[index];               //Access Value from L1
	    index = (index + tmp + ((index & 4) ? 28 : 36));   //variable based strides of 32
	    count++;                                           //number of accesses
	}
	clock_gettime(CLOCK_REALTIME, &endAccess); //end clock				
	L1Access = ((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec);
	L1Access /= count;

	printf("L1 Cache Access %lf\n", L1Access);

	//invalidate L1 by accessing all elements of array which is larger than cache
	for(count=0; count < L1_CACHE_SIZE; count++){
		int read = arrayInvalidateL1[count]; 
		read++;
		readValue+=read;               
	}

	index = 0;
	count = 0;
	clock_gettime(CLOCK_REALTIME, &startAccess); //start clock
	while (index < L1_CACHE_SIZE) {
	    int tmp = arrayAccess[index];               //Access Value from L2
	    index = (index + tmp + ((index & 4) ? 28 : 36));   //variable based strides of 32
	    count++;                                           //number of accesses
	}
	clock_gettime(CLOCK_REALTIME, &endAccess); //end clock
	L2Access = ((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec);
	L2Access /= count;

	printf("L2 Cache Acces %lf\n", L2Access);

	//invalidate L2 by accessing all elements of array which is larger than cache
	for(count=0; count < L2_CACHE_SIZE; count++){
		int read = arrayInvalidateL2[count];  
		read++;
		readValue+=read;                        
	}

	index = 0;
	count=0;
	clock_gettime(CLOCK_REALTIME, &startAccess);
	while (index < L1_CACHE_SIZE) {
	    int tmp = arrayAccess[index];               //Access Value from L3
	    index = (index + tmp + ((index & 4) ? 28 : 36));   //variable based strides of 32
	    count++;                                           //number of accesses
	}
	clock_gettime(CLOCK_REALTIME, &endAccess); //end clock
	L3Access = ((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec);
	L3Access /= count;

	printf("L3 Cache Access %lf\n", L3Access);

	printf("Read Value: %d", readValue);

}