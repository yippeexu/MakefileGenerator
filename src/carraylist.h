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

#ifndef MAKEFILEGENERATOR_CARRAYLIST_H
#define MAKEFILEGENERATOR_CARRAYLIST_H

#include "types.h"

template<typename T>
class CArrayList {

private:

    T *arr;
    u32 capacity;
    u32 len;

public:

    CArrayList(const u32 capacity = 0x10) : arr(new T[capacity]), capacity(capacity), len(0) {

    }

    explicit CArrayList(const CArrayList<T> &other) : arr(new T[other.capacity]), capacity(other.capacity), len(other.len) {
        for (u32 i = 0; i < len; i++) {
            arr[i] = other.arr[i];
        }
    }

    CArrayList &operator= (const CArrayList<T> &other) {
        arr = new T[other.capacity];
        capacity = other.capacity;
        len = other.len;

        for (u32 i = 0; i < len; i++) {
            arr[i] = other.arr[i];
        }
    }

    T *operator[] (const u32 index) {
        return get(index);
    }

    inline u32 size() const {
         return len;
    }

    inline b32 isEmpty() const {
        return (b32) !len;
    }

    void add(const T data) {
        if (len + 1 == capacity)
            resize();

        arr[len++] = data;
    }

    T *get(const u32 index) {
        return index < len ? &arr[index] : NULL;
    }

    b32 contains(const T data) {
        for (u32 i = 0; i < len; i++) {
            if (arr[i] == data)
                return True;
        }

        return False;
    }

    b32 remove(const u32 index) {
        if (isEmpty() || index >= len)
            return False;

        for (u32 i = index; i < len; i++) {
            arr[i] = arr[i + 1];
        }

        len--;

        return True;
    }

private:

    void resize() {
        capacity <<= 1;
        T *newArr = new T[capacity];

        for (u32 i = 0; i < len; i++) {
            newArr[i] = arr[i];
        }

        delete[] arr;
        arr = newArr;
    }

};

typedef CArrayList<char> CharList;

#endif //MAKEFILEGENERATOR_CARRAYLIST_H
