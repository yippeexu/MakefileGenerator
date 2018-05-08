/**
 * @author hockeyhurd
 * @version 5/8/18
 */

#include "filesystem.h"

#include <sys/stat.h>
#include <unistd.h>

static char *toFileMode(const EFileOp op) {
    switch (op) {
        case OP_READ:
            return "r";
        case OP_WRITE:
            return "w";
        case OP_OVERWRITE:
            return "w+";
        default:
#if Debug
            perror("Error unknown mode!\n");
            exit(-1);
#endif
            return NULL;
    }
}

static char workingDirBuf[1024];

char *getCurrentWorkingDir(void) {
    if (getcwd(workingDirBuf, sizeof(workingDirBuf)) != NULL)
        return workingDirBuf;

    return NULL;
}

b32 checkIfFileExists(const char *path) {
    struct stat s;
    return path != NULL && stringLength(path) && stat(path, &s) == 0 ? True : False;
}

u32 openFile(File *file) {
    char *mode = toFileMode(file->op);

    if (mode == NULL)
        return 1;

    file->file = fopen(file->path.cstr, mode);

    if (file->file == NULL) {
        perror("Error openning file!\n");
        return 1;
    }

    struct stat s;

    if (stat(file->path.cstr,&s) == 0) {
        file->isDir = s.st_mode & S_IFDIR ? True : False;
    }

    else {
        perror("Error determining whether is a directory!\n");
        return 1;
    }

    return 0;
}

void closeFile(File *file) {
    fclose(file->file);
    file->file = NULL;
}

u32 readByteFromFile(const File *file) {
    return (u32) getc(file->file);
}

u32 readIntFromFile(const File *file, const b32 bigEndian) {
    u32 output;

    if (bigEndian) {
        output = (u32) getc(file->file);
        output <<= 8;
        output |= getc(file->file);
        output <<= 8;
        output |= getc(file->file);
        output <<= 8;
        output |= getc(file->file);
    }

    else {
        output = (u32) getc(file->file);
        output |= getc(file->file) << 0x8;
        output |= getc(file->file) << 0x10;
        output |= getc(file->file) << 0x18;
    }

    return output;
}

void writeByteToFile(const u32 data, const File *file) {
    putc(data, file->file);
}

void writeIntToFile(const u32 data, const File *file, const b32 bigEndian) {
    if (bigEndian) {
        putc(data >> 0x18, file->file);
        putc((data >> 0x10) & 0xFF, file->file);
        putc((data >> 0x8) & 0xFF, file->file);
        putc(data & 0xFF, file->file);
    }

    else {
        u32 copy = data;

        putc(copy & 0xFF, file->file);
        copy >>= 8;

        putc(copy & 0xFF, file->file);
        copy >>= 8;

        putc(copy & 0xFF, file->file);
        copy >>= 8;

        putc(copy & 0xFF, file->file);
    }
}
