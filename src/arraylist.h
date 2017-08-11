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

#pragma once

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "types.h"

typedef struct ArrayList {
    void **data;
    u32 len;
    u32 capacity;
    u32 sizeInBytes;
} ArrayList;

inline u32 size(const ArrayList *list) {
    return list != NULL ? list->len : 0;
}

inline b32 isEmpty(const ArrayList *list) {
    return !size(list);
}

void initArrayList(ArrayList *, const u32, const u32);
void freeArrayList(ArrayList *);

void addArrayList(ArrayList *, void *);
void insertArrayList(ArrayList *, void *, const u32);
void removeArrayList(ArrayList *, const u32);
void *getArrayList(const ArrayList *, const u32);
b32 findArrayList(const ArrayList *, const void *);

#endif
