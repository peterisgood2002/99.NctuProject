// MainWnd.h: interface for the CMainWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MAINWND_H_INCLUDED)
#define MAINWND_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Motion.h"
#include "Skeleton.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
// Encapsulates the main window
//
class CMainWnd  
{
private:
	float	m_fFovy;
	float	m_fAspect;
	float	m_fZNear;
	float	m_fZFar;

public:
	CMainWnd();
	virtual ~CMainWnd();
	
	HWND m_hWnd;
	HGLRC m_hRC;									// Handle to RC
	Motion* pMotion;
	Skeleton* pSkeleton;
	int nFrameNum;

	void light_init();
	void DrawScene();
	void KillScene();
	void InitScene();	
	void Tick(BOOL &bRedrawScene);
	void UpdateFPS();
	void LoadASF(string name);
	void LoadAMC(string name);
	void IK();
	void KeyMsg(WPARAM key);
	void SetPerspectiveParam (float fovy, float aspect, float znear, float zfar);

private:
	
};

#endif // !defined(MAINWND_H_INCLUDED)
