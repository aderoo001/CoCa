/**
 * @file EdgeConResolution.h
 * @author Dimitri Didier / Alexis Deroo
 * @brief  Stack struture and some primitives.
 * @version 1
 * @date 2021-11-26
 * 
 * @copyright Creative Commons.
 * 
 */

#include <stdbool.h>

/** @brief: the stack type. First field is the stack and the second one his size. */
typedef struct stack *Stack;

/**
 * @brief Check if @p s is empty.
 * 
 * @param s An instance of Stack.
 * 
 * @return True if stack is empty.
 */
bool isEmpty(Stack s);

/**
 * @brief Create new instance of stack.
 * 
 * @param size Size of stack;
 * 
 * @return An instance of stack.
 */
Stack createStack(int size);

/**
 * Free instance @p s.
 * 
 * @param s An instance of stack.
 */
void freeStack(Stack s);

/**
 * @brief Push @p i in stack @p s.
 * 
 * @param s An instance of Stack.
 * @param i An integer.
 */
void push(Stack s, int i);

/**
 * @brief Pop last value of heap @p s.
 * 
 * @param s An instance of Stack.
 * 
 * @return The last value of stack.
 */
int pop(Stack s);

/**
 * @brief Print Stack @p s.
 * 
 * @param s An instance of Stack.
 */
void printStack(Stack s);