#include "interpreter.h"

#include "arraylist.h"

#define MIN_FILE_SIZE 4u // 'a' + '.' + 'c' + '\0' = 4
#define VALID_FLAG 0x80000000
#define VALID_MASK 0x7fffffff

static b32 isSourceFile(const String *);
static b32 validateVersion(const IFlags *);

b32 interpret(const char **args, const u32 argc, IFlags *flags) {
    if (argc > 1 && args != NULL && flags != NULL) {
        
        flags->args = (char **) args;
        flags->numArgs = argc;

        // ArrayList srcFiles;
        // initArrayList(&srcFiles, 0x10, sizeof(String));
        // printf("Size: %u\n", sizeof(String));

        String stdStr;
        stdStr.cstr = INT_FLAG_STD;
        stdStr.len = strlen(stdStr.cstr);

        for (u32 i = 1; i < argc; i++) {
#if Debug
            printf("[%u]: %s\n", i, args[i]);
#endif
            if (args[i][0] != '-')
                continue;

            else if (!strcmp(args[i], INT_FLAG_CPP)) {
                if (flags->cmode & VALID_FLAG) {
                    return False;
                }

                flags->cmode = VALID_FLAG;
            }

            else {
                String strArg;
                strArg.cstr = (char *) args[i];
                strArg.len = strlen(strArg.cstr);

                if (containsString(&strArg, &stdStr) && strArg.len > 6) {
                    b32 foundStart = False;
                    char *c = NULL;

                    for (c = strArg.cstr; c != '\0'; c++) {
                        if (*c >= '0' && *c <= '9') {
                            foundStart = True;
                            break;
                        }
                    }

                    if (!foundStart)
                        return False;

                    String ver;
                    ver.cstr = c;
                    ver.len = strArg.len - ((u32)(c - strArg.cstr));

                    if (!parseUInt(&ver, &flags->stdver))
                        return False;

                    flags->stdver |= VALID_FLAG;
                }

                // Check if source file for processing:
                else if (isSourceFile(&strArg)) {
                }

                // Invalid flag or file return false.
                else {
                    return False;
                }
            }
        }

#if 0
        return True;
#else
        return validateVersion(flags);
#endif
    }

    return False;
}

b32 isSourceFile(const String *str) {
    if (str == NULL || str->len < MIN_FILE_SIZE)
        return False;

    char *c = NULL;

    for (c = &str->cstr[str->len - 2]; c != str->cstr; c--) {
        if (*c == '.')
            break;
    }

    if (c == NULL)
        return False;
    else if (!strcmp(c, ".h"))
        return True;
    else if (!strcmp(c, ".hpp"))
        return True;
    else if (!strcmp(c, ".hxx"))
        return True;
    else if (!strcmp(c, ".c"))
        return True;
    else if (!strcmp(c, ".cpp"))
        return True;
    else if (!strcmp(c, ".cxx"))
        return True;

    return False;
}

b32 validateVersion(const IFlags *flags) {
    if (flags == NULL)
        return False;

    if (!(flags->cmode & VALID_FLAG))
        return False;

    if (flags->cmode & 1) {
        switch (flags->stdver & VALID_MASK) {
        case 99:
            return True;
        case 11:
            return True;
        case 89:
            return True;
        default:
            return False;
        }
    }

    else {
        switch (flags->stdver & VALID_MASK) {
        case 98:
            return True;
        case 11:
            return True;
        case 14:
            return True;
        case 17:
            return True;
        case 89:
            return True;
        default:
            return False;
        }
    }
}