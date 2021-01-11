/***************************************************************************
   NAME
     Demo.h
   PURPOSE
     程式界面設計點
   NOTES

   AUTHOR
     
   HISTORY
     Zhi-Chebg Yan - Match 3, 2010: Modified.
***************************************************************************/

#if !defined(DEMO_H_INCLUDED)
#define DEMO_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "MainWnd.h"
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CApp
// Application class

class CApp
{
private:
	friend class CCtrlPanel;
protected:
	HINSTANCE m_hInst;
	CMainWnd* m_pMainWnd;
	HACCEL m_hAccelTable;

public:
	CApp();
	~CApp();

	int Run();
	void OnIdle();
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine);	
	void ExitInstance();
	BOOL OnCommand(int nCmdID, int nEvent);
	LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	void OnPaint(HDC hDC);
	void OnDestroy();
	void OnCreate();
	void OnSize(int cx, int cy);
};

#endif // !defined(DEMO_H_INCLUDED)
