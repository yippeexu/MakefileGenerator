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

#ifndef MAKEGEN_ARRAYLIST_H
#define MAKEGEN_ARRAYLIST_H

#include "types.h"

typedef struct ArrayList {
    void **arr;
    u32 capacity;
    u32 sizeOf;
    u32 len;
} ArrayList;

typedef struct ArrayListIterator {
    ArrayList *list;
    u32 index;
} ArrayListIterator;

void constructArrayList(ArrayList *, const u32, const u32);
void destructArrayList(ArrayList *);

void *getArrayList(const ArrayList *, const u32);
void setArrayList(ArrayList *, const u32, void *);

void addArrayList(ArrayList *, const void *);
void removeArrayList(ArrayList *, const u32);
void removeLastArrayList(ArrayList *);

void constructArrayListIterator(ArrayListIterator *, const ArrayList *);
void desetructArrayListIterator(ArrayListIterator *);
b32 hasNextArrayListIterator(const ArrayListIterator *);
void *nextArrayListIterator(ArrayListIterator *);

#endif //MAKEGEN_ARRAYLIST_H
