/**
 * @author hockeyhurd
 * @version 5/8/18
 */

#pragma once

#ifndef MAKEFILEGENERATOR_FILESYSTEM_H
#define MAKEFILEGENERATOR_FILESYSTEM_H

#include "string.h"

#include <stdio.h>

typedef enum EnumFileOp {
    OP_INVALID = 0, OP_READ = 1, OP_WRITE = 2, OP_OVERWRITE = 3
} EFileOp;

typedef struct File {
    String path;
    EFileOp op;
    b32 isDir;
    FILE *file;
} File;

char *getCurrentWorkingDir(void);

b32 checkIfFileExists(const char *);

u32 openFile(File *);

void closeFile(File *);

u32 readByteFromFile(const File *);
u32 readIntFromFile(const File *, const b32);

void writeByteToFile(const u32, const File *);
void writeIntToFile(const u32, const File *, const b32);

#endif //MAKEFILEGENERATOR_FILESYSTEM_H
