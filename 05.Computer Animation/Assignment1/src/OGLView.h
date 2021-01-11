
/***************************************************************************
   NAME
     OGLView.h
   PURPOSE
     定義了 class COGLView
   NOTES
    CWnd provides the base functionality of all window classes
     (1)A CWnd object is distinct from a Windows window
       *A CWnd object is created or destroyed by the CWnd constructor and destructor.
       *The Windows window is a data structure internal to Windows 
        that is created by a Create member function and destroyed by the CWnd virtual destructor. 
     (2)The CWnd class and the message-map mechanism hide the WndProc  function
       *Incoming Windows notification messages are automatically routed through the message map 
        to the proper OnMessage CWnd member functions.
	 (3)The CWnd class also lets you create a Windows child window for your application
       *call the constructor CWnd to construct the CWnd object
       *call the Create  member function to create the child window and attach it to the CWnd  object. 
     (4)further classes are derived from CWnd to provide specific window types. Many of these classes, 
        including CFrameWnd, CMDIFrameWnd, CMDIChildWnd, CView, and CDialog, are designed for further derivation. 
   AUTHOR
     
   HISTORY
     Zhi-Chebg Yan - Match 3, 2010: Modified.
***************************************************************************/

#if !defined(AFX_OGLVIEW_H__2AB46761_27CD_11D1_83A0_004005308EB5__INCLUDED_)
#define AFX_OGLVIEW_H__2AB46761_27CD_11D1_83A0_004005308EB5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OGLView.h : header file
//

#include <GL/gl.h>
#include <GL/glu.h>

#include "Skeleton.h"
#include "Phys.h"
/////////////////////////////////////////////////////////////////////////////
// COGLView window

class COGLView : public CWnd
{
// Construction
public:
	COGLView();

// Attributes
public:
	CStatusBar  *m_ptrStatusBar;//系統最下方的狀態列
	HDC		m_hDC;
	HGLRC	m_hRC;
	CPoint	m_mousepos;
	float	m_Base_Rot_X,m_Base_Rot_Y,m_Base_Rot_Z;//用來儲存原本的Rotation值
	float	m_Grab_Rot_X,m_Grab_Rot_Y,m_Grab_Rot_Z;
	float	m_Grab_Trans_X,m_Grab_Trans_Y,m_Grab_Trans_Z;
	BOOL	m_DrawGeometry,m_SimRunning;//?
	int		m_curVisual;
	float	m_MorphPos;
	DWORD	m_StartTime;
	DWORD	m_FrameCnt;
	int		m_TimeIterations;//表示要多少TimeIteration紀錄一次Frame
	BOOL	m_UseFixedTimeStep;
	float	m_MaxTimeStep;
	float	m_LastTime;//紀錄Simulation目前跑到哪裡了


	CPhys		m_PhysEnv;
// Operations
public:
	BOOL	SetupPixelFormat(HDC hdc);
	GLvoid	drawScene(GLvoid);
	GLvoid  drawModel(t_Bone *curBone);
	GLvoid	initializeGL(GLsizei width, GLsizei height);
	void	OnSimulationSetVertexMass();
	void	OnSimulationSetsimproperties();
	void	OnSetTimeProperties();
	GLvoid	resize( GLsizei width, GLsizei height );
	void	GetGLInfo();
	void	HandleKeyUp(UINT nChar);
	void	HandleKeyDown(UINT nChar);
	GLvoid  morphModel(t_Bone *curBone);
	GLvoid	LoadBoneTexture(t_Bone *curBone);
	void	NewSystem();
	void	LoadFile(CString file1,CString baseName,CString ext);
	void	SaveFile(CString file1,CString baseName);
	void	RunSim();
	void    DeclareSomeCommandList();
	float	GetTime( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COGLView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COGLView();

	// Generated message map functions
protected:
	t_Bone	m_Skeleton,*m_CurBone;
	int		m_PickX, m_PickY;//滑鼠在螢幕上的位置
	int		m_ScreenWidth;
	int		m_ScreenHeight;
	//{{AFX_MSG(COGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGLVIEW_H__2AB46761_27CD_11D1_83A0_004005308EB5__INCLUDED_)
