#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//prototype
void arrayLatency(long long int size);

int main(){
	srand(time(NULL));  // Seed random function
	int i=0;
	for(i=2; i < 34; i++){
		arrayLatency(pow(2, i)); 		//call latency function on arrays of increasing size
	}

}

void arrayLatency(long long int size){
	const int NUM_ACCESSES = 1000000000;		//sample sizeo
	const int SECONDS_PER_NS = 1000000000;		//number of ns per second
	struct timespec startAccess, endAccess;		//struct for timer
	int newSize = size / sizeof(int);		//get array size
	int *cacheArray = malloc(size);			//allocate desired number of bytes
	int counter = 0;
	int index=0;
	long double accessTime = 0;
	int read=0;

	clock_gettime(CLOCK_REALTIME, &startAccess); //start clock
	for(counter = 0; counter < NUM_ACCESSES; counter++){
		index = rand() % newSize;				//randomly acces value in array of size newSize
		cacheArray[index];
	}
	clock_gettime(CLOCK_REALTIME, &endAccess); //end clock
	//calculate the time elapsed in ns per access
	accessTime = (((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec)) / NUM_ACCESSES;
	printf("Access time: %Lf for size %lld\n", accessTime, size);
	free(cacheArray);	//free array
}