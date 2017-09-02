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

static void writeSourceFile(const FILE *, const char *);
static void writeString(const String *, const FILE *);
static void writeCString(const char *, const FILE *);
static void writeFileName(const char *, const FILE *);

b32 writeToFile(const char *file, const SRC *src) {
    if (file == NULL || src == NULL || src->len < 2)
        return False;

    FILE *outputFile = fopen(file, "w");

    if (outputFile == NULL)
        return False;

    writeCString("CC = gcc", outputFile);
    putc('\n', outputFile);

    // Write "CC_VER = -std=c99":
    // writeCString("CC_VER = -std=c99", outputFile);
    writeCString("CC_VER = -std=c", outputFile);

    if (!src->cmode) 
        writeCString("++", outputFile);

    putc('0' + ((src->stdver / 10) % 10), outputFile);
    putc('0' + (src->stdver % 10), outputFile);
    putc('\n', outputFile);

    writeCString("CC_FLAGS = ", outputFile);
    writeString(&src->flags, outputFile);
    putc('\n', outputFile);
    putc('\n', outputFile);

    // all: main
    writeCString("all: ", outputFile);

    for (u32 i = 1; i < src->len; i++) {
        writeFileName(src->srcFiles[i].cstr, outputFile);
        writeCString(".o ", outputFile);
    }

    writeFileName(src->srcFiles[0].cstr, outputFile);

    putc('\n', outputFile);
    putc('\n', outputFile);

    // Write .o's
    for (u32 i = 1; i < src->len; i++) {
        writeSourceFile(outputFile, src->srcFiles[i].cstr);
        putc('\n', outputFile);
    }

    putc('\n', outputFile);
    writeFileName(src->srcFiles[0].cstr, outputFile);

    writeCString(": ", outputFile);
    writeCString(src->srcFiles[0].cstr, outputFile);
    putc('\n', outputFile);

    // $(CC)
    putc('\t', outputFile);
    writeCString(CC_VAR, outputFile);
    putc(' ', outputFile);

    // -o
    writeCString("-o ", outputFile);

    if (src->name.cstr != NULL && src->name.len) {
        writeString(&src->name, outputFile);
    }
    
    else {
        writeFileName(src->srcFiles[0].cstr, outputFile);
    }

#if 0
        // write name of executable:
        if (src->name.cstr != NULL && src->name.len) {
            // "-o"
            writeCString("-o ", outputFile);
            writeString(&src->name, outputFile);
        }
#endif

    putc(' ', outputFile);

    // EX: -g -Wall -std=c99
    // writeString(src->flags, outputFile);
    writeCString(CC_FLAGS_VAR, outputFile);
    putc(' ', outputFile);

    // main.c
    writeCString(src->srcFiles[0].cstr, outputFile);
    putc(' ', outputFile);

    // example.o
    for (u32 i = 1; i < src->len; i++) {
        for (char *c = (char *) src->srcFiles[i].cstr; *c != '\0' && *c != '.'; c++) {
            putc(*c, outputFile);
        }

        writeCString(".o ", outputFile);
        // putc(' ', outputFile);
    }

    putc('\n', outputFile);
    putc('\n', outputFile);

    // clean
    writeCString("clean:\n\trm *.o ", outputFile);
    writeFileName(src->srcFiles[0].cstr, outputFile);

    fclose(outputFile);

    return True;
}

void writeSourceFile(const FILE *file, const char *src) {
    if (file == NULL || src == NULL)
        return;
    
    // example.o: \' \'
    for (u32 i = 0; src[i] != '\0'; i++) {
        if (src[i] == '.')
            break;

        putc(src[i], (FILE *) file);
    }

    writeCString(".o: ", file);
    writeCString(src, file);
    putc('\n', (FILE *) file);

    // $(CC)
    putc('\t', (FILE *) file);
    writeCString(CC_VAR, file);
    putc(' ', (FILE *) file);

    // EX: -g -Wall -std=c99 + -c
    // writeString(flags, file);
    writeCString(CC_FLAGS_VAR, file);
    putc(' ', (FILE *) file);
    writeCString("-c", file);
    putc(' ', (FILE *) file);

    // example.c
    writeCString(src, file);
}

void writeString(const String *src, const FILE *file) {
    if (src != NULL && file != NULL) {
        for (u32 i = 0; i < src->len; i++)
            putc(src->cstr[i], (FILE *) file);
    }
}

void writeCString(const char *src, const FILE *file) {
    if (src != NULL && file != NULL) {
        for (u32 i = 0; src[i] != '\0'; i++)
            putc(src[i], (FILE *) file);
    }
}

void writeFileName(const char *src, const FILE *file) {
    if (src != NULL) {
        for (char *c = (char *) src; *c != '\0'; c++) {
            if (*c == '.')
                break;

            putc(*c, (FILE *) file);
        }
    }
}
