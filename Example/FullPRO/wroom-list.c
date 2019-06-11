typedef struct node {
	int val;
	struct node *next;
} node_t;

node_t *head = NULL;

// dodanie klienta do listy osob w poczekalni
void pushToWRoomList(int num) {
	if (head == NULL) {
		head = malloc(sizeof(node_t));
		if (head == NULL) {
			fprintf(stderr, "Error preparing WRoom list");
			exit(EXIT_FAILURE);
		}

		head->val = num;
		head->next = NULL;
	}
	else {
		node_t *current = head;
		while (current->next != NULL) {
			current = current->next;
		}

		current->next = malloc(sizeof(node_t));
		current->next->val = num;
		current->next->next = NULL;
	}
}

// usuniecie klienta z listy osob w poczekalni
void removeFromWRoomList(int num) {
	node_t *current = head;
	node_t *temp_node = NULL;

	if (head->val == num) {
		if (head->next == NULL) {
			head = NULL;
		}
		else {
			head = head->next;
		}
	}
	else {
		while (current->next != NULL) {
			if (current->val == num) {
				temp_node = current->next;
				current->next = temp_node->next;
				free(temp_node);
				break;
			}
			current = current->next;
		}
	}
}

// drukowanie listy osob w poczekalni
void printWRoomList() {
	node_t *current = head;

	if (current == NULL) {
		printf("-");
	}

	while (current != NULL) {
		printf("%d ", current->val);
		current = current->next;
	}
	printf("\n");
}
