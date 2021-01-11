/***************************************************************************
   NAME
     Particle.h
   PURPOSE
     定義了 class Particle
   NOTES
     1.CWinApp provides the application level functionalities
       (1)The CWinApp class has an important over-ridable function  InitInstance which handles the Window creations
       (2)The next important one is a data member, m_pMainWnd (of CWinApp) which holds the pointer to the window
   AUTHOR
     
   HISTORY
     Zhi-Chebg Yan - Match 3, 2010: Modified.
***************************************************************************/


#if !defined(AFX_Particle_H__082DB1E4_6069_11D1_83A0_004005308EB5__INCLUDED_)
#define AFX_Particle_H__082DB1E4_6069_11D1_83A0_004005308EB5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CParticleApp:
// See Particle.cpp for the implementation of this class
//

class CParticleApp : public CWinApp
{
public:
	CParticleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CParticleApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG

	/*This tells the application that 
	  the class in which this is called is going to have a message map and handle messages
	*/
	DECLARE_MESSAGE_MAP()//處理事件用 
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Particle_H__082DB1E4_6069_11D1_83A0_004005308EB5__INCLUDED_)
