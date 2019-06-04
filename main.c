#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "list.h"

sem_t sem;					//semafor fryzjera
sem_t clients[300];			//semafor klientów
sem_t shearing;				//semafor fotela
sem_t barber;				//semafor strzyżenia

pthread_mutex_t resignedClients;	//muteks klientów,którzy zrezygnowali
pthread_mutex_t clientQueue;		//muteks kolejki klientów oraz aktualnego klienta
pthread_mutex_t queue;				//muteks klientów
pthread_mutex_t communicate;		//muteks wypisywanych zmiennych

int chairs, clientsNumber = 0, resigned = 0, currentClient = -1;
bool debug = false;

element *waiting = NULL;
element *resign = NULL;