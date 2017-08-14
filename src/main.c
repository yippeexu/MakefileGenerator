/* MIT License
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

// #include "types.h"

#include "maker.h"
#include "interpreter.h"
#include "arraylist.h"
#include "stack.h"
#include "string.h"

#if Debug
#include <vld.h>
#endif

#if Debug & 0
#define INTERNAL_TEST True
#else
#define INTERNAL_TEST False
#endif

void error(const char *msg) {
    perror("Error interpeting args from command line!\n");

#ifdef WIN32
        system("PAUSE");
#endif

    exit(-1);
}

b32 isSourceFile(const String *);

s32 main(s32 argc, char **argv) {

#if 0
    String strInt;
    strInt.cstr = "945";
    strInt.len = strlen(strInt.cstr);

    u32 output;
    const b32 result = parseUInt(&strInt, &output);

    printf("Result: %u, value: %u\n", result, output);
#endif

#if 0
    String testStr;
    testStr.cstr = "a.c";
    testStr.len = 4;
    
    printf("Is source file %u\n", isSourceFile(&testStr));
#ifdef WIN32
    system("PAUSE");
#endif
    return 0;
#endif

    IFlags flags;
    flags.args = NULL;
    flags.numArgs = 0;
    flags.cmode = True;

    SRC sources;

    if (!interpret(argv, (u32) argc, &flags, &sources)) {
        error("Error interpeting args from command line!\n");
    }

    constructString(&sources.flags, DEFAULT_FLAGS);
    writeToFile(MAKEFILE_VAR, &sources);

    desrtuctString(&sources.flags);
    free(sources.srcFiles);

#if INTERNAL_TEST
    SRC src;
    src.len = 2;
    src.srcFiles = (String *) calloc(src.len, sizeof(String));
    constructString(&src.flags, DEFAULT_FLAGS);
    constructString(&src.srcFiles[0], "main.c");
    constructString(&src.srcFiles[1], "example.c");

    writeToFile(MAKEFILE_VAR, &src);

    desrtuctString(&src.flags);
    desrtuctString(&src.srcFiles[0]);
    desrtuctString(&src.srcFiles[1]);
    free(src.srcFiles);
#endif

#ifdef WIN32
    system("PAUSE");
#endif
    return 0;
}
