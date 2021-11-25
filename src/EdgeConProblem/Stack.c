#include "Stack.h"

struct stack {
    int *stack;
    unsigned int nb;
};

Stack createStack(int size) {
    Stack s = malloc(sizeof(Stack));
    s->stack = (int*) malloc(sizeof(int)*size);
    s->nb = 0;
    return s;
}

void freeStack(Stack s) {
    free(s->stack);
    s->stack = NULL;
    free(s);
    s = NULL;
}

bool isEmpty(Stack s) {
    return s->nb <= 0;
}

void push(Stack s, int i) {
    s->stack[s->nb] = i;
    s->nb++;
}

int pop(Stack s) {
    int i = -1;
    if (s->nb > 0)
    {
        s->nb--;
        i = s->stack[s->nb];
    }
    return i;
}

void printStack(Stack s) {
    printf("***Print stack -- size = %d***\n", s->nb);
    for (int i = 0; i < s->nb; i++)
    {
        printf("%d ", s->stack[i]);
    }
    printf("\n");
    printf("***End***\n");
}