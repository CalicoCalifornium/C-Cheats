// 	gcc -pthread -o threadmain main.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define STORAGELEN 100	//	Just a number from the preprocessor. Defines are always written in capital
const size_t storagelen=STORAGELEN;	// A typed constant that can be used elsewhere
int storage[STORAGELEN];

#define PRODUCERCOUNT 10
pthread_t producers[PRODUCERCOUNT];
const size_t producercount = PRODUCERCOUNT;

void printArray(char* header, int* array, size_t len) {

	printf("%s (%d)\n",header,len);

	size_t i;
	for (i = 0; i < len; i++) {

		printf(i%10==0 ?i==0?"" : "\n" : "");			//	Shortened if divived by ten is 0 print \n and if is 0 print "" also
		printf("%d%s",array[i],i<len-1?",":"\n");	// Shortened if not last of array print "," else print "\n"

	}

}

void* producer(void* arg) {
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	size_t i;			//	Because producercount is type size_t also
	for (i = 0; i < producercount; i++) {

		pthread_create(&producers[i],&attr,producer,(void*)(i+1));		//	https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/apis/users_14.htm

	}

	printArray("Storage contents", storage, storagelen);
	return(0);
}