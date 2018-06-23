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
#include "filesystem.h"
#include "filter.h"

#ifdef WIN32
#include <vld.h>
#endif

static void pause(void) {
    // system("PAUSE");
#if OS_WIN
    printf("Press enter to continue.\n");
    getchar();
#endif
}

#define TEST 0

typedef ArrayListIterator Iter;

#if 0
static void printString(const String *);

void printString(const String *string) {
    printf("[%u]: %s\n", string->len, string->cstr);
}
#endif

static s32 intCompare(void *left, void *right) {
	s32 leftInt = *(s32 *) &left;
	s32 rightInt = *(s32 *) &right;

	return leftInt - rightInt;
}

s32 main(s32 argc, char **argv) {
#if 1

	FilterList filter;
	filter.whiteListMode = True;
	
	constructArrayList(&filter.descriptor, 0x10, sizeof(u32));

	for (u32 i = 0; i <= 10; i += 2) {
		addArrayList(&filter.descriptor, (const void *) i);
	}

	u32 input[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const u32 len = sizeof(input) / sizeof(input[0]);
	u32 output[sizeof(input) / sizeof(input[0])] = { 0 };

	const u32 numElementsFound = filterArray((void **) input, (void **) output, (const pint) len, (const u32) sizeof(input[0]), &filter, intCompare);

	puts("Output:\n");
	for (u32 i = 0; i < numElementsFound; i++) {
		printf("[%u]: %u\n", i, output[i]);
	}

	destructArrayList(&filter.descriptor);

#elif TEST
    // for (s32 i = 1; i < argc; i++)
        // printf("[%d]: %s\n", i, argv[i]);

    // Test string method.

    File file;

    constructString(&file.path, "../MakefileGenerator/src/main.c");

    file.op = OP_READ;
    file.isDir = False;
    file.file = NULL;

    // printf("Exists: %u\n", checkIfFileExists("../MakefileGenerator/src/main.c"));

    if (openFile(&file)) {
        goto END;
    }

    u32 buf;
    u32 count = 0;
    while ((buf = readByteFromFile(&file)) != EOF) {
        printf("%c", (char) buf & 0xFF);

        if (++count == 50) {
            count = 0;
            putchar('\n');
        }
    }

    closeFile(&file);

    desrtuctString(&file.path);

    END:;
#elif 0

    String this;
    constructString(&this, "Hello World!");

    String comp;
    constructString(&comp, "Hello");

    printString(&this);
    printString(&comp);

    const b32 result = stringStartsWith(&this, &comp);
    printf("Result %u\n", result);

    desrtuctString(&this);
    desrtuctString(&comp);

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
