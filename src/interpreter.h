#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

// #include "types.h"
// #include "string.h"
#include "maker.h"

#define INT_FLAG_CPP "-cpp"
#define INT_FLAG_STD "-std="

typedef struct IntFlags {
    char **args;
    u32 numArgs;
    b32 cmode;
    u32 stdver;
} IFlags;

b32 interpret(const char **, const u32, IFlags *);

#endif // !INTERPRETER_H
