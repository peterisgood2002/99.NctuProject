
/* $Revision: 1.1 $ */
/*
 * Copyright (c) 1984-1998 by The MathWorks, Inc.
 * All Rights Reserved.
 */
/*
 * MATLAB Compiler: 2.2
 * Date: Sun Apr 29 04:44:20 2001
 * Arguments: "-B" "macro_default" "-O" "all" "-O" "fold_scalar_mxarrays:on"
 * "-O" "fold_non_scalar_mxarrays:on" "-O" "optimize_integer_for_loops:on" "-O"
 * "array_indexing:on" "-O" "optimize_conditionals:on" "-v" "-t" "-Z" "-h" "-I"
 * "/devel/R12p1/build/standalone/libmwsglm/modified_M" "-I"
 * "/devel/R12p1/build/toolbox/matlab/audio" "-I"
 * "/devel/R12p1/build/toolbox/matlab/datafun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/datatypes" "-I"
 * "/devel/R12p1/build/toolbox/matlab/elfun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/elmat" "-I"
 * "/devel/R12p1/build/toolbox/matlab/funfun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/general" "-I"
 * "/devel/R12p1/build/toolbox/matlab/graphics" "-I"
 * "/devel/R12p1/build/toolbox/matlab/graph2d" "-I"
 * "/devel/R12p1/build/toolbox/matlab/graph3d" "-I"
 * "/devel/R12p1/build/toolbox/matlab/iofun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/lang" "-I"
 * "/devel/R12p1/build/toolbox/matlab/matfun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/ops" "-I"
 * "/devel/R12p1/build/toolbox/matlab/polyfun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/specfun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/strfun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/timefun" "-I"
 * "/devel/R12p1/build/toolbox/matlab/sparfun" "-I"
 * "/devel/R12p1/build/toolbox/compiler" "-I"
 * "/devel/R12p1/build/toolbox/matlab/uitools" "-I"
 * "/devel/R12p1/build/toolbox/matlab/demos" "-I"
 * "/devel/R12p1/build/toolbox/matlab/specgraph" "-w" "error" "-w"
 * "enable:break_without_loop" "-w" "enable:undefined_variable" "-w"
 * "enable:undefined_variable_or_unknown_function" "-w"
 * "enable:matlab_method_used" "-w" "disable:using_mex_only_function" "-w"
 * "disable:using_graphics_function" "-w" "disable:max_page_width_violation"
 * "-O" "all" "-O" "fold_scalar_mxarrays:on" "-O" "fold_non_scalar_mxarrays:on"
 * "-O" "optimize_integer_for_loops:on" "-O" "array_indexing:on" "-O"
 * "optimize_conditionals:on" "-c" "-d" "mfile_src" "libmmfile.mlib" "-w"
 * "enable:too_many_inputs" "-w" "enable:unix_dos_second_argument" "-W"
 * "lib:libmwsglm" "-B" "mfile_src/libmwsglm_response" "align" "brighten"
 * "bwcontr" "camzoom" "caxis" "close" "closereq" "clruprop" "colorbar"
 * "colordef" "colormap" "colornone" "datachildren" "dialog" "errordlg"
 * "filemenufcn" "findall" "gcbf" "gcbo" "gco" "getobj" "getuprop" "ginput"
 * "gpf" "gray" "helpwin" "hgload" "hgsave" "hsv" "imwrite" "ind2rgb"
 * "inputdlg" "jet" "jpropeditutils" "legend" "makemenu" "menu" "menubar"
 * "menulabel" "moveaxis" "msgbox" "noanimate" "nodither" "open" "plotedit"
 * "polar" "print" "printdlg" "printjob" "printtables" "propedit" "putdowntext"
 * "questdlg" "rotate3d" "saveas" "savtoner" "scribeclearmode" "setptr"
 * "setuprop" "textwrap" "uiclearmode" "uiopen" "uirestore" "uiresume"
 * "uisuspend" "uiwait" "warndlg" "watchoff" "watchon" "whitebg" "zoom"
 * "studio" "matlabrc" "defaultprinter_windows" "printopt" "saveasfig"
 * "saveasm" "-k" "mfile_src/libmwsglm_dep.mak" 
 */

#ifndef MLF_V2
#define MLF_V2 1
#endif

#ifndef __libmwsglm_h
#define __libmwsglm_h 1

#ifdef __cplusplus
extern "C" {
#endif

#include "libmatlb.h"

extern void InitializeModule_libmwsglm(void);
extern void TerminateModule_libmwsglm(void);
extern mxArray * mlfNAlign(int nargout,
                           mlfVarargoutList * varargout,
                           mxArray * Action,
                           ...);
extern mxArray * mlfAlign(mlfVarargoutList * varargout, mxArray * Action, ...);
extern void mlfVAlign(mxArray * Action, ...);
extern void mlxAlign(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNBrighten(int nargout, mxArray * map, mxArray * beta);
extern mxArray * mlfBrighten(mxArray * map, mxArray * beta);
extern void mlfVBrighten(mxArray * map, mxArray * beta);
extern void mlxBrighten(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfBwcontr(mxArray * cc);
extern void mlxBwcontr(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfCamzoom(mxArray * arg1, mxArray * arg2);
extern void mlxCamzoom(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNCaxis(int nargout,
                           mxArray * * cmax,
                           mxArray * arg1,
                           mxArray * arg2);
extern mxArray * mlfCaxis(mxArray * * cmax, mxArray * arg1, mxArray * arg2);
extern void mlfVCaxis(mxArray * arg1, mxArray * arg2);
extern void mlxCaxis(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNClose(int nargout, ...);
extern mxArray * mlfClose(mxArray * synthetic_varargin_argument, ...);
extern void mlfVClose(mxArray * synthetic_varargin_argument, ...);
extern void mlxClose(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfClosereq(void);
extern void mlxClosereq(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfClruprop(mxArray * handle, mxArray * name);
extern void mlxClruprop(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNColorbar(int nargout,
                              mxArray * arg1,
                              mxArray * arg2,
                              mxArray * arg3);
extern mxArray * mlfColorbar(mxArray * arg1, mxArray * arg2, mxArray * arg3);
extern void mlfVColorbar(mxArray * arg1, mxArray * arg2, mxArray * arg3);
extern void mlxColorbar(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNColordef(int nargout, mxArray * arg1, mxArray * arg2);
extern mxArray * mlfColordef(mxArray * arg1, mxArray * arg2);
extern void mlfVColordef(mxArray * arg1, mxArray * arg2);
extern void mlxColordef(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNColormap(int nargout, mxArray * arg1, mxArray * arg2);
extern mxArray * mlfColormap(mxArray * arg1, mxArray * arg2);
extern void mlfVColormap(mxArray * arg1, mxArray * arg2);
extern void mlxColormap(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfColornone(mxArray * state, mxArray * fig);
extern void mlxColornone(int nlhs,
                         mxArray * plhs[],
                         int nrhs,
                         mxArray * prhs[]);
extern mxArray * mlfDatachildren(mxArray * parent);
extern void mlxDatachildren(int nlhs,
                            mxArray * plhs[],
                            int nrhs,
                            mxArray * prhs[]);
extern mxArray * mlfDialog(mxArray * synthetic_varargin_argument, ...);
extern void mlxDialog(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNErrordlg(int nargout,
                              mlfVarargoutList * varargout,
                              mxArray * ErrorString,
                              mxArray * DlgName,
                              mxArray * Replace);
extern mxArray * mlfErrordlg(mlfVarargoutList * varargout,
                             mxArray * ErrorString,
                             mxArray * DlgName,
                             mxArray * Replace);
extern void mlfVErrordlg(mxArray * ErrorString,
                         mxArray * DlgName,
                         mxArray * Replace);
extern void mlxErrordlg(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfFilemenufcn(mxArray * hfig, mxArray * cmd);
extern void mlxFilemenufcn(int nlhs,
                           mxArray * plhs[],
                           int nrhs,
                           mxArray * prhs[]);
extern mxArray * mlfFindall(mxArray * HandleList, ...);
extern void mlxFindall(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfGcbf(void);
extern void mlxGcbf(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNGcbo(int nargout, mxArray * * fig);
extern mxArray * mlfGcbo(mxArray * * fig);
extern void mlfVGcbo(void);
extern void mlxGcbo(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfGco(mxArray * fig);
extern void mlxGco(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfGetobj(mxArray * HG);
extern void mlxGetobj(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfGetuprop(mxArray * handle, mxArray * name);
extern void mlxGetuprop(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNGinput(int nargout,
                            mxArray * * out2,
                            mxArray * * out3,
                            mxArray * arg1);
extern mxArray * mlfGinput(mxArray * * out2, mxArray * * out3, mxArray * arg1);
extern void mlfVGinput(mxArray * arg1);
extern void mlxGinput(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfGpf(void);
extern void mlxGpf(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfGray(mxArray * m);
extern void mlxGray(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfHelpwin(mxArray * arg1, mxArray * arg2, mxArray * arg3, ...);
extern void mlxHelpwin(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNHgload(int nargout,
                            mxArray * * old_props,
                            mxArray * filename,
                            ...);
extern mxArray * mlfHgload(mxArray * * old_props, mxArray * filename, ...);
extern void mlfVHgload(mxArray * filename, ...);
extern void mlxHgload(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfHgsave(mxArray * synthetic_varargin_argument, ...);
extern void mlxHgsave(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfHsv(mxArray * m);
extern void mlxHsv(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfImwrite(mxArray * synthetic_varargin_argument, ...);
extern void mlxImwrite(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNInd2rgb(int nargout,
                             mxArray * * g,
                             mxArray * * b,
                             mxArray * a,
                             mxArray * cm);
extern mxArray * mlfInd2rgb(mxArray * * g,
                            mxArray * * b,
                            mxArray * a,
                            mxArray * cm);
extern void mlfVInd2rgb(mxArray * a, mxArray * cm);
extern void mlxInd2rgb(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNInputdlg(int nargout,
                              mxArray * Prompt,
                              mxArray * Title,
                              mxArray * NumLines,
                              mxArray * DefAns,
                              mxArray * Resize);
extern mxArray * mlfInputdlg(mxArray * Prompt,
                             mxArray * Title,
                             mxArray * NumLines,
                             mxArray * DefAns,
                             mxArray * Resize);
extern void mlfVInputdlg(mxArray * Prompt,
                         mxArray * Title,
                         mxArray * NumLines,
                         mxArray * DefAns,
                         mxArray * Resize);
extern void mlxInputdlg(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfJet(mxArray * m);
extern void mlxJet(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNJpropeditutils(int nargout,
                                    mlfVarargoutList * varargout,
                                    mxArray * action,
                                    ...);
extern mxArray * mlfJpropeditutils(mlfVarargoutList * varargout,
                                   mxArray * action,
                                   ...);
extern void mlfVJpropeditutils(mxArray * action, ...);
extern void mlxJpropeditutils(int nlhs,
                              mxArray * plhs[],
                              int nrhs,
                              mxArray * prhs[]);
extern mxArray * mlfNLegend(int nargout, mxArray * * labelhandles, ...);
extern mxArray * mlfLegend(mxArray * * labelhandles, ...);
extern void mlfVLegend(mxArray * synthetic_varargin_argument, ...);
extern void mlxLegend(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfMakemenu(mxArray * fig,
                             mxArray * labels,
                             mxArray * calls,
                             mxArray * tags);
extern void mlxMakemenu(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfMenu(mxArray * xHeader, ...);
extern void mlxMenu(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfMenubar(void);
extern void mlxMenubar(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfMenulabel(mxArray * * acc, mxArray * inlabel);
extern void mlxMenulabel(int nlhs,
                         mxArray * plhs[],
                         int nrhs,
                         mxArray * prhs[]);
extern void mlfMoveaxis(mxArray * arg);
extern void mlxMoveaxis(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNMsgbox(int nargout, mlfVarargoutList * varargout, ...);
extern mxArray * mlfMsgbox(mlfVarargoutList * varargout, ...);
extern void mlfVMsgbox(mxArray * synthetic_varargin_argument, ...);
extern void mlxMsgbox(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfNoanimate(mxArray * state, mxArray * fig);
extern void mlxNoanimate(int nlhs,
                         mxArray * plhs[],
                         int nrhs,
                         mxArray * prhs[]);
extern void mlfNodither(mxArray * state, mxArray * fig);
extern void mlxNodither(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNOpen(int nargout, mxArray * name);
extern mxArray * mlfOpen(mxArray * name);
extern void mlfVOpen(mxArray * name);
extern void mlxOpen(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNPlotedit(int nargout, mlfVarargoutList * varargout, ...);
extern mxArray * mlfPlotedit(mlfVarargoutList * varargout, ...);
extern void mlfVPlotedit(mxArray * synthetic_varargin_argument, ...);
extern void mlxPlotedit(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNPolar(int nargout,
                           mxArray * theta,
                           mxArray * rho,
                           mxArray * line_style);
extern mxArray * mlfPolar(mxArray * theta, mxArray * rho, mxArray * line_style);
extern void mlfVPolar(mxArray * theta, mxArray * rho, mxArray * line_style);
extern void mlxPolar(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNPrint(int nargout, mlfVarargoutList * varargout, ...);
extern mxArray * mlfPrint(mlfVarargoutList * varargout, ...);
extern void mlfVPrint(mxArray * synthetic_varargin_argument, ...);
extern void mlxPrint(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfPrintdlg(mxArray * synthetic_varargin_argument, ...);
extern void mlxPrintdlg(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfPrintjob(void);
extern void mlxPrintjob(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfPrinttables(mxArray * * devices,
                                mxArray * * extensions,
                                mxArray * * classes,
                                mxArray * * colorDevs,
                                mxArray * * destinations,
                                mxArray * * descriptions,
                                mxArray * pj);
extern void mlxPrinttables(int nlhs,
                           mxArray * plhs[],
                           int nrhs,
                           mxArray * prhs[]);
extern mxArray * mlfNPropedit(int nargout,
                              mlfVarargoutList * varargout,
                              mxArray * h,
                              ...);
extern mxArray * mlfPropedit(mlfVarargoutList * varargout, mxArray * h, ...);
extern void mlfVPropedit(mxArray * h, ...);
extern void mlxPropedit(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfPutdowntext(mlfVarargoutList * varargout, ...);
extern void mlxPutdowntext(int nlhs,
                           mxArray * plhs[],
                           int nrhs,
                           mxArray * prhs[]);
extern mxArray * mlfNQuestdlg(int nargout,
                              mxArray * Question,
                              mxArray * Title,
                              mxArray * Btn1,
                              mxArray * Btn2,
                              mxArray * Btn3,
                              mxArray * Default);
extern mxArray * mlfQuestdlg(mxArray * Question,
                             mxArray * Title,
                             mxArray * Btn1,
                             mxArray * Btn2,
                             mxArray * Btn3,
                             mxArray * Default);
extern void mlfVQuestdlg(mxArray * Question,
                         mxArray * Title,
                         mxArray * Btn1,
                         mxArray * Btn2,
                         mxArray * Btn3,
                         mxArray * Default);
extern void mlxQuestdlg(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfRotate3d(mxArray * arg, mxArray * arg2);
extern void mlxRotate3d(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfSaveas(mxArray * h, mxArray * name, mxArray * format);
extern void mlxSaveas(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfSavtoner(mxArray * state, mxArray * fig);
extern void mlxSavtoner(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfScribeclearmode(mxArray * fig, ...);
extern void mlxScribeclearmode(int nlhs,
                               mxArray * plhs[],
                               int nrhs,
                               mxArray * prhs[]);
extern mxArray * mlfNSetptr(int nargout,
                            mlfVarargoutList * varargout,
                            mxArray * fig,
                            mxArray * curs,
                            mxArray * fname);
extern mxArray * mlfSetptr(mlfVarargoutList * varargout,
                           mxArray * fig,
                           mxArray * curs,
                           mxArray * fname);
extern void mlfVSetptr(mxArray * fig, mxArray * curs, mxArray * fname);
extern void mlxSetptr(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfSetuprop(mxArray * h, mxArray * name, mxArray * value);
extern void mlxSetuprop(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNTextwrap(int nargout, mlfVarargoutList * varargout, ...);
extern mxArray * mlfTextwrap(mlfVarargoutList * varargout, ...);
extern void mlfVTextwrap(mxArray * synthetic_varargin_argument, ...);
extern void mlxTextwrap(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfUiclearmode(mxArray * fig, ...);
extern void mlxUiclearmode(int nlhs,
                           mxArray * plhs[],
                           int nrhs,
                           mxArray * prhs[]);
extern void mlfUiopen(mxArray * type, mxArray * direct);
extern void mlxUiopen(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfUirestore(mxArray * uistate, mxArray * kidsOnly);
extern void mlxUirestore(int nlhs,
                         mxArray * plhs[],
                         int nrhs,
                         mxArray * prhs[]);
extern void mlfUiresume(mxArray * hFigDlg);
extern void mlxUiresume(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfUisuspend(mxArray * fig);
extern void mlxUisuspend(int nlhs,
                         mxArray * plhs[],
                         int nrhs,
                         mxArray * prhs[]);
extern void mlfUiwait(mxArray * hFigDlg);
extern void mlxUiwait(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNWarndlg(int nargout,
                             mlfVarargoutList * varargout,
                             mxArray * WarnString,
                             mxArray * DlgName,
                             mxArray * Replace);
extern mxArray * mlfWarndlg(mlfVarargoutList * varargout,
                            mxArray * WarnString,
                            mxArray * DlgName,
                            mxArray * Replace);
extern void mlfVWarndlg(mxArray * WarnString,
                        mxArray * DlgName,
                        mxArray * Replace);
extern void mlxWarndlg(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfWatchoff(mxArray * figNumber);
extern void mlxWatchoff(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfWatchon(void);
extern void mlxWatchon(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfWhitebg(mxArray * fig, mxArray * c);
extern void mlxWhitebg(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNZoom(int nargout, ...);
extern mxArray * mlfZoom(mxArray * synthetic_varargin_argument, ...);
extern void mlfVZoom(mxArray * synthetic_varargin_argument, ...);
extern void mlxZoom(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNStudio(int nargout, mxArray * Option);
extern mxArray * mlfStudio(mxArray * Option);
extern void mlfVStudio(mxArray * Option);
extern void mlxStudio(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfMatlabrc(void);
extern void mlxMatlabrc(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfDefaultprinter_windows(void);
extern void mlxDefaultprinter_windows(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern mxArray * mlfPrintopt(mxArray * * dev);
extern void mlxPrintopt(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfSaveasfig(mxArray * h, mxArray * name);
extern void mlxSaveasfig(int nlhs,
                         mxArray * plhs[],
                         int nrhs,
                         mxArray * prhs[]);
extern void mlfSaveasm(mxArray * h, mxArray * filename);
extern void mlxSaveasm(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern void mlfIofun_private_writebmp(mxArray * X,
                                      mxArray * map,
                                      mxArray * filename,
                                      ...);
extern void mlxIofun_private_writebmp(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern void mlfIofun_private_writehdf(mxArray * data,
                                      mxArray * map,
                                      mxArray * filename,
                                      ...);
extern void mlxIofun_private_writehdf(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern void mlfIofun_private_writejpg(mxArray * data,
                                      mxArray * map,
                                      mxArray * filename,
                                      ...);
extern void mlxIofun_private_writejpg(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern void mlfIofun_private_writepcx(mxArray * X,
                                      mxArray * map,
                                      mxArray * fname,
                                      ...);
extern void mlxIofun_private_writepcx(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern void mlfIofun_private_writepng(mxArray * data,
                                      mxArray * map,
                                      mxArray * filename,
                                      ...);
extern void mlxIofun_private_writepng(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern void mlfIofun_private_writetif(mxArray * data,
                                      mxArray * map,
                                      mxArray * filename,
                                      ...);
extern void mlxIofun_private_writetif(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern void mlfIofun_private_writexwd(mxArray * M,
                                      mxArray * CM,
                                      mxArray * fname,
                                      ...);
extern void mlxIofun_private_writexwd(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern mxArray * mlfGraphics_private_finish(mxArray * pj_in);
extern void mlxGraphics_private_finish(int nlhs,
                                       mxArray * plhs[],
                                       int nrhs,
                                       mxArray * prhs[]);
extern mxArray * mlfGraphics_private_ghostscript(mxArray * pj_in);
extern void mlxGraphics_private_ghostscript(int nlhs,
                                            mxArray * plhs[],
                                            int nrhs,
                                            mxArray * prhs[]);
extern void mlfGraphics_private_hpgl(mxArray * pj);
extern void mlxGraphics_private_hpgl(int nlhs,
                                     mxArray * plhs[],
                                     int nrhs,
                                     mxArray * prhs[]);
extern mxArray * mlfGraphics_private_inputcheck(mxArray * * devices,
                                                mxArray * * options,
                                                mxArray * pj_in,
                                                ...);
extern void mlxGraphics_private_inputcheck(int nlhs,
                                           mxArray * plhs[],
                                           int nrhs,
                                           mxArray * prhs[]);
extern mxArray * mlfGraphics_private_name(mxArray * pj_in);
extern void mlxGraphics_private_name(int nlhs,
                                     mxArray * plhs[],
                                     int nrhs,
                                     mxArray * prhs[]);
extern mxArray * mlfGraphics_private_newpage(mxArray * pj_in);
extern void mlxGraphics_private_newpage(int nlhs,
                                        mxArray * plhs[],
                                        int nrhs,
                                        mxArray * prhs[]);
extern mxArray * mlfGraphics_private_positions(mxArray * pj_in,
                                               mxArray * handles);
extern void mlxGraphics_private_positions(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern mxArray * mlfGraphics_private_prepare(mxArray * pj_in, mxArray * h);
extern void mlxGraphics_private_prepare(int nlhs,
                                        mxArray * plhs[],
                                        int nrhs,
                                        mxArray * prhs[]);
extern mxArray * mlfGraphics_private_preparepointers(mxArray * pj_in);
extern void mlxGraphics_private_preparepointers(int nlhs,
                                                mxArray * plhs[],
                                                int nrhs,
                                                mxArray * prhs[]);
extern mxArray * mlfGraphics_private_render(mxArray * pj_in, mxArray * h);
extern void mlxGraphics_private_render(int nlhs,
                                       mxArray * plhs[],
                                       int nrhs,
                                       mxArray * prhs[]);
extern mxArray * mlfGraphics_private_restore(mxArray * pj_in, mxArray * h);
extern void mlxGraphics_private_restore(int nlhs,
                                        mxArray * plhs[],
                                        int nrhs,
                                        mxArray * prhs[]);
extern void mlfGraphics_private_restorepointers(mxArray * pj);
extern void mlxGraphics_private_restorepointers(int nlhs,
                                                mxArray * plhs[],
                                                int nrhs,
                                                mxArray * prhs[]);
extern void mlfGraphics_private_send(mxArray * pj);
extern void mlxGraphics_private_send(int nlhs,
                                     mxArray * plhs[],
                                     int nrhs,
                                     mxArray * prhs[]);
extern mxArray * mlfGraphics_private_setup(mxArray * pj);
extern void mlxGraphics_private_setup(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern mxArray * mlfGraphics_private_start(mxArray * pj_in);
extern void mlxGraphics_private_start(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern mxArray * mlfGraphics_private_validate(mxArray * pj_in);
extern void mlxGraphics_private_validate(int nlhs,
                                         mxArray * plhs[],
                                         int nrhs,
                                         mxArray * prhs[]);
extern void mlfGraph2d_private_fixtoolbar(mxArray * fig);
extern void mlxGraph2d_private_fixtoolbar(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern mxArray * mlfNGraph2d_private_newtext(int nargout,
                                             mlfVarargoutList * varargout,
                                             ...);
extern mxArray * mlfGraph2d_private_newtext(mlfVarargoutList * varargout, ...);
extern void mlfVGraph2d_private_newtext(mxArray * synthetic_varargin_argument,
                                        ...);
extern void mlxGraph2d_private_newtext(int nlhs,
                                       mxArray * plhs[],
                                       int nrhs,
                                       mxArray * prhs[]);
extern void mlfGraph2d_private_promoteoverlay(mxArray * fig);
extern void mlxGraph2d_private_promoteoverlay(int nlhs,
                                              mxArray * plhs[],
                                              int nrhs,
                                              mxArray * prhs[]);
extern void mlfScribefiglisten(mxArray * fig, mxArray * onoff);
extern void mlxScribefiglisten(int nlhs,
                               mxArray * plhs[],
                               int nrhs,
                               mxArray * prhs[]);
extern mxArray * mlfGeneral_private_isfigure(mxArray * h);
extern void mlxGeneral_private_isfigure(int nlhs,
                                        mxArray * plhs[],
                                        int nrhs,
                                        mxArray * prhs[]);
extern mxArray * mlfGeneral_private_ishghandle(mxArray * h);
extern void mlxGeneral_private_ishghandle(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern mxArray * mlfNHdf(int nargout, mlfVarargoutList * varargout, ...);
extern mxArray * mlfHdf(mlfVarargoutList * varargout, ...);
extern void mlfVHdf(mxArray * synthetic_varargin_argument, ...);
extern void mlxHdf(int nlhs, mxArray * plhs[], int nrhs, mxArray * prhs[]);
extern mxArray * mlfNIofun_private_wjpgc(int nargout,
                                         mlfVarargoutList * varargout,
                                         ...);
extern mxArray * mlfIofun_private_wjpgc(mlfVarargoutList * varargout, ...);
extern void mlfVIofun_private_wjpgc(mxArray * synthetic_varargin_argument, ...);
extern void mlxIofun_private_wjpgc(int nlhs,
                                   mxArray * plhs[],
                                   int nrhs,
                                   mxArray * prhs[]);
extern mxArray * mlfNIofun_private_pcxrle(int nargout,
                                          mlfVarargoutList * varargout,
                                          ...);
extern mxArray * mlfIofun_private_pcxrle(mlfVarargoutList * varargout, ...);
extern void mlfVIofun_private_pcxrle(mxArray * synthetic_varargin_argument,
                                     ...);
extern void mlxIofun_private_pcxrle(int nlhs,
                                    mxArray * plhs[],
                                    int nrhs,
                                    mxArray * prhs[]);
extern mxArray * mlfNIofun_private_png(int nargout,
                                       mlfVarargoutList * varargout,
                                       ...);
extern mxArray * mlfIofun_private_png(mlfVarargoutList * varargout, ...);
extern void mlfVIofun_private_png(mxArray * synthetic_varargin_argument, ...);
extern void mlxIofun_private_png(int nlhs,
                                 mxArray * plhs[],
                                 int nrhs,
                                 mxArray * prhs[]);
extern mxArray * mlfNIofun_private_wtifc(int nargout,
                                         mlfVarargoutList * varargout,
                                         ...);
extern mxArray * mlfIofun_private_wtifc(mlfVarargoutList * varargout, ...);
extern void mlfVIofun_private_wtifc(mxArray * synthetic_varargin_argument, ...);
extern void mlxIofun_private_wtifc(int nlhs,
                                   mxArray * plhs[],
                                   int nrhs,
                                   mxArray * prhs[]);
extern mxArray * mlfGraphics_private_disabledberror(void);
extern void mlxGraphics_private_disabledberror(int nlhs,
                                               mxArray * plhs[],
                                               int nrhs,
                                               mxArray * prhs[]);
extern void mlfGraphics_private_enabledberror(mxArray * hasDBError);
extern void mlxGraphics_private_enabledberror(int nlhs,
                                              mxArray * plhs[],
                                              int nrhs,
                                              mxArray * prhs[]);
extern mxArray * mlfGraphics_private_getget(mxArray * h, mxArray * propName);
extern void mlxGraphics_private_getget(int nlhs,
                                       mxArray * plhs[],
                                       int nrhs,
                                       mxArray * prhs[]);
extern mxArray * mlfGraphics_private_privdos(mxArray * * result, ...);
extern void mlxGraphics_private_privdos(int nlhs,
                                        mxArray * plhs[],
                                        int nrhs,
                                        mxArray * prhs[]);
extern mxArray * mlfGraphics_private_isfigure(mxArray * h);
extern void mlxGraphics_private_isfigure(int nlhs,
                                         mxArray * plhs[],
                                         int nrhs,
                                         mxArray * prhs[]);
extern mxArray * mlfGraphics_private_ishghandle(mxArray * h);
extern void mlxGraphics_private_ishghandle(int nlhs,
                                           mxArray * plhs[],
                                           int nrhs,
                                           mxArray * prhs[]);
extern mxArray * mlfGraphics_private_isslhandle(mxArray * h);
extern void mlxGraphics_private_isslhandle(int nlhs,
                                           mxArray * plhs[],
                                           int nrhs,
                                           mxArray * prhs[]);
extern void mlfGraphics_private_setset(mxArray * h,
                                       mxArray * propName,
                                       mxArray * propValue);
extern void mlxGraphics_private_setset(int nlhs,
                                       mxArray * plhs[],
                                       int nrhs,
                                       mxArray * prhs[]);
extern mxArray * mlfGraphics_private_preparehg(mxArray * pj_in, mxArray * h);
extern void mlxGraphics_private_preparehg(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern void mlfGraphics_private_catpreview(mxArray * pj,
                                           mxArray * epsFilename,
                                           mxArray * tifFilename,
                                           mxArray * preFilename);
extern void mlxGraphics_private_catpreview(int nlhs,
                                           mxArray * plhs[],
                                           int nrhs,
                                           mxArray * prhs[]);
extern mxArray * mlfGraphics_private_getpp(mxArray * target);
extern void mlxGraphics_private_getpp(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern mxArray * mlfGetprinttemplate(mxArray * h);
extern void mlxGetprinttemplate(int nlhs,
                                mxArray * plhs[],
                                int nrhs,
                                mxArray * prhs[]);
extern void mlfGraphics_private_setpp(mxArray * target, mxArray * properties);
extern void mlxGraphics_private_setpp(int nlhs,
                                      mxArray * plhs[],
                                      int nrhs,
                                      mxArray * prhs[]);
extern mxArray * mlfGraphics_private_restorehg(mxArray * pj_in, mxArray * h);
extern void mlxGraphics_private_restorehg(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern mxArray * mlfGraphics_private_blt(mxArray * pj, mxArray * h);
extern void mlxGraphics_private_blt(int nlhs,
                                    mxArray * plhs[],
                                    int nrhs,
                                    mxArray * prhs[]);
extern void mlfGraphics_private_noselection(mxArray * state, mxArray * fig);
extern void mlxGraphics_private_noselection(int nlhs,
                                            mxArray * plhs[],
                                            int nrhs,
                                            mxArray * prhs[]);
extern mxArray * mlfGraphics_private_prepareui(mxArray * pj_in, mxArray * Fig);
extern void mlxGraphics_private_prepareui(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern mxArray * mlfGraphics_private_ptpreparehg(mxArray * pt_in, mxArray * h);
extern void mlxGraphics_private_ptpreparehg(int nlhs,
                                            mxArray * plhs[],
                                            int nrhs,
                                            mxArray * prhs[]);
extern mxArray * mlfGraphics_private_screenpos(mxArray * window,
                                               mxArray * requestedPosition);
extern void mlxGraphics_private_screenpos(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern mxArray * mlfPrinttemplate(mxArray * synthetic_varargin_argument, ...);
extern void mlxPrinttemplate(int nlhs,
                             mxArray * plhs[],
                             int nrhs,
                             mxArray * prhs[]);
extern void mlfGraphics_private_ptrestorehg(mxArray * pt, mxArray * h);
extern void mlxGraphics_private_ptrestorehg(int nlhs,
                                            mxArray * plhs[],
                                            int nrhs,
                                            mxArray * prhs[]);
extern mxArray * mlfGraphics_private_restoreui(mxArray * pj_in, mxArray * Fig);
extern void mlxGraphics_private_restoreui(int nlhs,
                                          mxArray * plhs[],
                                          int nrhs,
                                          mxArray * prhs[]);
extern void libmwsglmInitialize(void);
extern void libmwsglmTerminate(void);

#ifdef __cplusplus
}
#endif

#endif
