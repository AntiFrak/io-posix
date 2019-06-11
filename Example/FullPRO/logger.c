void logger() {
	printf("Res: %d WRoom: %d/%d [in: %d]\n", resigned, currentlyInWRoom, numOfChairs, custInChair);
	if (debug == 1) {
		printf("\tRes: ");
		if (resigned != 0) {
			int i;
			for (i = 0; i < resigned; i++) {
				printf("%d ", resClients[i]);
			}
		}
		else {
			printf("-");
		}

		printf("\n\tWRoom: ");
		printWRoomList();
	}
	printf("\n");
}