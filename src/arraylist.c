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

#include "arraylist.h"

static void resizeArrayList(ArrayList *);

void initArrayList(ArrayList *list, const u32 capacity, const u32 sizeInBytes) {
    if (list != NULL) {
        list->data = (void **) calloc(capacity, sizeInBytes);
        list->len = 0;
        list->capacity = capacity;
        list->sizeInBytes = sizeInBytes;
    }
}

void freeArrayList(ArrayList *list) {
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->capacity = 0;
        list->len = 0;
        list->sizeInBytes = 0;
    }
}

void addArrayList(ArrayList *list, void *data) {
    if (list != NULL) {

        if (list->len >= list->capacity)
            resizeArrayList(list);

        list->data[list->len++] = data;
    }
}

void insertArrayList(ArrayList *list, void *data, const u32 index) {
    if (list != NULL && index < list->len) {
        
        if (list->len >= list->capacity)
            resizeArrayList(list);

        list->len++;

        for (u32 i = list->len; i > index; i--) {
            list->data[i] = list->data[i - 1];
        }

        list->data[index] = data;
    }
}

void removeArrayList(ArrayList *list, const u32 index) {
    if (list != NULL && index < list->len) {

        for (u32 i = index; i < list->len - 1; i++) {
            list->data[i] = list->data[i + 1];
        }

        list->len--;
    }
}

void *getArrayList(const ArrayList *list, const u32 index) {
    return list != NULL && index < list->len ? list->data[index] : NULL;
}

b32 findArrayList(const ArrayList *list, const void *data) {
    if (list == NULL)
        return False;

    for (u32 i = 0; i < list->len; i++) {
        if (list->data[i] == data)
            return True;
    }

    return False;
}

void resizeArrayList(ArrayList *list) {
    list->capacity <<= 1;
    void **newArr = (void **) calloc(list->capacity, list->sizeInBytes);

    for (u32 i = 0; i < list->len; i++) {
        newArr[i] = list->data[i];
    }

    free(list->data);

    list->data = newArr;
}
