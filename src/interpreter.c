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

#ifndef DEFAULT_C_STD
#define DEFAULT_C_STD 99u
#endif // !DEFAULT_C_STD


extern void myFree(void *, const char *);
extern void *myMalloc(const u32, const char *);

static const char *flagVerCheck = "-std=c";
static const char *flagNameCheck = "-name=";
static const u32 flagNameCheckLen = 6;
static const u32 flagVerCheckLen = 6;
static SourceFile fileBuf[0x100];
static u32 bufIndex = 0;

static void cleanupAllocs(ArrayList *);
static b32 decodeIFlag(const String *, IFlags *);

static void loadDefaultEnvironment(ArrayList *, IFlags *);

void cleanupAllocs(ArrayList *list) {
    destructArrayList(list);
    // free(*list);
    myFree(list, "Allocs");
    // *list = NULL;
}

b32 decodeIFlag(const String *arg, IFlags *flags) {
    String temp;
    temp.cstr = "-l";
    temp.len = stringLength(temp.cstr);

    if (!stringCompare(arg->cstr, "-Wall"))
        flags->wall = 1;
    else if (!stringCompare(arg->cstr, "-g"))
        flags->optLevel = OPT_DEBUG;
    else if (!stringCompare(arg->cstr, "-O0"))
        flags->optLevel = OPT_OFF;
    else if (!stringCompare(arg->cstr, "-O1"))
        flags->optLevel = OPT_LOW;
    else if (!stringCompare(arg->cstr, "-O2"))
        flags->optLevel = OPT_MED;
    else if (!stringCompare(arg->cstr, "-O3"))
        flags->optLevel = OPT_HIGH;
    else if (!stringCompare(arg->cstr, "-pipe")) {
        String *pipe = (String *) myMalloc(sizeof(String), "Malloc -pipe flag");
        constructString(pipe, "-pipe");

        addArrayList(&flags->flags, pipe);
    }

    else if (!stringCompare(arg->cstr, "-pthread")) {
        String *pThread = (String *) myMalloc(sizeof(String), "Malloc -pthread flag");
        constructString(pThread, "-pthread");

        addArrayList(&flags->flags, pThread);
    }

    // Below needs to be verified as incorrect! (Temp deprecated!).
#if 0
    else if (!stringCompare(arg->cstr, "-gtest")) {
        String *gtest = (String *) myMalloc(sizeof(String), "Malloc -gtest flag");
        constructString(gtest, "-gtest");

        addArrayList(&flags->flags, gtest);
    }
#endif

    else if (stringStartsWith(arg, &temp)) {
        String *link = (String *) myMalloc(sizeof(String), "Malloc -l flag");
        constructString(link, arg->cstr);

        addArrayList(&flags->flags, link);
    }

    // Could be "-name=<insert name here>"
    else if (arg->len >= flagNameCheckLen) {
        for (u32 i = 0; i < flagNameCheckLen; i++) {
            if (arg->cstr[i] != flagNameCheck[i]) {
                goto CHECK;
                return False;
            }
        }

        // Valid name, set appropriate flags.
        constructString(&flags->outputName, arg->cstr + 6);
    }

    // -std=c11, -std=c++11
    // Check for stdver:
    else if (arg->len == 9 || arg->len == 11) {
    CHECK:;
        u32 i;
        for (i = 0; i < flagVerCheckLen; i++) {
            if (arg->cstr[i] != flagVerCheck[i])
                return False;
        }

        if (arg->len == 11) {
            if (arg->cstr[i++] != '+' || arg->cstr[i++] != '+')
                return False;
        }

        // flags->stdver
        String temp;
        temp.cstr = (char *) &arg->cstr[i];
        temp.len = stringLength(temp.cstr);

        u32 output = 0;

        parseUInt(&temp, &output);
        flags->stdver = (flag_t) (output & 0xff);
    }

    else
        return False;

    return True;
}

void loadDefaultEnvironment(ArrayList *sourceFiles, IFlags *flags) {
    /*
    *   -pipe -pthread -gtest -g -Wall -std=c11 -name=makegen main.c arraylist.c 
    *   interpreter.c maker.c source.c string.c types.c
    */

    SourceFile *mainFile = &fileBuf[bufIndex++];
    SourceFile *exFile = &fileBuf[bufIndex++];

    constructString(&mainFile->fileName, "main.c");
    constructString(&exFile->fileName, "ex.c");

    mainFile->fileType = SOURCE;
    exFile->fileType = SOURCE;

    addArrayList(sourceFiles, mainFile);
    addArrayList(sourceFiles, exFile);

    flags->optLevel = OPT_DEBUG;
    flags->wall = 1;
    flags->stdver = DEFAULT_C_STD;
    flags->cmode = True;
    constructString(&flags->outputName, "main");

    String *pipe = (String *) myMalloc(sizeof(String), "Malloc -pipe flag");
    constructString(pipe, "-pipe");

    addArrayList(&flags->flags, pipe);
}

void initIFlags(IFlags *flags) {
    flags->optLevel = OPT_INVALID;
    flags->wall = INTERPRETER_INVALID_FLAG;
    flags->stdver = INTERPRETER_INVALID_FLAG;
    flags->cmode = INTERPRETER_INVALID_FLAG;
    flags->outputName.cstr = NULL;
    flags->outputName.len = 0;
    
    constructArrayList(&flags->flags, 0x10, sizeof(String *));
}

void freeIFlags(IFlags *flags) {
    flags->optLevel = OPT_INVALID;
    flags->wall = INTERPRETER_INVALID_FLAG;
    flags->stdver = INTERPRETER_INVALID_FLAG;
    flags->cmode = INTERPRETER_INVALID_FLAG;

    if (flags->outputName.cstr != NULL && flags->outputName.len)
        desrtuctString(&flags->outputName);

    if (flags->flags.len) {
        for (s64 i = flags->flags.len - 1; i >= 0; i--) {
            String *string = (String *) getArrayList(&flags->flags, (const u32) i);
            desrtuctString(string);
            myFree(string, "Free flag from ArrayList");
            removeLastArrayList(&flags->flags);
        }
    }

    destructArrayList(&flags->flags);
}

u32 interpretArgs(const u32 argc, char **argv, ArrayList *sourceFiles, IFlags *flags) {
    if (argc <= 1 || argv == NULL)
        return 0;

    /*ArrayList *sourceFiles = (ArrayList *) malloc(sizeof(ArrayList));

    if (sourceFiles == NULL) {
        free(sourceFiles);
        perror("Error allocating space for an ArrayList!\n");
        exit(-1);
        return NULL;
    }*/

    constructArrayList(sourceFiles, 0x10, sizeof(SourceFile));

    if (!stringCompare(argv[1], "--example")) {
        loadDefaultEnvironment(sourceFiles, flags);
        return sourceFiles->len;
    }

    for (u32 i = 1; i < argc; i++) {
        String arg;
        arg.cstr = argv[i];
        arg.len = stringLength(arg.cstr);

        // Is argument.
        if (arg.cstr[0] == '-') {
            if (!decodeIFlag(&arg, flags)) {
                perror("Error parsing flag: ");
                perror(arg.cstr);
                perror("\n");
                cleanupAllocs(sourceFiles);
                exit(-1);
            }
        }

        // Else check if a valid source file.
        else {
            SourceFile *file = &fileBuf[bufIndex++];
            file->fileName = arg;
            b32 cmode = True;

            if (isValidSourceFile(file, &cmode)) {
                // If valid we need to deep copy file name.
                // copyString(&file->fileName, &arg);
                constructString(&file->fileName, arg.cstr);

                // Add to source file list.
                addArrayList(sourceFiles, file);

                if (!cmode)
                    flags->cmode = 0;
                else if (flags->cmode == INTERPRETER_INVALID_FLAG)
                    flags->cmode = cmode;
            }
        }
    }

    return sourceFiles->len;
}
