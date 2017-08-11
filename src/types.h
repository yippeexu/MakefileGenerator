#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#define Debug 1
#else 
#define Debug 0
#endif

#if Debug
#define global_status 
#else
#define global_status static
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef u32 b32;

typedef float f32;
typedef double f64;

#ifndef False
#define False (b32) 0u
#endif

#ifndef True
#define True (b32) 1u
#endif

u32 pow(const u32, const u32);

#endif