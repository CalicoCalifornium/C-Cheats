// 	gcc -pthread -o threadmain pthreadStuff.c; ./threadmain

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

#define STORAGELEN 100	//	Just a number from the preprocessor. Defines are always written in capital
const size_t storagelen=STORAGELEN;	// A typed constant that can be used elsewhere
int storage[STORAGELEN] = {0};	//	Don't trust your compiler, initialize your variables
size_t productcount = 0;

#define PRODUCERCOUNT 10
pthread_t producers[PRODUCERCOUNT];
const size_t producercount = PRODUCERCOUNT;

int stop = 0;		//	Flag for stopping threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;		// https://www.ibm.com/support/knowledgecenter/en/ssw_aix_71/generalprogramming/mutexes.html

void printArray(char* header, int* array, size_t len);	//	Header for function
size_t findEmpty(int* array, int emptyValue, int len);

void* producer(void* arg) {

	int product = (int)arg;

	for (;;) {

		if (stop) break;		//	Get out of the loop

		pthread_mutex_lock(&mutex);		// Begin critical section. There's also a 'trylock'

		size_t i = findEmpty(storage,0,storagelen);
		if (i <  storagelen) {
			storage[i] = product;
			productcount++;
		}

		pthread_mutex_unlock(&mutex);		//	Mutex unlock

		usleep(rand()%1000 + 1000);		//	Sleep for .999 to 1.999 seconds

	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	size_t i;			//	Because producercount is type size_t also
	for (i = 0; i < producercount; i++) {

		pthread_create(&producers[i],&attr,producer,(void*)(i+1));		//	https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/apis/users_14.htm

	}

	char buf[256];
	gets(buf);
	stop = 1;

	for (i = 0; i < producercount; i++)
		pthread_join(producers[i],NULL);
	
	char header[256];

	sprintf(header, "Storage contents %d", productcount);		//	sprintf prints to string

	printArray(header, storage, storagelen);
	return(0);
}

void printArray(char* header, int* array, size_t len) {

	printf("%s (%d)\n",header,len);

	size_t i;
	for (i = 0; i < len; i++) {

		printf(i%10==0 ?i==0?"" : "\n" : "");			//	Shortened if divived by ten is 0 print \n and if is 0 print "" also
		printf("%d%s",array[i],i<len-1?",":"\n");	// Shortened if not last of array print "," else print "\n"

	}

}

size_t findEmpty(int* array, int emptyValue, int len) {

	size_t i;
	for (i = 0; i < len; i++) {

		if (array[i]==emptyValue) return i;		//	Not found - return len or more because type size_t

	}

	return len;

}