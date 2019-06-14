// zablokowanie pamieci na liste zrezygnowanych
void prepareResClients() {
	resClients = (int *)malloc(resClientsSize * sizeof(int));
	if (resClients == NULL) {
		fprintf(stderr, "blad przedzielenia pameci dla kilentow zrezygnowanych");
		exit(EXIT_FAILURE);
	}
}

// dodanie klienta do listy zrezygnowanych
void addResignedClient(int num) {
	if (resigned == resClientsSize) {
		resClientsSize += 10;
		resClients = realloc(resClients, resClientsSize * sizeof(int));
		if (resClients == NULL) {
			fprintf(stderr, "blad pamieci przy dodawaniu klienta do listy zrezygnowanych");
			exit(EXIT_FAILURE);
		}
	}

	resClients[resigned] = num;
	resigned++;
}