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

#include "types.h"

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 myPow(const u32 base, const u32 power) {
    if (!power)
        return 1;
    else if (power == 1)
        return base;

    u32 result = base;

    for (u32 i = 0; i < power - 1; i++) {
        result *= base;
    }

    return result;
}

void *myMalloc(const u32 size, const char *tag) {
    assert(size > 0);

    void *ptr = malloc(size);

    if (ptr == NULL) {
        fprintf(stderr, "Error malloc'ing ptr: %p (size: %u) with tag: %s!\n", ptr, size, tag);
        exit(-1);
    }

#if Debug
    assert(tag != NULL);
    printf("Malloc'ing ptr: %p (size: %u) with tag: %s\n", ptr, size, tag);
#endif

    return ptr;
}

void myFree(void *ptr, const char *tag) {
    assert(ptr != NULL);

#if Debug
    printf("Freeing ptr: %p with tag: %s\n", ptr, tag);
#endif
    free(ptr);
}

void copyBytes(char *src, char *dest, const u32 srcOffset, const u32 destOffset, const u32 numBytes) {
	const u32 srcOffsetPos = srcOffset * numBytes;
	const u32 destOffsetPos = destOffset * numBytes;

	for (u32 i = 0; i < numBytes; i++) {
		dest[i + destOffsetPos] = src[i + srcOffsetPos];
	}
}

#ifdef __cplusplus
}
#endif
