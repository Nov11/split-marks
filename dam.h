#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BSIZE 80
#define BLANK " "
#define ELBEG "<!ELEMENT"
#define ELEND ">"
#define ATTBG "("
#define ATTED ")"
#define XMLHD "<?xml version=\"1.0\"?>\x0d\x0a"
#define DTDHD "<!DOCTYPE"
#define DTDRT "wow"
#define PCDAT 1
#define SUBND 2
#define FOUND 1
#define NOTFOUND 0
#define	STKSZ 20
#define QUESZ 20

typedef struct attnode{
	char* name;
	struct attnode* next;
}ATTNODE;

typedef struct ele{
	char* name;
	ATTNODE* attnode;
	struct ele* next;
}ELE;

typedef struct xmlnode{
	char* name;
	struct xmlnode* subnodelist;
	struct xmlnode* next; //node in the same level,or say sibling node
	char* value;		//only need to print it,so no type trans
}XMLNODE;
	
typedef int TREE;

XMLNODE*  stack[STKSZ];
int top;
XMLNODE* queue[QUESZ];
int head,tail;

void push(XMLNODE* p)
{
	if(top == STKSZ){
		printf("cannot push in\n");
		return;
	}
	stack[top] = p;
	top++;
}

void pop(XMLNODE** p)
{
	if(top == 0){
		printf("empty stack\n");
		return;
	}
	top--;
	*p = stack[top];
}

void inqu(XMLNODE* p)
{
	if((tail + 1) % QUESZ == head){	//one item to identify full or empty
		printf("queue is full\n");
		exit(1); //not going to haddle it now
	}
	queue[tail] = p;
	tail = (tail + 1) % QUESZ;
}

void dequ(XMLNODE** p)
{
	if(head == tail){
		printf("queue is empty\n");
		exit(1);
	}
	*p = queue[head];
	head = (head + 1) % QUESZ;
}

int quempty()
{
	if(head == tail){
		return 1;
	}
	return 0;
}