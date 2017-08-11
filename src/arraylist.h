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
