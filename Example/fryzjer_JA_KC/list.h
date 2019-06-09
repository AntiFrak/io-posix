#ifndef list
#define list
#include <stdio.h>
#include <stdlib.h>

typedef struct element
{
	struct element * next;
	int nr;
} element;

element * create(int d, int par);
void deleteList(element * p);
int count(element * p);
element * add(element * p, int value);
void print(element * p);
element * deleteElement(element * p, int value);

#endif
