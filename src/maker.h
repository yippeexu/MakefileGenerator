#pragma once

#ifndef MAKER_H
#define MAKER_H

// #include "types.h"
#include "string.h"

#define CC_VAR "$(CC)"
#define CC_FLAGS_VAR "$(CC_FLAGS)"
#define CPP_VAR "$(CPP)"
#define MAKEFILE_VAR "makefile"
#define DEFAULT_FLAGS "$(CC_VER) -g -Wall"

typedef struct SRC {
    // char *flags;
    String flags;
    // char **srcFiles;
    String *srcFiles;
    u32 len;
} SRC;

b32 writeToFile(const char *, const SRC *);

#endif // !MAKER_H
