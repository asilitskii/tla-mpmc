#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

uint32_t max_capacity, numProducers, numConsumers;
uint32_t *buffer;
uint32_t fillIndex = 0, takeIndex = 0, count = 0;

pthread_cond_t modify;
pthread_mutex_t mutex;

void append(uint32_t value) {
	buffer[fillIndex] = value;
	fillIndex = (fillIndex + 1) % max_capacity;
	count++;
}

uint32_t take() {
	uint32_t tmp = buffer[takeIndex];
	takeIndex = (takeIndex + 1) % max_capacity;
	count--;
	return tmp;
}

void *producer (void * arg) {
	while(1) {
		pthread_mutex_lock(&mutex);   // acquire the lock	

		while (count == max_capacity) {   // check if the buffer is full
		    pthread_cond_wait(&modify, &mutex);
		}
		
		append(rand() % (10));        // produce!

		pthread_cond_signal(&modify); // signal that the buffer is modified
        pthread_mutex_unlock(&mutex); // release the lock
	}
}

void *consumer (void * arg) {
	uint32_t id = *((uint32_t *) arg);
	long report = 0;
	while(1) {
		pthread_mutex_lock(&mutex);   // acquire the lock

		while (count == 0) {           // check if the buffer is empty
			pthread_cond_wait(&modify, &mutex); // wait for the buffer to be filled
		}

		take();                       // consume (we don't care about the value)!

		pthread_cond_signal(&modify); // signal that the buffer is modified
		pthread_mutex_unlock(&mutex); // release the lock

		if (report++ % 10000 == 0) {
    		printf("\n%ld values consumed by %d", report, id);
		}
	} 
}

int main(int argc, char * argv[]) {
	if (argc < 4) {
		printf ("Usage: ./producer_consumer <buffer_size> <#_of_producers> <#_of_consumers> \n");
		exit(1);
	}

	srand(999);

	/* Process arguments */
	max_capacity = atoi(argv[1]);
	numProducers = atoi(argv[2]);
	numConsumers = atoi(argv[3]);

	printf("Buffer capacity = %d, Producers = %d, Consumers = %d\n", max_capacity, numProducers, numConsumers);

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&modify, NULL);

	/* Allocate space for the buffer */
	buffer = malloc(sizeof(int) * max_capacity);
	pthread_t prods[numProducers], cons[numConsumers];
	uint32_t threadIds[numConsumers];

	uint32_t i;
	/* Create the producers */
	for (i = 0; i < numProducers ; i++) {
		pthread_create(&prods[i], NULL, producer, NULL);
	}

	/* Create the consumers */
	for (i = 0; i < numConsumers; i++) {
		threadIds[i] = i;
		pthread_create(&cons[i], NULL, consumer, &threadIds[i]);
	}

	/* Wait for all threads to finish */
	for (i = 0; i < numProducers ; i++) 
		pthread_join(prods[i], NULL);	

	for (i = 0; i < numConsumers; i++)
		pthread_join(cons[i], NULL);

	return 0;
}
