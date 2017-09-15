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

#ifndef MAKER_H
#define MAKER_H

#include "string.h"

#define CC_VAR "$(CC)"
#define CC_FLAGS_VAR "$(CC_FLAGS)"
#define CPP_VAR "$(CPP)"
#define MAKEFILE_VAR "makefile"
#define DEFAULT_FLAGS "$(CC_VER) -g -Wall"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SRC {
    String flags;
    String *srcFiles;
    u32 len;
    String name;
    u32 stdver;
    b32 cmode;
} SRC;

b32 writeToFile(const char *, const SRC *);

#ifdef __cplusplus
}
#endif

#endif // !MAKER_H
