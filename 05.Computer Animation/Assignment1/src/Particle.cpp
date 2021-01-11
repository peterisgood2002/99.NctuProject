#include "stdafx.h"
#include "Particle.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParticleApp

BEGIN_MESSAGE_MAP(CParticleApp, CWinApp)
	//{{AFX_MSG_MAP(CParticleApp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParticleApp construction

CParticleApp::CParticleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CParticleApp object

CParticleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CParticleApp initialization

BOOL CParticleApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	/*
	 Call this member function from within the InitInstance  member function 
	 to enable and load the list of most recently used (MRU) files and last preview state.
	 
	 LoadStdProfileSettings�N�O�Ψӳ]�w�t�ΰO���̪�}�L���ɮ׼�
	*/
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMainFrame* pFrame = new CMainFrame;//���ͤ@�ӷs��MainFrame

	/*
	 Call to dynamically create a frame window from resource information
	 ���{��Ū�J  IDR_MAINFRAME �o�@�ӱƪ��覡
	*/
	if (!pFrame->LoadFrame(IDR_MAINFRAME,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE)) {
		return FALSE;
	}

	m_pMainWnd = pFrame;//�NWindows���V�߲��ͪ�MainFrame

	// The one and only window has been initialized, so show and update it.
//	m_pMainWnd->ShowWindow(SW_SHOW);
//	m_pMainWnd->UpdateWindow();

	return TRUE;
}