#pragma once

#ifndef STACK_H
#define STACK_H

#include "arraylist.h"

#if 0
typedef struct Stack {
    ArrayList list;
} Stack;
#else
typedef ArrayList Stack;
#endif

void initStack(Stack *, const u32);
void freeStack(Stack *);

void pushStack(Stack *, void *);
void *peekStack(const Stack *);
void popStack(Stack *, void **);

#endif // !STACK_H
