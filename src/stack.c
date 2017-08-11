#include "stack.h"

void initStack(Stack *stack, const u32 sizeInBytes) {
    initArrayList(stack, 0x10, sizeInBytes);
}

void freeStack(Stack *stack) {
    freeArrayList(stack);
}

void pushStack(Stack *stack, void *data) {
    addArrayList(stack, data);
}

void *peekStack(const Stack *stack) {
    return stack != NULL && stack->len ? stack->data[stack->len - 1] : NULL;
}

void popStack(Stack *stack, void **output) {
    if (stack != NULL && stack->len) {
        stack->len--;

        if (output != NULL)
            *output = stack->data[stack->len];
    }
}