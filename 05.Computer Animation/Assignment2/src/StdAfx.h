// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(STDAFX_H_INCLUDED)
#define STDAFX_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <gdiplus.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Local Header Files

// OpenGL support
#include <GL/gl.h>
#include <GL/glu.h>

// TODO: reference additional headers your program requires here

double TimeSpent(bool init);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(STDAFX_H_INCLUDED)
