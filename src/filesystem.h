/**
 * @author hockeyhurd
 * @version 5/8/18
 */

#pragma once

#ifndef MAKEFILEGENERATOR_FILESYSTEM_H
#define MAKEFILEGENERATOR_FILESYSTEM_H

#include "string.h"
#include "arraylist.h"

#include <stdio.h>

struct DIR;

typedef enum EnumFileOp {
    EFILE_OP_INVALID = 0, EFILE_OP_READ = 1, EFILE_OP_WRITE = 2, EFILE_OP_OVERWRITE = 3
} EFileOp;

typedef enum EnumDirOp {
	EDIR_OP_INVALID = 0, EDIR_OP_VALID = 1
} EDirOp;

typedef struct File {
    String path;
    EFileOp op;
    b32 isDir;
    FILE *file;
} File;

typedef struct Directory {
	String path;
	EDirOp op;
	struct DIR *dir;
} Dir;

char *getCurrentWorkingDir(void);

b32 checkIfFileExists(const char *);

u32 openFile(File *);
void closeFile(File *);

u32 readByteFromFile(const File *);
u32 readIntFromFile(const File *, const b32);

void writeByteToFile(const u32, const File *);
void writeIntToFile(const u32, const File *, const b32);

b32 openDir(Dir *);
void closeDir(Dir *);

u32 getFilesAndSubdirectories(const Dir *, const ArrayList *);
void cleanupDirectoryListing(const ArrayList *);

#endif //MAKEFILEGENERATOR_FILESYSTEM_H
