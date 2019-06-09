#include "list.h"

element *create (int d, int par)
{
	element *first = malloc(sizeof(element));
	first -> nr = par;
	element *temp = first;
	
	int i;
	
	for(i=1; i<d; i++)
	{
		element *newElement = malloc(sizeof(element));
		newElement -> nr = par;
		temp -> next = newElement;
		temp = temp -> next;
	}
	
	return first;
}

void deleteList(element *p)
{
	while (p != NULL)
	{
		element *f = p;
		p = p -> next;
		f -> next = NULL;
		free(f);
	}
}

int count (element *p)
{
	element *temp = p;
	int counter;
	counter = 0;
	
	while (temp != NULL)
	{
		counter++;
		temp = temp -> next;
	}
	
	return counter;
}

element *add(element *p, int value)
{
	element *temp = p;
	
	if (p == NULL)
	{
		p = malloc(sizeof(element));
		p -> nr = value;
		p -> next = NULL;
	}
	
	else
	{
		while (temp -> next != NULL)
		{
			temp = temp -> next;
		}
	
		element *newElement = malloc(sizeof(element));
		newElement -> nr = value;
		temp -> next = newElement;
	}
	
	return p;
}

void print(element *p)
{
	element *temp = p;
	
	while (temp != NULL)
	{
		printf("%d, ", temp -> nr);
		temp = temp->next;
	}

	printf("\n");
}


element *deleteElement(element *p, int value)
{
	if (p -> nr == value)
	{
		if (p -> next != NULL)
		{
			element *f = p;
			p = p -> next;
			free(f);
			
			return p;
		}
		
		else
		{
			free(p);
			return NULL;
		}
	}
	
	else
	{
		element *temp = p;
		
		while (temp -> next != NULL && temp -> next -> nr != value)
		{
			temp = temp -> next;
		}
		
		if (temp -> next != NULL)
		{
		
			if (temp -> next -> next != NULL)
			{
				element *t = temp -> next -> next;
				element *f = temp -> next;
				temp -> next = t;
				
				free(f);
			}
			
			else
			{
				element *f = temp -> next;
				temp -> next = NULL;
				
				free(f);
			}
			
			
		}
		return p;
	}
}
