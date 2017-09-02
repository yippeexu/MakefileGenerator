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

#include "arraylist.h"

#define MIN_FILE_SIZE 4u // 'a' + '.' + 'c' + '\0' = 4
#define VALID_FLAG 0x80000000
#define VALID_MASK 0x7fffffff

static String stdStr;
static String nameStr;

static b32 isSourceFile(const String *);
static void loadSources(SRC *, ArrayList *);
static void loadFlags(SRC *, IFlags *);
static b32 validateDebugMode(const u32);
static b32 validateVersion(const IFlags *);
static b32 containsStringInArrayList(const ArrayList *, const char *);

b32 interpret(const char **args, const u32 argc, IFlags *flags, SRC *sources) {
    if (argc > 1 && args != NULL && flags != NULL && sources != NULL) {
        
        flags->args = (char **) args;
        flags->numArgs = argc;
        flags->name.cstr = NULL;
        flags->name.len = 0;
        flags->stdver = 0;
        flags->debugMode = 0;
        flags->wall = 0;

        ArrayList srcFiles;
        initArrayList(&srcFiles, 0x10, sizeof(char *));
        // printf("Size: %u\n", sizeof(String));

        if (!stdStr.len) {
            stdStr.cstr = INT_FLAG_STD;
            stdStr.len = strlen(stdStr.cstr);
        }

        if (!nameStr.len) {
            nameStr.cstr = INT_FLAG_NAME;
            nameStr.len = strlen(nameStr.cstr);
        }

        for (u32 i = 1; i < argc; i++) {
#if Debug
            printf("[%u]: %s\n", i, args[i]);
#endif
            if (args[i][0] != '-') {
                String strArg;
                strArg.cstr = (char *) args[i];
                strArg.len = strlen(strArg.cstr);

                // Check if source file for processing:
                if (isSourceFile(&strArg)) {
                    if (!containsStringInArrayList(&srcFiles, strArg.cstr))
                        addArrayList(&srcFiles, strArg.cstr);
                }

                else {
                    continue;
                }
            }

            else if (!strcmp(args[i], INT_FLAG_CPP)) {
                if (flags->cmode & VALID_FLAG) {
                    return False;
                }

                flags->cmode = VALID_FLAG;
            }

            else if (!strcmp(args[i], INT_FLAG_DEBUG_MODE)) {
                if (flags->debugMode & VALID_FLAG) {
                    return False;
                }

                flags->debugMode = VALID_FLAG | 1;
            }

            else if (!strcmp(args[i], INT_FLAG_WALL)) {
                if (flags->wall & VALID_FLAG) {
                    return False;
                }

                flags->wall = VALID_FLAG | 1;
            }

            else {
                String strArg;
                strArg.cstr = (char *) args[i];
                strArg.len = strlen(strArg.cstr);

                // Is -std=c/c++xx
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

                else if (containsString(&strArg, &nameStr) && strArg.len > 7) {
                    flags->name.cstr = &strArg.cstr[6];
                    flags->name.len = strArg.len - 6;
                }

#if 0
                // Check if source file for processing:
                else if (isSourceFile(&strArg)) {
                    if (!containsStringInArrayList(&srcFiles, strArg.cstr))
                        addArrayList(&srcFiles, strArg.cstr);
                }
#endif

                // Invalid flag or file return false.
                else {
                    return False;
                }
            }
        }

#if 0
        return True;
#else
        // return validateVersion(flags) && srcFiles.len;
        if (!srcFiles.len || !validateDebugMode(flags->debugMode) || 
            !validateDebugMode(flags->wall) || !validateVersion(flags))
            return False;

        loadSources(sources, &srcFiles);
        loadFlags(sources, flags);
        freeArrayList(&srcFiles);
        
        return True;
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

void loadSources(SRC *sources, ArrayList *list) {
    if (sources != NULL && list != NULL && list->len) {
        sources->len = list->len;
        sources->srcFiles = (String *) calloc(sources->len, sizeof(String));
        char **arr = (char **) list->data;

        for (u32 i = 0; i < list->len; i++) {
            sources->srcFiles[i].cstr = arr[i];
            sources->srcFiles[i].len = strlen(arr[i]);
        }

    }
}

void loadFlags(SRC *sources, IFlags *flags) {
    if (sources != NULL && flags != NULL) {
        sources->stdver = flags->stdver & VALID_MASK;
        sources->cmode = flags->cmode & 1;

        ArrayList list;
        initArrayList(&list, 0x40, sizeof(char));

        if (flags->debugMode == (VALID_FLAG | 1)) {
            addArrayList(&list, (void *) '-');
            addArrayList(&list, (void *) 'g');
            addArrayList(&list, (void *) ' ');
        }

        if (flags->wall == (VALID_FLAG | 1)) {
            addArrayList(&list, (void *) '-');
            addArrayList(&list, (void *) 'W');
            addArrayList(&list, (void *) 'a');
            addArrayList(&list, (void *) 'l');
            addArrayList(&list, (void *) 'l');
            addArrayList(&list, (void *) ' ');
        }

        addArrayList(&list, (void *) '-');
        addArrayList(&list, (void *) 's');
        addArrayList(&list, (void *) 't');
        addArrayList(&list, (void *) 'd');
        addArrayList(&list, (void *) '=');
        addArrayList(&list, (void *) 'c');

        if (flags->cmode & 1) {
            switch (flags->stdver & VALID_MASK) {
                case 99:
                    addArrayList(&list, (void *) '9');
                    addArrayList(&list, (void *) '9');
                    break;
                case 11:
                    addArrayList(&list, (void *) '1');
                    addArrayList(&list, (void *) '1');
                    break;
                case 89:
                    addArrayList(&list, (void *) '8');
                    addArrayList(&list, (void *) '9');
                    break;
                default:
                    break;
            }
        }

        else {
            addArrayList(&list, (void *) '+');
            addArrayList(&list, (void *) '+');

            switch (flags->stdver & VALID_MASK) {
                case 98:
                    addArrayList(&list, (void *) '9');
                    addArrayList(&list, (void *) '8');
                    break;
                case 11:
                    addArrayList(&list, (void *) '1');
                    addArrayList(&list, (void *) '1');
                    break;
                case 14:
                    addArrayList(&list, (void *) '1');
                    addArrayList(&list, (void *) '4');
                    break;
                case 17:
                    addArrayList(&list, (void *) '1');
                    addArrayList(&list, (void *) '7');
                    break;
                case 89:
                    addArrayList(&list, (void *) '8');
                    addArrayList(&list, (void *) '9');
                    break;
                default:
                    break;
            }
        }

        // addArrayList(&list, (void *) ' ');
        // addArrayList(&list, (void *) '\0');

        // constructString(&sources->flags, *(char **) &list.data);

        sources->flags.len = list.len; // +1;
        sources->flags.cstr = calloc(sources->flags.len, sizeof(char));

        for (u32 i = 0; i < list.len; i++) {
            sources->flags.cstr[i] = *(char *) &list.data[i];
        }

        sources->name.cstr = flags->name.cstr;
        sources->name.len = flags->name.len - 1; // TODO: Change this hack?!??!

        freeArrayList(&list);
    }
}

b32 validateDebugMode(const u32 flag) {
    return !flag || flag == (VALID_FLAG | 1);
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

b32 containsStringInArrayList(const ArrayList *list, const char *str) {
    if (list == NULL || str == NULL || !list->len)
        return False;

    const char **arr = (const char **) list->data;

    for (u32 i = 0; i < list->len; i++) {
        if (arr[i] == str)
            return True;

        if (!strcmp(arr[i], str))
            return True;
    }

    return False;
}