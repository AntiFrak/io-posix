typedef struct ticket_lock_t {
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	unsigned long queue_head, queue_tail;
} ticket_lock_t;

#define TICKET_LOCK_INITIALIZER { PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER }

void ticket_lock(ticket_lock_t *ticket)
{
	unsigned long queue_me;

	pthread_mutex_lock(&ticket->mutex);
	queue_me = ticket->queue_tail++;
	while (queue_me != ticket->queue_head)
	{
		pthread_cond_wait(&ticket->cond, &ticket->mutex);
	}
	pthread_mutex_unlock(&ticket->mutex);
}

void ticket_unlock(ticket_lock_t *ticket)
{
	pthread_mutex_lock(&ticket->mutex);
	ticket->queue_head++;
	pthread_cond_broadcast(&ticket->cond);
	pthread_mutex_unlock(&ticket->mutex);
}