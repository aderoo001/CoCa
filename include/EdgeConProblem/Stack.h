#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct stack *Stack;

int getSize(Stack s);

bool isEmpty(Stack s);

Stack createStack(int size);

void freeStack(Stack s);

void push(Stack s, int i);

int pop(Stack s);

void printStack(Stack s);