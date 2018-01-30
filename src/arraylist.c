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

static void resizeArray(ArrayList *);
extern void myFree(void *, const char *);

void resizeArray(ArrayList *list) {
    const u32 newCapacity = list->capacity << 1;
    void **newArr = calloc(list->capacity, list->sizeOf);

    for (u32 i = 0; i < list->capacity; i++) {
        newArr[i] = list->arr[i];
    }

    // free(list->arr);
    myFree(list->arr, "Re-sizing ArrayList");

    list->arr = newArr;
    list->capacity = newCapacity;
}

void constructArrayList(ArrayList *list, const u32 capacity, const u32 sizeOf) {
    list->arr = calloc(capacity, sizeOf);

    if (list->arr == NULL) {
        perror("Error Calloc'ing!\n");
        exit(-1);
    }

    list->capacity = capacity;
    list->sizeOf = sizeOf;
    list->len = 0;
}

void destructArrayList(ArrayList *list) {
    // free(list->arr);
    myFree(list->arr, "ArrayList");

#if Debug
    list->arr = NULL;
    list->capacity = 0;
    list->sizeOf = 0;
    list->len = 0;
#endif
}

void *getArrayList(const ArrayList *list, const u32 index) {
    // return index < list->len ? *(list->arr + (index * list->sizeOf)) : NULL;
    return index < list->len ? list->arr[index] : NULL;
}

void setArrayList(ArrayList *list, const u32 index, void *data) {
    if (index < list->len) {
        list->arr[index] = data;
    }
}

void addArrayList(ArrayList *list, const void *data) {
    if (list->len + 1 >= list->capacity)
        resizeArray(list);

    list->arr[list->len++] = (void *) data;
}

void removeArrayList(ArrayList *list, const u32 index) {
    if (!list->len || index >= list->len)
        return;

    for (u32 i = index; i < list->len - 1; i++) {
        list->arr[i] = list->arr[i + 1];
    }

    list->len--;
}

void constructArrayListIterator(ArrayListIterator *iter, const ArrayList *list) {
    iter->list = (ArrayList *) list;
    iter->index = 0;
}

void desetructArrayListIterator(ArrayListIterator *list) {

}

b32 hasNextArrayListIterator(const ArrayListIterator *iter) {
    return (b32) (iter != NULL && iter->list != NULL && iter->index < iter->list->len);
}

void *nextArrayListIterator(ArrayListIterator *iter) {
    if (iter->list != NULL && iter->index < iter->list->len) {
        // iter->index++;
        void *ret = iter->list->arr[iter->index];
        iter->index++;

        return ret;
    }

    return NULL;
}