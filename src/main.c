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

#include "interpreter.h"
#include "maker.h"

#ifdef WIN32
#include <vld.h>
#endif

#if OS_WIN
static void pause(void) {
    system("PAUSE");
}
#else
static void pause(void) {}
#endif

#define TEST 0

typedef ArrayListIterator Iter;

static void printString(const String *);
void myFree(void *, const char *);

void printString(const String *string) {
    printf("[%u]: %s\n", string->len, string->cstr);
}

void myFree(void *ptr, const char *tag) {
#if Debug
    printf("Freeing ptr: %p with tag: %s\n", ptr, tag);
#endif
    free(ptr);
}

s32 main(s32 argc, char **argv) {
#if TEST
    String test;
    constructString(&test, "Hello");
    printString(&test);

    appendCString(&test, " World!");
    printString(&test);

    desrtuctString(&test);

#else
    IFlags flags;
    initIFlags(&flags);

    // ArrayList *sourceFiles = interpretArgs(argc, argv, &flags);
    ArrayList sourceFiles;

    if (!interpretArgs(argc, argv, &sourceFiles, &flags)) {
        printf("Error collecting source flags...\n");
    }

    else {
        printf("Collecting source files success!!\n");

        u32 fileNum = 0;
        Iter iter;

#if 1
        constructArrayListIterator(&iter, &sourceFiles);
        printf("Files:\n");
        while (hasNextArrayListIterator(&iter)) {
            SourceFile *sourceFile = (SourceFile *) nextArrayListIterator(&iter);
            printf("\t[%u]: %s\n", ++fileNum, sourceFile->fileName.cstr);
            // free(sourceFile->fileName.cstr);
        }
#endif

        SRC source;
        constructSources(&source, "makefile", &sourceFiles);

        writeToFile(&source, &flags);

        constructArrayListIterator(&iter, &sourceFiles);
        while (hasNextArrayListIterator(&iter)) {
            SourceFile *sourceFile = (SourceFile *) nextArrayListIterator(&iter);
            // printf("\t[%u]: %s\n", ++fileNum, sourceFile->fileName.cstr);
            // free(sourceFile->fileName.cstr);
            myFree(sourceFile->fileName.cstr, "SourceFile's String");
        }

        destructArrayList(&sourceFiles);
        destructSources(&source);
    }

    freeIFlags(&flags);
#endif
    pause();
    return 0;
}
