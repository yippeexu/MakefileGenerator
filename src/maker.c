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

#include "maker.h"

#include <stdio.h>

typedef ArrayListIterator Iter;

void myFree(void *, const char *);

static void writeString(const String *, const FILE *);
static void writeCString(const char *, const FILE *);
static void writeChar(const char, const FILE *);
static void writeActualFileName(const SourceFile *, const FILE *);

static void writeMakefileHeaderVars(SRC *, const IFlags *, const FILE *);
static b32 writeMakefileOptimizationLevel(SRC *, const OptLevel, const b32, const FILE *);
static void writeMakefileModules(const SRC *, const IFlags *, const FILE *);

// static void compileFlagsToString(String *, const IFlags *);
static b32 isValidSTDVersion(const b32, const u32);

void writeString(const String *word, const FILE *file) {
    for (u32 i = 0; i < word->len - 1; i++)
        putc(word->cstr[i], (FILE *) file);
}

void writeCString(const char *word, const FILE *file) {
    for (char *c = (char *) word; *c != '\0'; c++)
        putc(*c, (FILE *) file);
}

void writeChar(const char c, const FILE *file) {
    putc((char) c, (FILE *) file);
}

void writeActualFileName(const SourceFile *file, const FILE *makefile) {
    char *c = NULL;

    for (c = file->fileName.cstr; *c != '.'; c++) {
        writeChar(*c, makefile);
    }

    // if (*c == '.')
        // writeChar(++c, makefile);
}

void writeMakefileHeaderVars(SRC *source, const IFlags *flags, const FILE *makefile) {
    constructString(&source->flags, "");

    if (flags->cmode == 1)
        writeCString("CC = gcc\n", makefile);
    else
        writeCString("CC = g++\n", makefile);

    writeCString("CC_FLAGS = ", makefile);

    b32 needsSpace = False;
    
    if (flags->flags.len) {
        // Below is removed as it should be un-neccessary.
#if 0
        if (needsSpace)
            writeChar(' ', makefile);

        else
            needsSpace = False;
#endif

        for (u32 i = 0; i < flags->flags.len; i++) {
            String *string = getArrayList(&flags->flags, i);
            writeString(string, makefile);
            writeChar(' ', makefile);
            // myFree(string, "ArrayList Flag String");
        }
    }

    needsSpace = writeMakefileOptimizationLevel(source, flags->optLevel, needsSpace, makefile);

    if (flags->wall != INTERPRETER_INVALID_FLAG) {
        if (needsSpace) {
            writeCString(" -Wall", makefile);
            appendCString(&source->flags, " -Wall");
        }

        else {
            needsSpace = True;
            writeCString("-Wall", makefile);
            appendCString(&source->flags, "-Wall");
        }
    }

    if (flags->stdver != INTERPRETER_INVALID_FLAG) {
        String stdver;

        if (toString(&stdver, (s32) flags->stdver)) {

            if (!isValidSTDVersion(flags->cmode, flags->stdver)) {
                perror("Invalid stdver! Please check your input!\n");
                
                if (flags->cmode)
                    ((IFlags *) flags)->stdver = DEFAULT_C_STD;
                else
                    ((IFlags *)flags)->stdver = DEFAULT_CPP_STD;

                toString(&stdver, (s32) flags->stdver);
            }

            if (needsSpace) {
                writeCString(" -std=", makefile);
                appendCString(&source->flags, " -std=");
            }

            else {
                needsSpace = True;
                writeCString("-std=", makefile);
                appendCString(&source->flags, "-std=");
            }

            if (flags->cmode) {
                writeCString("c", makefile);
                appendCString(&source->flags, "c");
            }

            else {
                writeCString("c++", makefile);
                appendCString(&source->flags, "c++");
            }

            writeString(&stdver, makefile);
            appendCString(&source->flags, stdver.cstr);

            desrtuctString(&stdver);
        }
    }
}

b32 writeMakefileOptimizationLevel(SRC *source, const OptLevel level, const b32 needsSpace, const FILE *makefile) {
    if (needsSpace)
        writeChar(' ', makefile);
    
    switch (level) {
    case OPT_DEBUG:
        writeCString("-g", makefile);
        appendCString(&source->flags, "-g");
        break;

    case OPT_OFF:
        writeCString("-O0", makefile);
        appendCString(&source->flags, "-O0");
        break;

    case OPT_LOW:
        writeCString("-O1", makefile);
        appendCString(&source->flags, "-O1");
        break;

    case OPT_MED:
        writeCString("-O2", makefile);
        appendCString(&source->flags, "-O2");
        break;

    case OPT_HIGH:
        writeCString("-O3", makefile);
        appendCString(&source->flags, "-O3");
        break;

    default:
        return False;
    }

    return True;
}

void writeMakefileModules(const SRC *source, const IFlags *flags, const FILE *makefile) {
    writeCString("all: link ", makefile);

    Iter iter;
    constructArrayListIterator(&iter, source->sourceFiles);

    while (hasNextArrayListIterator(&iter)) {
        SourceFile *file = nextArrayListIterator(&iter);
        
        if (file->fileType == SOURCE) {
            writeActualFileName(file, makefile);
            writeCString(".o ", makefile);
        }
    }

    writeChar('\n', makefile);
    writeChar('\n', makefile);

    constructArrayListIterator(&iter, source->sourceFiles);

    while (hasNextArrayListIterator(&iter)) {
        SourceFile *file = nextArrayListIterator(&iter);

        if (file->fileType == SOURCE) {
            writeActualFileName(file, makefile);
            writeCString(".o: ", makefile);
            writeString(&file->fileName, makefile);
            writeChar('\n', makefile);

            writeChar('\t', makefile);
            writeCString(CC_VAR, makefile);
            writeChar(' ', makefile);
            // writeString(&source->flags, makefile);
            writeCString(CC_FLAGS_VAR, makefile);
            writeCString(" -c ", makefile);
            writeString(&file->fileName, makefile);
            writeChar('\n', makefile);
        }
    }

    writeCString("link: ", makefile);
    constructArrayListIterator(&iter, source->sourceFiles);

    while (hasNextArrayListIterator(&iter)) {
        SourceFile *file = nextArrayListIterator(&iter);

        if (file->fileType == SOURCE) {
            writeActualFileName(file, makefile);
            writeCString(".o ", makefile);
        }
    }

    writeCString("\n\t", makefile);
    writeCString(CC_VAR, makefile);
    writeChar(' ', makefile);
    writeCString(CC_FLAGS_VAR, makefile);
    writeChar(' ', makefile);

    if (flags->outputName.cstr != NULL && flags->outputName.len) {
        writeCString("-o ", makefile);
        writeString(&flags->outputName, makefile);
        writeChar(' ', makefile);
    }

    constructArrayListIterator(&iter, source->sourceFiles);

    while (hasNextArrayListIterator(&iter)) {
        SourceFile *file = nextArrayListIterator(&iter);

        if (file->fileType == SOURCE) {
            writeActualFileName(file, makefile);
            writeCString(".o ", makefile);
        }
    }

    writeCString("\nclean:\n\trm -f *.o", makefile);

    if (flags->outputName.cstr != NULL && flags->outputName.len) {
        writeChar(' ', makefile);
        writeString(&flags->outputName, makefile);
    }
}

b32 isValidSTDVersion(const b32 cmode, const u32 version) {
    if (cmode == 1) {
        switch (version) {
        case 90:
            return True;
        case 99:
            return True;
        case 11:
            return True;
        // case 17: // Waiting C17/C18 standard...
            // break;
        default:
            return False;
        }
    }

    else {
        switch (version) {
        case 98:
            return True;
        case 11:
            return True;
        case 14:
            return True;
        case 17:
            return True;
        default:
            return False;
        }
    }

    return False;
}

void constructSources(SRC *src, const char *fileName, const ArrayList *sourceFiles) {
    constructString(&src->fileName, fileName);
    src->flags.cstr = NULL;
    src->flags.len = 0;
    // src->stdver = 0x80000000;
    // src->cmode = 0x80000000;
    src->sourceFiles = (ArrayList *) sourceFiles;
}

void destructSources(SRC *src) {
    if (src->fileName.cstr != NULL && src->fileName.len)
        desrtuctString(&src->fileName);
    if (src->flags.cstr != NULL && src->flags.len)
        desrtuctString(&src->flags);

    // src->stdver = 0x80000000;
    // src->cmode = 0x80000000;
    src->sourceFiles = NULL;
}

void addSourceFile(const SRC *src, const SourceFile *sourceFile) {
    addArrayList(src->sourceFiles, sourceFile);
}

b32 writeToFile(const SRC *source, const IFlags *flags) {
    if (flags->cmode == INTERPRETER_INVALID_FLAG) {
        perror("Could not determine \"C-Mode\"!  Assuming mode 'C++'\n");
        ((IFlags *) flags)->cmode = 0;
    }

    FILE *outputFile = fopen(source->fileName.cstr, "w");

    if (outputFile == NULL) {
        fclose(outputFile);
        perror("Error writing file: ");
        perror(source->fileName.cstr);
        perror("\n");
        // exit(-1);
        return False;
    }

    // compileFlagsToString(source->flags, flags);

    // writeCString("Hello makefile!\nThis is a test!", outputFile);
    writeMakefileHeaderVars((SRC *) source, flags, outputFile);
    writeChar('\n', outputFile);
    writeMakefileModules(source, flags, outputFile);

    fclose(outputFile);

    return True;
}