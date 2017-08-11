#include "string.h"

b32 isNum(const char c) {
    return c >= '0' && c <= '9';
}

u32 charToNum(const char c) {
    return c - '0';
}

u32 strlen(const char *cstr) {
    if (cstr == NULL)
        return 0;

    u32 count = 0;

    while (cstr[count++] != '\0');

    return count;
}

s32 strcmp(const char *left, const char *right) {
    if (left == NULL && right == NULL)
        return 0;
    else if (left != NULL && right == NULL)
        return 1;
    else if (left == NULL && right != NULL)
        return -1;

    u32 i;

    for (i = 0; left[i] != '\0'; i++) {
        if (right[i] == '\0' || left[i] > right[i])
            return 1;

        else if (left[i] < right[i])
            return -1;
    }

    return right[i] == '\0' ? 0 : 1;
}

b32 containsString(const String *string, const String *find) {
    if (string == NULL || find == NULL || string->len < find->len)
        return False;

    u32 index = 0;
    for (u32 i = 0; i < string->len; i++) {
        if (string->cstr[i] == find->cstr[index]) {
            if (++index == find->len - 1)
                return True;
        }

        else
            index = 0;
    }

    return index == find->len - 1;
}

b32 parseUInt(const String *string, u32 *output) {
    if (string == NULL || string->cstr == NULL || !string->len || output == NULL)
        return False;

    *output = 0;

    u32 power = 1;
    for (s32 i = string->len - 2; i >= 0; i--) {
        if (!isNum(string->cstr[i]))
            return False;

        *output += charToNum(string->cstr[i]) * power;
        power *= 10;
    }

    return True;
}

void constructString(String *string, const char *cstr) {
    if (string != NULL && cstr != NULL) {
        string->len = strlen(cstr);
        string->cstr = (char *) calloc(string->len, sizeof(char));

        for (u32 i = 0; i < string->len; i++) {
            string->cstr[i] = cstr[i];
        }
    }
}

void desrtuctString(String *string) {
    if (string != NULL) {
        free(string->cstr);
        string->cstr = NULL;
        string->len = 0;
    }
}