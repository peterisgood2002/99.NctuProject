#include "stdafx.h"
#include "Particle.h"
#include "MainFrm.h"
#include <iostream>
#include <exception>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
/*
 This macro takes two parameters. 
 1.The class name which implements the message map ��@��message map���禡
 2.the base class for it. �����O
   (1)It'll succeeded by the Macro which represent message
 3.It is then closed by END_MESSAGE_MAP(3rd Macro).

*/
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	//It'll succeeded by the Macro which represent message It'll have the relative function to implement
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_VIEW_GEOMETRY, OnViewGeometry)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GEOMETRY, OnUpdateViewGeometry)
	ON_COMMAND(ID_HELP_WHICHOPENGL, OnHelpWhichopengl)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)//�NID_FILE_OPEN�MOnFileOpen�����p
	ON_COMMAND(ID_SIMULATION_RUNNING, OnSimulationRunning)//Running�Q���U��
	ON_UPDATE_COMMAND_UI(ID_SIMULATION_RUNNING, OnUpdateSimulationRunning)
	ON_COMMAND(ID_SIMULATION_RESET, OnSimulationReset)//Reset�Q���U��
	ON_COMMAND(ID_SIMULATION_SETSIMPROPERTIES, OnSimulationSetsimproperties)//set Simulation property�Q���U��
	ON_COMMAND(ID_SIMULATION_USEGRAVITY, OnSimulationUsegravity)
	ON_UPDATE_COMMAND_UI(ID_SIMULATION_USEGRAVITY, OnUpdateSimulationUsegravity)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)//�s��
	ON_COMMAND(ID_VIEW_SHOWGEOMETRY, OnViewShowgeometry)//Geometry�Q���U��
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWGEOMETRY, OnUpdateViewShowgeometry)
	ON_COMMAND(ID_VIEW_SHOWSPRINGS, OnViewShowsprings)//�Ψӥ��}��Spring�Ϊ�
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWSPRINGS, OnUpdateViewShowsprings)
	ON_COMMAND(ID_VIEW_SHOWVERTICES, OnViewShowvertices)//�Ψ�����I��
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWVERTICES, OnUpdateViewShowvertices)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SIMULATION_SETVERTEXMASS, OnSimulationSetvertexmass)//Set Vertex Mass�Q���U��
	ON_COMMAND(ID_FILE_NEWSYSTEM, OnFileNewsystem)//Clear System�Q���U��
	ON_COMMAND(ID_SIMULATION_SETTIMINGPROPERTIES, OnSimulationSettimingproperties)//Set Timing Properties�Q���U��
	ON_COMMAND(ID_INTEGRATOR_EULER, OnIntegratorEuler)//Euler�Q���U��
	ON_UPDATE_COMMAND_UI(ID_INTEGRATOR_EULER, OnUpdateIntegratorEuler)
	ON_COMMAND(ID_INTEGRATOR_MIDPOINT, OnIntegratorMidpoint)//Midpoint �Q���U��
	ON_UPDATE_COMMAND_UI(ID_INTEGRATOR_MIDPOINT, OnUpdateIntegratorMidpoint)
	ON_COMMAND(ID_INTEGRATOR_RUNGEKUTTA4, OnIntegratorRungekutta4)//Runge-Kutta4�Q���U��
	ON_UPDATE_COMMAND_UI(ID_INTEGRATOR_RUNGEKUTTA4, OnUpdateIntegratorRungekutta4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*
  �ΨӰ���ӵe�������A�C�����q�Ϊ� �Ш���w�q�i�H�ݨ��������
*/
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	/*
	 CWinApp* AFXAPI AfxGetApp( );
	      The pointer returned by this function can be used to access application information 
		  such as the main message-dispatch code or the topmost window.
     ============================================

	 HCURSOR LoadStandardCursor(
          LPCTSTR lpszCursorName 
     ) const;
	      Loads the Windows predefined cursor resource that lpszCursorName specifies.
		  An IDC_ manifest constant identifier that specifies a predefined Windows cursor
	*/
	m_HArrow = AfxGetApp()->LoadStandardCursor(IDC_WAIT);//�Ψӱ���cusor�� �гz�L�NOpenGL�����������覡�ݨ��ĪG
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
/// Local Variables ///////////////////////////////////////////////////////////
	RECT rect;
///////////////////////////////////////////////////////////////////////////////
	/*http://msdn.microsoft.com/en-us/library/ms633503%28VS.85%29.aspx*/
	GetClientRect(&rect); 

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)//�����ͤ����O
		return -1;

	/*�U�����@�����A�C*/
	if (!m_wndStatusBar.Create(this ) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_OGLView.m_ptrStatusBar = &m_wndStatusBar;
	/*
	  http://msdn.microsoft.com/en-us/library/0yhc9kx4%28VS.80%29.aspx
	  CRect(1, 1,rect.right - 3,rect.bottom)�GThe size and position of the window, in client coordinates of pParentWnd.
	  http://msdn.microsoft.com/zh-tw/library/9y3502k0%28VS.80%29.aspx
	*/
	m_OGLView.Create(NULL,"Render Window",WS_CHILD | WS_VISIBLE, CRect(1, 1,rect.right - 3,rect.bottom),this,104); // - 60 bottom
	m_OGLView.ShowWindow(true);

	m_OGLView.Invalidate(TRUE);

	return 0;
}

/*
  Called by the framework before the creation of the Windows window 
  The CREATESTRUCT structure defines the initialization parameters passed to the window procedure of an application.
*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
/// Local Variables ///////////////////////////////////////////////////////////
	HICON hicon;
///////////////////////////////////////////////////////////////////////////////
	/*
	  Loads the icon resource named by lpszResourceName  or specified by nIDResource from the executable file.
	  HICON LoadIcon(
			LPCTSTR lpszResourceName 
	  ) const;
	  HICON LoadIcon(
			UINT nIDResource 
	  ) const;

	*/
	hicon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/*Allows you to register your own window classes.*/
	m_ClassName = AfxRegisterWndClass(NULL,
		(HCURSOR)m_HArrow, (HBRUSH)::GetStockObject(DKGRAY_BRUSH), hicon); //m_HArrow
	cs.lpszClass = m_ClassName;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

///////////////////////////////////////////////////////////////////////////////
// Procedure:	OnHelpWhichopengl
// Purpose:		Create dialog to Show which version of OGL is running
// Notes:		Pretty Handy info for debugging
///////////////////////////////////////////////////////////////////////////////		
void CMainFrame::OnHelpWhichopengl() 
{
	m_OGLView.GetGLInfo();
}
// OnWhichogl

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);//�I�s�����O���ʧ@
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
/*http://msdn.microsoft.com/en-us/library/01c9aaty%28VS.80%29.aspx*/
void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting  ���F��|�v�T��Dialog Box����X

	m_OGLView.drawScene();//�z�LOpenGL�u��e�e
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	m_OGLView.SetWindowPos( &wndTopMost, 1, 1, cx - 3, cy - 21 , SWP_NOZORDER ); // -60 bottom

	CFrameWnd::OnSize(nType, cx, cy);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_OGLView.HandleKeyDown(nChar);
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
/// Local Variables ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	switch (nChar)
	{
		case ' ':
			break;
	}
	m_OGLView.HandleKeyUp(nChar);
//	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// Hierarchy Manipulation Functions

void CMainFrame::InitializeSkeleton()
{
}


// TOGGLE THE STATUS OF THE VIEW GEOMETRY FLAG
void CMainFrame::OnViewGeometry() 
{
	m_OGLView.m_DrawGeometry = !m_OGLView.m_DrawGeometry;
	m_OGLView.drawScene();
}

// SET THE CHECKED STATUS OF THE VIEW GEOMETRY MENU BASED ON STATUS
void CMainFrame::OnUpdateViewGeometry(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_DrawGeometry );
}

/*Clear System�Q���U��Ĳ�o*/
void CMainFrame::OnFileNewsystem() 
{
	m_OGLView.NewSystem();
}
/*Open�Q���U��Ĳ�o*/
void CMainFrame::OnFileOpen() 
{
	char szFilter[] = "DPS files (*.dps)|*.dps|OBJ files (*.obj)|*.obj||";  // WILL INCLUDE Biovision Hierarchy BVH (*.bvh)|*.bvh|
	/*http://msdn.microsoft.com/en-us/library/wh5hz49d%28VS.80%29.aspx*/
	CFileDialog	dialog( TRUE, ".obj", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString name;
	/*http://msdn.microsoft.com/en-us/library/3xb6dhth%28VS.80%29.aspx*/
	if (dialog.DoModal())
	{
		//CString totalfn = dialog.GetFolderPath() +"\\" + dialog.GetFileName();
		m_OGLView.LoadFile( dialog.GetFileName() ,dialog.GetFileTitle( ),dialog.GetFileExt()  );//Ū�ɶi�JOGLView��
		m_OGLView.Invalidate(TRUE);
	}
}
/*Save�Q���U��Ĳ�o*/
void CMainFrame::OnFileSave() 
{
	char szFilter[] = "DPS files (*.dps)|*.dps||";  // WILL INCLUDE Biovision Hierarchy BVH (*.bvh)|*.bvh|
	CFileDialog	dialog( FALSE, ".dps", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString name;		
	if (dialog.DoModal())
	{
		//CString totalfn = dialog.GetFolderPath() +"\\" + dialog.GetFileName();
		m_OGLView.SaveFile(dialog.GetFileName(),dialog.GetFileTitle( ));
		m_OGLView.Invalidate(TRUE);
	}
}
/*Running�Q���U��Ĳ�o*/
void CMainFrame::OnSimulationRunning() 
{
	m_OGLView.HandleKeyUp('R');  // FORCE SYSTEM TO START RUNNING THROUGH KEYPRESS ��@�OOpenGL��������R�Q���U�h�F
}

void CMainFrame::OnUpdateSimulationRunning(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_SimRunning );
}

void CMainFrame::OnSimulationReset() 
{
	m_OGLView.m_PhysEnv.ResetWorld();
	m_OGLView.Invalidate(TRUE);
}
/*set Simulation property�Q���U��*/
void CMainFrame::OnSimulationSetsimproperties() 
{
	m_OGLView.OnSimulationSetsimproperties();
}
/*Set Timing Properties�Q���U��*/
void CMainFrame::OnSimulationSettimingproperties() 
{
	m_OGLView.OnSetTimeProperties();	
}
/*Set Vertex Mass�Q���U��*/
void CMainFrame::OnSimulationSetvertexmass() 
{
	m_OGLView.OnSimulationSetVertexMass();
}

void CMainFrame::OnSimulationUsegravity() 
{
	m_OGLView.m_PhysEnv.UseGravity = !m_OGLView.m_PhysEnv.UseGravity;
	m_OGLView.Invalidate(TRUE);
}

void CMainFrame::OnUpdateSimulationUsegravity(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_PhysEnv.UseGravity );
}
/*Show Geometry�Q���U��Ĳ�o*/
void CMainFrame::OnViewShowgeometry() 
{
	m_OGLView.m_DrawGeometry = !m_OGLView.m_DrawGeometry;
	m_OGLView.Invalidate(TRUE);
}

void CMainFrame::OnUpdateViewShowgeometry(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_DrawGeometry );
}
/*Show Spring�Q���U��Ĳ�o*/
void CMainFrame::OnViewShowsprings() 
{
	m_OGLView.m_PhysEnv.DrawSprings = !m_OGLView.m_PhysEnv.DrawSprings;
	m_OGLView.Invalidate(TRUE);
}

void CMainFrame::OnUpdateViewShowsprings(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_PhysEnv.DrawSprings );
}
/*Show Vertex�Q���U��Ĳ�o*/
void CMainFrame::OnViewShowvertices() 
{
	m_OGLView.m_PhysEnv.DrawVertices = !m_OGLView.m_PhysEnv.DrawVertices;
	m_OGLView.Invalidate(TRUE);
}

void CMainFrame::OnUpdateViewShowvertices(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_PhysEnv.DrawVertices );
}
/*����Frame�ɩI�s*/
void CMainFrame::OnClose() 
{
	m_OGLView.m_SimRunning = FALSE;
	
	CFrameWnd::OnClose();
}

BOOL CMainFrame::DestroyWindow() 
{
	
	return CFrameWnd::DestroyWindow();
}
/*Euler�Q���U��*/
void CMainFrame::OnIntegratorEuler() 
{
	m_OGLView.m_PhysEnv.IntegratorType = EULER_INTEGRATOR;
	m_OGLView.Invalidate(TRUE);
}

void CMainFrame::OnUpdateIntegratorEuler(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_PhysEnv.IntegratorType == EULER_INTEGRATOR );
}
/*Midpoint �Q���U��*/
void CMainFrame::OnIntegratorMidpoint() 
{
	m_OGLView.m_PhysEnv.IntegratorType = MIDPOINT_INTEGRATOR;
	m_OGLView.Invalidate(TRUE);
}

void CMainFrame::OnUpdateIntegratorMidpoint(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_PhysEnv.IntegratorType == MIDPOINT_INTEGRATOR );
}
/*Runge-Kutta4�Q���U��*/
void CMainFrame::OnIntegratorRungekutta4() 
{
	m_OGLView.m_PhysEnv.IntegratorType = RK4_INTEGRATOR;
	m_OGLView.Invalidate(TRUE);
}

void CMainFrame::OnUpdateIntegratorRungekutta4(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_PhysEnv.IntegratorType == RK4_INTEGRATOR );
}

