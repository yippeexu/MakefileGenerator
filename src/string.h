/*
* MIT License
*
* Copyright (c) 2017 hockeyhurd
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

#ifndef STRING_H
#define STRING_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct String {
    char *cstr;
    u32 len;
    // u32 capacity;
} String;

b32 isNum(char);
u32 charToNum(char);

u32 stringLength(const char *);
s32 stringCompare(const char *, const char *);
// b32 strcpy(const char *, const char *);
b32 containsString(const String *, const String *);

b32 parseUInt(const String *, u32 *);
b32 toString(String *, s32);

void constructString(String *, const char *);
void desrtuctString(String *);

#ifdef __cplusplus
}
#endif

#endif // !STRING_H
