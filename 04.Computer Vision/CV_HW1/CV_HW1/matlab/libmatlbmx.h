
/* $Revision: 1.1 $ */
/*
 * Copyright (c) 1984-1998 by The MathWorks, Inc.
 * All Rights Reserved.
 */
/*
 * MATLAB Compiler: 2.1
 * Date: Sun Oct 15 20:21:17 2000
 * Arguments: "-B" "macro_default" "-O" "all" "-O" "fold_scalar_mxarrays:on"
 * "-O" "fold_non_scalar_mxarrays:on" "-O" "optimize_integer_for_loops:on" "-O"
 * "array_indexing:on" "-O" "optimize_conditionals:on" "-c" "-w" "error" "-w"
 * "enable:break_without_loop" "-w" "enable:undefined_variable" "-w"
 * "enable:undefined_variable_or_unknown_function" "-w"
 * "enable:matlab_method_used" "-w" "disable:using_mex_only_function" "-w"
 * "disable:using_graphics_function" "-w" "disable:max_page_width_violation"
 * "-I" "/devel/R12/build/standalone/libmmfile/modified_M" "-I"
 * "/devel/R12/build/toolbox/matlab/audio" "-I"
 * "/devel/R12/build/toolbox/matlab/datafun" "-I"
 * "/devel/R12/build/toolbox/matlab/datatypes" "-I"
 * "/devel/R12/build/toolbox/matlab/elfun" "-I"
 * "/devel/R12/build/toolbox/matlab/elmat" "-I"
 * "/devel/R12/build/toolbox/matlab/funfun" "-I"
 * "/devel/R12/build/toolbox/matlab/general" "-I"
 * "/devel/R12/build/toolbox/matlab/graphics" "-I"
 * "/devel/R12/build/toolbox/matlab/graph2d" "-I"
 * "/devel/R12/build/toolbox/matlab/graph3d" "-I"
 * "/devel/R12/build/toolbox/matlab/iofun" "-I"
 * "/devel/R12/build/toolbox/matlab/lang" "-I"
 * "/devel/R12/build/toolbox/matlab/matfun" "-I"
 * "/devel/R12/build/toolbox/matlab/ops" "-I"
 * "/devel/R12/build/toolbox/matlab/polyfun" "-I"
 * "/devel/R12/build/toolbox/matlab/specfun" "-I"
 * "/devel/R12/build/toolbox/matlab/strfun" "-I"
 * "/devel/R12/build/toolbox/matlab/timefun" "-I"
 * "/devel/R12/build/toolbox/matlab/sparfun" "-I"
 * "/devel/R12/build/toolbox/compiler" "-I"
 * "/devel/R12/build/toolbox/matlab/uitools" "-Z" "-v" "-W" "lib:libmatlbmx"
 * "-d" "include" "/devel/R12/build/toolbox/matlab/strfun/deblank.m"
 * "/devel/R12/build/toolbox/matlab/ops/intersect.m"
 * "/devel/R12/build/toolbox/matlab/ops/ismember.m"
 * "/devel/R12/build/toolbox/matlab/ops/setdiff.m"
 * "/devel/R12/build/toolbox/matlab/ops/setxor.m"
 * "/devel/R12/build/toolbox/matlab/strfun/strcat.m"
 * "/devel/R12/build/toolbox/matlab/strfun/strmatch.m"
 * "/devel/R12/build/toolbox/matlab/ops/union.m"
 * "/devel/R12/build/toolbox/matlab/ops/unique.m" 
 */

#ifndef MLF_V2
#define MLF_V2 1
#endif

#ifndef __libmatlbmx_h
#define __libmatlbmx_h 1

#ifdef __cplusplus
extern "C" {
#endif

#include "libmatlb.h"

extern void InitializeModule_libmatlbmx(void);
extern void TerminateModule_libmatlbmx(void);
extern mxArray * mlfDeblank(mxArray * s);
extern void mlxDeblank(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNIntersect(int nargout,
                               mxArray * * ia,
                               mxArray * * ib,
                               mxArray * a,
                               mxArray * b,
                               mxArray * flag);
extern mxArray * mlfIntersect(mxArray * * ia,
                              mxArray * * ib,
                              mxArray * a,
                              mxArray * b,
                              mxArray * flag);
extern void mlfVIntersect(mxArray * a, mxArray * b, mxArray * flag);
extern void mlxIntersect(int nlhs,
                         mxArray * plhs[],
                         int nrhs,
                         mxArray * prhs[]);
extern mxArray * mlfIsmember(mxArray * a, mxArray * s, mxArray * flag);
extern void mlxIsmember(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfSetdiff(mxArray * * ia,
                            mxArray * a,
                            mxArray * b,
                            mxArray * flag);
extern void mlxSetdiff(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNSetxor(int nargout,
                            mxArray * * ia,
                            mxArray * * ib,
                            mxArray * a,
                            mxArray * b,
                            mxArray * flag);
extern mxArray * mlfSetxor(mxArray * * ia,
                           mxArray * * ib,
                           mxArray * a,
                           mxArray * b,
                           mxArray * flag);
extern void mlfVSetxor(mxArray * a, mxArray * b, mxArray * flag);
extern void mlxSetxor(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfStrcat(mxArray * synthetic_varargin_argument, ...);
extern void mlxStrcat(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfStrmatch(mxArray * str, mxArray * strs, mxArray * flag);
extern void mlxStrmatch(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNUnion(int nargout,
                           mxArray * * ia,
                           mxArray * * ib,
                           mxArray * a,
                           mxArray * b,
                           mxArray * flag);
extern mxArray * mlfUnion(mxArray * * ia,
                          mxArray * * ib,
                          mxArray * a,
                          mxArray * b,
                          mxArray * flag);
extern void mlfVUnion(mxArray * a, mxArray * b, mxArray * flag);
extern void mlxUnion(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNUnique(int nargout,
                            mxArray * * ndx,
                            mxArray * * pos,
                            mxArray * a,
                            mxArray * flag);
extern mxArray * mlfUnique(mxArray * * ndx,
                           mxArray * * pos,
                           mxArray * a,
                           mxArray * flag);
extern void mlfVUnique(mxArray * a, mxArray * flag);
extern void mlxUnique(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void libmatlbmxInitialize(void);
extern void libmatlbmxTerminate(void);

#ifdef __cplusplus
}
#endif

#endif
