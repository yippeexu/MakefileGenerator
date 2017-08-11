#pragma once

#ifndef STRING_H
#define STRING_H

#include "types.h"

typedef struct String {
    char *cstr;
    u32 len;
    // u32 capacity;
} String;

b32 isNum(const char);
u32 charToNum(const char);

u32 strlen(const char *);
s32 strcmp(const char *, const char *);
// b32 strcpy(const char *, const char *);
b32 containsString(const String *, const String *);
b32 parseUInt(const String *, u32 *);

void constructString(String *, const char *);
void desrtuctString(String *);

#endif // !STRING_H
