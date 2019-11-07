/*
 * File: stack.c
 * -------------
 * Linked-list implementation of a stack.
 *
 * COS315 Assignment 3
 * 31/05/03
 *
 */
#include<stdio.h>

/*
 * Type: stackElementT
 * -------------------
 * This is the type of the objects entered in the stack.
 * Edit it to change the type of things to be placed in
 * the stack.
 */
 
typedef struct _nodeT {
	valueT value;
	struct _nodeT *next;
} nodeT;

typedef struct {
	nodeT *head;
} stackT;

typedef char valueT;

static nodeT *NewNode(void);

/*
 * Function: NewNode
 * Usage: nodeT *node = NewNode();
 * -------------------------------
 * This function returns a pointer to a dynamically allocated
 * node.
 *
 */
static nodeT* NewNode(void) {
  nodeT *new = (nodeT *)malloc(sizeof(nodeT));

  if(new==NULL) {
    printf("Error in NewNode:  out of memory!\n");
    return NULL;
  }
  return new;
}

/* 
 * Function: NewStack
 * Usage: stackT *stack = NewStack();
 * ----------------------------------
 * This function returns a pointer to a newly allocated stack and
 * sets the head to NULL.
 *
 */
stackT *NewStack(void) {
  stackT *newStack = (stackT *)malloc(sizeof(stackT));

  if(newStack==NULL) {
    printf("Error in NewStack:  out of memory!\n");
    return NULL;
  }

  newStack->head=NULL;
  return newStack;
}

/*
 * Function: FreeStack
 * Usage: FreeStack(stack);
 * ------------------------
 * This function frees up space associated with
 * an EMPTY stack.
 *
 */
void FreeStack(stackT *stack) {
  if(!IsEmpty(stack)) 
    printf("Cannot free a non-empty stack!\n");
  else
    free(stack);
}

/*
 * Function: EmptyStack
 * Usage: EmptyStack(stack);
 * -------------------------
 * This function frees up spaces associated with the elements
 * in the stack, but does not delete the stack.
 *
 */
void EmptyStack(stackT *stack) {
  nodeT *old;

  while(!IsEmpty(stack)) {
    old=stack->head->next;
    free(stack->head);
    stack->head=old;
  }
}

/*
 * Function: IsEmpty
 * Usage: if(IsEmpty(stack)) FreeStack(stack);
 * -------------------------------------------
 * Determine whether or not a stack is empty.
 * 
 */
bool IsEmpty(stackT *stack) {
  if(stack->head==NULL)
    return true;
  return false;
}

/*
 * Function: Push
 * Usage: Push(stack,value);
 * -------------------------
 * Pushes a value onto the stack.
 *
 */
void Push(stackT *stack, valueT value) {
  nodeT *new = NewNode();

  new->value=value;
  if(stack->head==NULL) {
    stack->head=new;
    new->next=NULL;
  } else {
    new->next=stack->head;
    stack->head=new;
  }
}

/*
 * Function: Pop
 * Usage: value = Pop(stack);
 * --------------------------
 * This function pops a value from the stack.
 * 
 */
valueT Pop(stackT *stack) {
  valueT value;
  nodeT *old;
  if(stack->head==NULL) {
    printf("Stack is empty!\n");
    return 0;
  } else {
    value = stack->head->value;
    old = stack->head->next;
    free(stack->head);
    stack->head=old;
    return value;
  }
}
