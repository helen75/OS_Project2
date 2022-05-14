#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#define NO_OF_THREADS 10

struct Singleton {
   char *Data;	
};


struct Singleton *singletonObjectPointer;


int addresses[NO_OF_THREADS];
sem_t sem;

void *runner(void *params); /* the thread */
struct Singleton *getInstance();

int main()
{
	int i;
	sem_init(&sem,0,1);
	pthread_t threadIds[NO_OF_THREADS];
	

	for (i=0; i < NO_OF_THREADS; i++){
		pthread_create(&threadIds[i], NULL, &runner, (void *)(i));
	} 

	/* Wait until all threads are done */
	for (i=0; i < NO_OF_THREADS; i++){
		pthread_join(threadIds[i], NULL);
	} 
	
	/* Control addresses. All of them should be same */
	int prevAddr=addresses[0];
	for (i=1; i < NO_OF_THREADS; i++){
		if(addresses[i]!=prevAddr){
			printf("Object is created more than once\n");
			return -1;
		}
		prevAddr=addresses[i];
	}
	for (i=0; i < NO_OF_THREADS; i++){
		printf("Singleton Addresses for each thread %x\n",addresses[i]);
	}
	printf("Successful\n");
	return 1;
}

/**
 * The thread will begin control in this function
 */
void *runner(void *params) 
{
	int i = (int)params;
	printf("Thread %d\n",i);
	struct Singleton *s = getInstance();
	addresses[i]=s;
	pthread_exit(0);
}

//Fill this method
struct Singleton *getInstance(){
	
	if(singletonObjectPointer == NULL){ 	
		sem_wait(&sem); 		
		
		if (singletonObjectPointer == NULL){ 
		
				singletonObjectPointer = (struct Singleton *)malloc(sizeof(struct Singleton)); 
				printf("---Address of singletonObjectPointer is %x\n",singletonObjectPointer);
				singletonObjectPointer->Data="This is object data"; }
				
			sem_post(&sem);	}	

	return singletonObjectPointer; }
	
	
/*    
 	if this code was in java:
 	
 	if(singletonObjectPointer == NULL) {
 	
 		synchronized(Singleton.class) {
 		
 			if(singletonObjectPointer == NULL) {
 				singletonObjectPointer = new Singleton();}}}
 				
 	return singletonObjectPointer
 	
 	
 	
 	-- This "synchronized" part locks the second if part. But we cannot use it in C. 
 	-- So I used wait and post to locked that part. 
 	-- After the first thread go in wait, others cannot go inside that part until first one post it. 
 	-- And after the thread finised it's job, it goes out and post it then return with an instance. 
	-- SingletonObjectPointer is not NULL anymore.
 	-- After that a waiting thread goes in but because of the first thread, It cannot go in second if. It just post and return.
 	-- Other remaining threads do the same thing and that makes this: "getInstance method with double-check locking in C" 		

*/  



