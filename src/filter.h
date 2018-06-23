#pragma once

#ifndef FILTER_H
#define FILTER_H

#include "string.h"
#include "arraylist.h"

typedef struct FilterList {
	b32 whiteListMode;
	ArrayList descriptor;
} FilterList;

u32 filterArray(void **, void **, const pint, const u32, FilterList *, CompareFunc);
u32 filterArrayList(ArrayList *, ArrayList *, FilterList *, CompareFunc);

#endif