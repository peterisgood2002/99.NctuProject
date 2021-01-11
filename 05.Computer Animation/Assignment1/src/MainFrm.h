/***************************************************************************
   NAME
     MainFrm.h
   PURPOSE
     定義了 class CMainFrame
   NOTES
     CFrameWnd provides the functionalities related to GUI
   AUTHOR
     
   HISTORY
     Zhi-Chebg Yan - Match 3, 2010: Modified.

***************************************************************************/

#if !defined(AFX_MAINFRM_H__082DB1E8_6069_11D1_83A0_004005308EB5__INCLUDED_)
#define AFX_MAINFRM_H__082DB1E8_6069_11D1_83A0_004005308EB5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OGLView.h"
#include "Skeleton.h"

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CString m_ClassName;
	HCURSOR m_HArrow;//處理螢幕上的指標用
	COGLView  m_OGLView;
// Constructor
public:
	CMainFrame();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;//http://msdn.microsoft.com/en-us/library/fha3tfk7%28VS.80%29.aspx
	t_Bone		m_Skeleton;

	void InitializeSkeleton();

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewGeometry();
	afx_msg void OnUpdateViewGeometry(CCmdUI* pCmdUI);
	afx_msg void OnHelpWhichopengl();
	afx_msg void OnFileOpen();
	afx_msg void OnSimulationRunning();
	afx_msg void OnUpdateSimulationRunning(CCmdUI* pCmdUI);
	afx_msg void OnSimulationReset();
	afx_msg void OnSimulationSetsimproperties();
	afx_msg void OnSimulationUsegravity();
	afx_msg void OnUpdateSimulationUsegravity(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnViewShowgeometry();
	afx_msg void OnUpdateViewShowgeometry(CCmdUI* pCmdUI);
	afx_msg void OnViewShowsprings();
	afx_msg void OnUpdateViewShowsprings(CCmdUI* pCmdUI);
	afx_msg void OnViewShowvertices();
	afx_msg void OnUpdateViewShowvertices(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSimulationSetvertexmass();
	afx_msg void OnFileNewsystem();
	afx_msg void OnSimulationSettimingproperties();
	afx_msg void OnIntegratorEuler();
	afx_msg void OnUpdateIntegratorEuler(CCmdUI* pCmdUI);
	afx_msg void OnIntegratorMidpoint();
	afx_msg void OnUpdateIntegratorMidpoint(CCmdUI* pCmdUI);
	afx_msg void OnIntegratorRungekutta4();
	afx_msg void OnUpdateIntegratorRungekutta4(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()//處理事件用
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__082DB1E8_6069_11D1_83A0_004005308EB5__INCLUDED_)
