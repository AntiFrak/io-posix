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

int chairs,clientsNumber=0,resigned=0,currentClient=-1;
bool debug=false;

element *waiting=NULL;
element *resign=NULL;

void *barb()
{
	srand(time(NULL));
	int i;
	
	while(1)
	{
		if(waiting!=NULL)
			sem_post(&clients[waiting->nr]);	//fryzjer jest budzony gdy są klienci

		sem_wait(&sem);							//fryzjer zasypia
		pthread_mutex_unlock(&queue);			//odblokowanie kolejki klientów
		sem_wait(&barber);						//fryzjer czeka na gotowość klienta
		
		for(i=0;i<rand()%10000+1000;i++)
		{
			//strzyżenie
		}

		pthread_mutex_lock(&clientQueue);	//blokowanie poczekalni
		pthread_mutex_lock(&communicate);	//blokada zmiennych
		printf("Koniec strzyzenia klienta %d.\n",currentClient);
		fflush(stdout);
		pthread_mutex_unlock(&communicate);
		currentClient=-1;
		pthread_mutex_unlock(&clientQueue);	//odblokowanie poczekalni
		sem_post(&shearing);				//koniec strzyżenia
	}
}

void *client(void *arg)
{
	int i;
	int numer=(int)arg;
	srand(time(NULL));
	
	for(i=0;i<rand();i++)
	{
	//opóźnienie przyjścia
	}

	pthread_mutex_lock(&clientQueue);
	if(currentClient==-1 && clientsNumber==0)	
	{
		printf("Klient %d wchodzi do gabinetu od razu.\n",numer);
		currentClient=numer;			
		sem_post(&sem);						//budzenie fryzjera
		sem_wait(&clients[numer]);			//klient czeka na wpuszczenie
		pthread_mutex_lock(&communicate);	//blokada wypisywanych zmiennych
		
		printf("Res: %d WRomm: %d/%d [in: %d]\n",resigned,clientsNumber,chairs,currentClient);
		fflush(stdout);
		pthread_mutex_unlock(&communicate);	//zwolnienie blokady
		pthread_mutex_lock(&communicate);	//blokada wypisywanych zmiennych
		
		printf("Klient %d siada na fotel.\n",numer);
		printf("Res: %d WRomm: %d/%d [in: %d]\n",resigned,clientsNumber,chairs,currentClient);
		fflush(stdout);
		pthread_mutex_unlock(&communicate);	//zwolnienie blokady
		pthread_mutex_unlock(&clientQueue);	//zwolnienie blokady z poczekalni
		pthread_mutex_lock(&queue);			//zablokowanie kolejki wątków
		
		sem_post(&barber);		//klient jest gotowy
		sem_wait(&shearing);	//klient czeka na zakonczenie strzyzenia
	}
	else
	if(clientsNumber<chairs)
	{
		waiting=add(waiting,numer);	//zajecie miejsca w poczekalni
		clientsNumber++;
		printf("Klient %d wchodzi do poczekalni.\n",numer);

		if(debug)
		{
			printf("POCZEKALNIA: ");
			print(waiting);
			fflush(stdout);
		}

		sem_post(&sem);				//budzenie fryzjera
		sem_wait(&clients[numer]);	//czekanie na wpuszczenie
		
		pthread_mutex_lock(&communicate);
		printf("Res: %d WRomm: %d/%d [in: %d]\n",resigned,clientsNumber,chairs,currentClient);
		fflush(stdout);
		pthread_mutex_unlock(&communicate);
		pthread_mutex_unlock(&clientQueue);				//odblokowanie poczekalni
		pthread_mutex_lock(&queue);						//zablokuj kolejkę wątków
		pthread_mutex_lock(&clientQueue);				//zablokuj poczekalnię
		currentClient=numer;							//aktualnie strzyżonym klientem będzie ten klient
		waiting=deleteElement(waiting,currentClient);	//przejscie do gabinetu
		clientsNumber--;								//zmiana liczby klientow

		if(debug)
		{
			printf("POCZEKALNIA: ");
			print(waiting);
			fflush(stdout);
		}

		pthread_mutex_unlock(&clientQueue);				//odblokowanie poczekalni
		printf("Klient %d siada na fotel.\n",numer);
		pthread_mutex_lock(&communicate);				//j.w.
		printf("Res: %d WRomm: %d/%d [in: %d]\n",resigned,clientsNumber,chairs,currentClient);
		fflush(stdout);
		pthread_mutex_unlock(&communicate);
		sem_post(&barber);			//klient gotowy do strzyżenia
		sem_wait(&shearing);		//czekanie na zakończenie strzyżenia
	}
	else
	{
		pthread_mutex_lock(&resignedClients);					//zablokuj listę klientów,którzy zrezygnowali
		resigned++;					
		resign=add(resign,numer);								//dodanie klienta do listy klientów,którzy zrezygnowali
		printf("Klient %d rezygnuje ze strzyzenia.\n",numer);
		pthread_mutex_unlock(&clientQueue);						//odblokuj poczekalnię
		if(debug)
		{
			printf("Lista klientow,ktorzy zrezygnowali: ");
			print(resign);
			fflush(stdout);
		}
		pthread_mutex_unlock(&resignedClients);		//odblokuj listę i liczbę klientów,którzy zrezygnowali
		pthread_mutex_lock(&communicate);		
		printf("Res: %d WRomm: %d/%d [in: %d]\n",resigned,clientsNumber,chairs,currentClient);
		fflush(stdout);
		pthread_mutex_unlock(&communicate);
	}
}

int main(int argc,char **argv)
{
	int a,b;

	if(argv[1]!=NULL)
		sscanf(argv[1],"%d",&a);
	else
	{
		printf("Blad! Brak parametrów!\n");
		return (EXIT_FAILURE);
	}

	if(argv[2]!=NULL)
		sscanf(argv[2],"%d",&b);
	else
	{
		printf("Blad! Brak liczby krzeseł!\n");
		return (EXIT_FAILURE);
	}

	if(argv[3]!=NULL)
		if(strcmp(argv[3],"-debug")==0)
			debug=true;
	if(a>300)
		a=300;

	pthread_t klienci[a],Fryzjer;
	chairs=b;
	sem_init(&sem,0,0);
	sem_init(&shearing,0,0);
	sem_init(&barber,0,0);
	int i;

	for(i=0;i<a;i++)
		sem_init(&clients[i],0,1);

	pthread_mutex_init(&queue,NULL);
	pthread_mutex_init(&communicate,NULL);
	pthread_mutex_init(&resignedClients,NULL);
	pthread_mutex_init(&clientQueue,NULL);
	pthread_mutex_lock(&queue);
	pthread_create(&Fryzjer,NULL,barb,NULL);

	for(i=0;i<a;i++)
		pthread_create(&klienci[i],NULL,client,(void *)i);

	pthread_join(Fryzjer,NULL);

	for(i=0;i<a;i++)
		pthread_join(klienci[i],NULL);
	
	pthread_kill(Fryzjer,SIGKILL);
	pthread_mutex_destroy(&queue);
	pthread_mutex_destroy(&communicate);
	pthread_mutex_destroy(&resignedClients);
	pthread_mutex_destroy(&clientQueue);

	deleteList(waiting);
	deleteList(resign);

	return 0;
}
