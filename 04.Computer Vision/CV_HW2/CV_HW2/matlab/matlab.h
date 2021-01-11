/*
 * $Revision: 1.79 $  $Date: 2000/04/17 18:49:00 $
 * Copyright (c) 1984-1999 The MathWorks, Inc.
 */

#ifndef matlab_h
#define matlab_h

#ifdef __cplusplus
    extern "C" {
#endif

/* builtin functions */
#include "libmatlb.h"
#include "libmatlbm.h"

/* get Prototypes */
#include "libmmfile.h"

/* graphics library interface */
#include "libsgl.h"

/* graphics library m-file interface */
#include "libmwsglm.h"

#ifndef MLF_ENABLE_ND_ARRAYS
#define mlfOnes(A,B) mlfOnes(A, B, NULL)
#define mlfRand(A,B) mlfRand(A, B, NULL)
#define mlfRandn(A,B) mlfRandn(A, B, NULL)
#define mlfZeros(A,B) mlfZeros(A, B, NULL)
extern mxArray * mlfSize2d(mxArray **OO1, mxArray *RI1, mxArray *OI1);
#define mlfSize mlfSize2d
#endif

/* Backwards compatibility macros */
#if !defined(MLF_V2)
#if !defined(MATLAB_COMPILER_GENERATED_CODE)
#define mlfDel2(A,B,C) mlfDel2(A, B, C, NULL)
#define mlfDatevec(A,B,C,D,E,F) mlfDatevec(A,B,C,D,E,F,NULL)
#define mlfQr(A,B,C,D) mlfQr(A,B,C,D,NULL)
#define mlfLu(A,B,C) mlfLu(A,B,C,NULL)
#define mlfAutomesh(A,B,C) mlfAutomesh(A,B,C,NULL)
#define mlfCov(A,B,C) mlfCov(A,B,C,NULL)
#define mlfStrjust(A) mlfStrjust(A,NULL)
#define mlfNumjac(o2,o3,i1,i2,i3,i4,i5,i6,i7,i8,i9) \
        mlfNumjac(o2,o3,NULL,i1,i2,i3,i4,i5,i6,i7,i8,i9,NULL)
#define mlfFmin(A,B,C,D,E,F) mlfFmin(A,B,C,D,E,F,NULL)
#define mlfFmins(A,B,C,D,E,F) mlfFmins(A,B,C,D,E,F,NULL)
#define mlfFzero(A,B,C,D,E) mlfFzero(NULL, NULL, NULL, A, B, C, D, E, NULL)
#define mlfIcubic(A,B,C) mlfIcubic(A,B,C,NULL)
#define mlfInterp1(A,B,C,D) mlfInterp1(A,B,C,D,NULL)
#define mlfInterp2(A,B,C,D,E,F) mlfInterp2(A,B,C,D,E,F,NULL)
#define mlfInterp4(A,B,C,D,E) mlfInterp4(A,B,C,D,E,NULL)
#define mlfInterp5(A,B,C,D,E) mlfInterp5(A,B,C,D,E,NULL)
#define mlfInterp6(A,B,C,D,E) mlfInterp6(A,B,C,D,E,NULL)
#define mlfQuad(out,A,B,C,D,E,F) mlfQuad(out,A,B,C,D,E,F,NULL)
#define mlfQuad8(out,A,B,C,D,E,F) mlfQuad8(out,A,B,C,D,E,F,NULL)
#endif /*!defined(MATLAB_COMPILER_GENERATED_CODE) */
#endif

#ifndef MLF_USE_VARARGOUT
#define mlfFeval mlfFeval_12
#endif

#ifdef __cplusplus
    }	/* extern "C" */
#endif

#endif /* matlab_h */
