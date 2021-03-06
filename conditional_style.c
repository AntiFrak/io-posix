void conditional_style() {
	prepareResClients();

	pthread_t customerThread;
	int thread;

	// tworzenie watku fryzjera
	thread = pthread_create(&barberThread, NULL, sleepingBarber, NULL);
	if (thread != 0) {
		fprintf(stderr, "blad przy tworzeniu watkow fryzjera!");
		exit(EXIT_FAILURE);
	}

	// nieskonczone tworzenie watkow klientow
	while (1)
	{
		lastCustNr++;

		int *custNr = malloc(sizeof(*custNr));
		if (custNr == NULL)
		{
			fprintf(stderr, "blad przedzielenia pamieci dla nast klienta!");
			exit(EXIT_FAILURE);
		}

		*custNr = lastCustNr;
		thread = pthread_create(&customerThread, NULL, waitingRoom, custNr);
		if (thread != 0)
		{
			fprintf(stderr, "blad przy tworzeniu watkow fryzjera!");
			exit(EXIT_FAILURE);
		}

		int rnd = rand() % 4;
		usleep(rnd * 500 * 1000); // oczekiwanie na przyjscie kolejnego klienta
	}

	// zabezpieczenie przed deadlockiem
	pthread_join(barberThread, NULL);
	pthread_join(customerThread, NULL);
}

// funkcja obslugujaca watek fryzjera
void *sleepingBarber() {
	while (1) {
        // sekcja krytyczna do sprawdzenia zmiennej currentlyInWRoom
		pthread_mutex_lock(&waitMutex);
		if (currentlyInWRoom == 0) {
			pthread_mutex_unlock(&waitMutex);
			
			pthread_mutex_lock(&sleepMutex); // brak klientow, fryzjer spi
			sleeping = 1;

			// oczekiwanie na klienta
			pthread_cond_wait(&sleepingBarber_cond, &sleepMutex);

			// klient przyszedł, wybudzenie fryzjera
			sleeping = 0;
			pthread_mutex_unlock(&sleepMutex);
		}
		else {
			pthread_mutex_unlock(&waitMutex);

			int rnd = rand() % 3;
			usleep(rnd * 500 * 1000); // strzyzenie

			// koniec strzyzenia, wyslanie sygnalu o zwolnieniu miejsca u fryzjera
			pthread_cond_signal(&workingBarber_cond);
		}
	}
	pthread_exit(0);
}

// poczekalnia
void *waitingRoom(void *number) {
	int num = *(int *)number;

	// lock na mutexie poczekalni (sprawdzac moze tylko jeden klient na raz)
	ticket_lock(&queueMutex);

	// klient sprawdza wolne miejsca, jesli nie ma -> rezygnuje
	if (currentlyInWRoom == numOfChairs) {
		addResignedClient(num);
		log();
		ticket_unlock(&queueMutex); // wyjscie z obszaru krytycznego
	}
	else { // byly miejsca w poczekalni
		currentlyInWRoom++;
		pushToWRoomList(num);
		log();

		if (sleeping == 1) { // sprawdzanie czy fryzjer spi
			pthread_cond_signal(&sleepingBarber_cond); // klient go budzi
		}

		// klient zwalnia queueMutex - poczekalnie moze sprawdzic kolejny klient
		ticket_unlock(&queueMutex);

		// klient zajmuje miejsce na końcu kolejki
		ticket_lock(&queueFIFOMutex);
		pthread_mutex_lock(&waitMutex);
		ticket_unlock(&queueFIFOMutex); // moze to zwolnic klient z odpowiednim ticketem

		// klient czeka na zwolnienie miejsca u fryzjera
		pthread_cond_wait(&workingBarber_cond, &waitMutex);
		custInChair = num;
		removeFromWRoomList(num);
		currentlyInWRoom--;
		log();
        
        // oczekiwanie na skonczenie strzyzenia
		pthread_cond_wait(&workingBarber_cond, &waitMutex);
		custInChair = 0;
		log();
		pthread_mutex_unlock(&waitMutex);
	}
	free(number);
}
