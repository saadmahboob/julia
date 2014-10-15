/*
 * Copyright (c) 2014 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef PSE_ARRAY_H_
#define PSE_ARRAY_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAX_DIM 4

// In the C file generated by J2C, some types are hand-written like array here. 
// The others are coverted from Julia types by compiler.

// We have to define Array as a C struct instead of a C++ class, because
// C++ class cannot be offloaded to MIC  

#define DECL_C_ARRAY(ELEMENT_TYPE,TYPENAME) \
typedef struct { \
	ELEMENT_TYPE*  data; \
    unsigned num_dim; \
    unsigned dims[MAX_DIM]; \
} TYPENAME##_array; \
TYPENAME##_array inline new_##TYPENAME##_array_1d(ELEMENT_TYPE* _data, unsigned _N1); \
TYPENAME##_array inline new_##TYPENAME##_array_2d(ELEMENT_TYPE* _data, unsigned _N1, unsigned _N2); \
TYPENAME##_array inline new_##TYPENAME##_array_3d(ELEMENT_TYPE* _data, unsigned _N1, unsigned _N2, unsigned _N3); \
TYPENAME##_array inline new_##TYPENAME##_array_4d(ELEMENT_TYPE* _data, unsigned _N1, unsigned _N2, unsigned _N3, unsigned _N4); \
ELEMENT_TYPE inline sum_##TYPENAME##_array(TYPENAME##_array a); 

template <class T>
unsigned ARRAYLEN(T a) {
  unsigned ret = a.dims[0];
  int i;
  for(i = 1; i < a.num_dim; ++i) {
    ret *= a.dims[1];
  }
  return ret;
}
//#define ARRAYLEN(a) (a.N1 * a.N2) 
#define ARRAYSIZE(a, i) (a.dims[i-1]) 
#define ARRAYBOUNDSCHECK(a, i) assert(i <= ARRAYLEN(a))

// It seems that the index has been normalized to 1 dimension in jl_f_arrayset/ref
#define FREEARRAY(a) if (a.data != NULL) { free(a.data); }
#define INITARRAY(a) a.data = NULL

#define ARRAYELEM1(a, i) (a.data[i - 1])
#define ARRAYELEM2(a, i, j) (a.data[(i - 1)*a.dims[1] + j - 1])

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define ARRAYELEM(...) GET_MACRO(__VA_ARGS__, ARRAYELEM2, ARRAYELEM1)(__VA_ARGS__)

//#define false 0
//#define true 1
//typedef int bool; // or #define bool int

#endif /* PSE_ARRAY_H_ */
