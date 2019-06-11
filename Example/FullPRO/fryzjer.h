#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "ticket_lock.c"
#include "wroom-list.c"

void logger();
void prepareResClients();
void addResignedClient(int);

void printWRoomList();
void pushToWRoomList(int);
void removeFromWRoomList(int);

void mutex_style();
void *barber();
void *customer(void *);

void conditional_style();
void ticket_lock(ticket_lock_t *ticket);
void ticket_unlock(ticket_lock_t *ticket);
void *sleepingBarber();
void *waitingRoom(void *);

volatile int debug = 0;
volatile int condv = 0;

int *resClients; // tablica przechowujaca liste klientow, ktorzy zrezygnowali z wizyty
int resClientsSize = 10; // zmienna przechowujaca aktualny rozmiar listy Res
volatile int resigned = 0; // liczba klientow ktorzy zrezygnowali z wizyty

volatile int numOfChairs; // liczba krzesel w poczekalni
volatile int currentlyInWRoom = 0; // aktualna liczba klientow w poczekalni

volatile int lastCustNr = 0; // flaga przechowujaca numer ostatnio stworzonego klienta
volatile int custInChair = 0; // flaga przechowujaca numer klienta w gabinecie
volatile int served = 1; // flaga, 'czy ostatni klient obsluzony'

volatile int sleeping = 0;

pthread_t barberThread;

// mutex_style >>
sem_t customers;
sem_t barbers;
sem_t mutex;
sem_t chair;
// mutex_style <<

// conditional_style >>
pthread_cond_t sleepingBarber_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t workingBarber_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sleepMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t waitMutex = PTHREAD_MUTEX_INITIALIZER;
ticket_lock_t queueMutex = TICKET_LOCK_INITIALIZER;
ticket_lock_t queueFIFOMutex = TICKET_LOCK_INITIALIZER;
// conditional_style <<

#include "mutex_style.c"
#include "logger.c"
#include "resigned_clients.c"
#include "conditional_style.c"