// Demo.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Demo.h"
#include "FileOpenDlg.h"
#include <ZMouse.h>					// for MouseWheel message

/////////////////////////////////////////////////////////////////////////////
// Global CApp object

CApp gApp;
CFileOpenDlg	g_OpenDlg;//開啟檔案用
CFileOpenDlg	g_SaveDlg(false);//儲存檔案用 目前這好像沒用到廢物

#define WndHeight 600
#define WndWidth 800

#pragma comment (lib, "Gdiplus.lib")

/////////////////////////////////////////////////////////////////////////////
// Global functions
/////////////////////////////////////////////////////////////////////////////

//
// WinMain()
// The applications entry point.
// MFC程式進入點
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	/*初始化整個視窗*/
	if (!gApp.InitInstance(hInstance, nCmdShow, lpCmdLine)) 
		return FALSE;

	/*(3)The Message Loop*/
	int nRet = gApp.Run();

	gApp.ExitInstance();

	return nRet;
}

//
// WndProc()
// Window procedure for the main window. Simply calls CApp::WindowProc.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	return gApp.WindowProc(hWnd, message, wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// CApp construction/destruction

CApp::CApp():
	m_hAccelTable(0),
	m_hInst(0),
	m_pMainWnd(NULL)	
{
}

CApp::~CApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// CApp member functions
/////////////////////////////////////////////////////////////////////////////

//
// InitInstance()
// Called when the application starts. Creates and shows the main window.
// 初始化整個程式
//
BOOL CApp::InitInstance(HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine)
{
	/*(1)Registering the Window Class*/
	/*
	  stores the name of our window class, we will use it shortly to register our window class with the system. 
	*/
	const char szWindowClass[] = "DemoWindowClass";
	const char szWindowTitle[] = "Demo";

	m_hInst = hInstance;

	// Register main window class 設定相關資料
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);//The size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW;//Class Styles (CS_*), not to be confused with Window Styles (WS_*) This can usually be set to 0. 
	wcex.lpfnWndProc = (WNDPROC)WndProc;//Pointer to the window procedure for this window class
	wcex.cbClsExtra = 0;//Amount of extra data allocated for this class in memory. Usually 0. 
	wcex.cbWndExtra = 0;//Amount of extra data allocated in memory per window of this type. Usually 0. 
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_DEMO);//Large (usually 32x32) icon shown when the user presses Alt+Tab. 
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);//Cursor that will be displayed over our windo
	wcex.hbrBackground = 0;//Background Brush to set the color of our window
	wcex.lpszMenuName = (LPCSTR)IDC_DEMO;//ame of a menu resource to use for the windows with this class. 
	wcex.lpszClassName = szWindowClass;//Name to identify the class with.
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);//mall (usually 16x16) icon to show in the taskbar and in the top left corner of the window. 
	if(!RegisterClassEx(&wcex))
		return FALSE;

	/*(2)Creating the Window*/
	// Load accelerator table
	m_hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_DEMO);//讀入快捷鍵的table

	// Create main window
	m_pMainWnd = new CMainWnd;//產生一個Main Windwo
	m_pMainWnd->m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClass, szWindowTitle, 
		WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION |WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, WndWidth, WndHeight, NULL, NULL, hInstance, NULL);
	
	if (!m_pMainWnd->m_hWnd)
	{
		delete m_pMainWnd;
		return FALSE;
	}

	OnCreate();

	// Show the main window
	ShowWindow(m_pMainWnd->m_hWnd, nCmdShow);
	UpdateWindow(m_pMainWnd->m_hWnd);

	// begin for setting up the avi generator
	BITMAPINFOHEADER	bmih;
	bmih.biSize		= sizeof (BITMAPINFOHEADER);// BitmapInfoHeader的大小
	bmih.biPlanes		= 1;					// 位平面
	bmih.biBitCount		= 24;					// 比特格式(24 Bit, 3 Bytes)
	bmih.biWidth		= 800;					// 宽度(800 Pixels)
	bmih.biHeight		= 600;					// 高度 (600 Pixels)
	bmih.biCompression	= BI_RGB;				// 申请的模式 = RGB

	return TRUE;
}

//
// ExitInstance()
// Called when the application exits.
//
void CApp::ExitInstance()
{
}

//
// OnCommand()
// Handles WM_COMMAND messages
//
BOOL CApp::OnCommand(int nCmdID, int nEvent)
{
	HDC hDC = GetDC(m_pMainWnd->m_hWnd);
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
	switch (nCmdID)
	{				
		case ID_OPEN:
			g_OpenDlg.m_pFilter = _T("ASF (*.asf)\0*.asf\0\0");
			g_OpenDlg.DoModal();
			SetCurrentDirectory(g_OpenDlg.m_pPathName);
			m_pMainWnd->LoadASF(g_OpenDlg.m_pFileTitle);
			g_OpenDlg.m_pFilter = _T("AMC (*.amc)\0*.amc\0\0");
			g_OpenDlg.DoModal();
			m_pMainWnd->LoadAMC(g_OpenDlg.m_pFileTitle);
			break;
		case IDM_EXIT://離開
			SendMessage(m_pMainWnd->m_hWnd, WM_CLOSE, 0, 0);
			break;
		case ID_ADJUST:
			m_pMainWnd->IK();
			break;
		default:
			return FALSE;
	}
	wglMakeCurrent(0, 0);
	return TRUE;
}

//
// Run()
// Starts the message pump
/*(3)The Message Loop*/
//
int CApp::Run()
{  

    /*(3)The Message Loop*/
    MSG msg;
    while(TRUE)
	{
		/*The PeekMessage function dispatches incoming sent messages, 
		  checks the thread message queue for a posted message, and retrieves the message*/
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0))
				break;
			if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg)) 
			{		
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
			OnIdle();
	}

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return msg.wParam;
}

//
// WindowProc()
// Handles messages sent to the main window
/*(4)the Window Procedure*/
//http://www.toymaker.info/Games/html/wndproc.html
LRESULT CApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static bool Is_Ctrl_Down = false;

	hDC = BeginPaint(hWnd, &ps);
	switch (message) 
	{
		case WM_CLOSE://按下關閉時的動作
			DestroyWindow(m_pMainWnd->m_hWnd);
			m_pMainWnd->m_hWnd = 0;
			break;
		/*
		 The WM_COMMAND message is sent when the user selects a command item from a menu
		 
		*/
		case WM_COMMAND:
			if(!OnCommand(LOWORD(wParam), HIWORD(wParam)))
				return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		/*The WM_PAINT message is sent 
			when the system or another application makes a request to paint a portion of an application's window*/
		case WM_PAINT:
			OnPaint(hDC);
			break;
		case WM_CREATE:
			break;
		case WM_DESTROY:
			OnDestroy();
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			OnSize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_KEYDOWN:
			wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
			switch(wParam)
			{
			case VK_CONTROL:
				Is_Ctrl_Down = true;
				break;
			default:
				m_pMainWnd->KeyMsg(wParam);
				break;
			}
			break;
			wglMakeCurrent(0, 0);
		case WM_KEYUP:
			Is_Ctrl_Down = false;
			break;		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);//預設的Message該怎麼進行
   }
	EndPaint(hWnd, &ps);
   return 0;
}

//
// OnIdle()
// Called when there are no messages in the message queue. 
// Sets current OpenGL RC and notifies the main window class that another frame 
// is to be drawn. 
// 程式Idle時會做的函式
//
void CApp::OnIdle()
{
	BOOL bRedraw = TRUE;
	/*修改FPS*/
	m_pMainWnd->Tick(bRedraw);
	/*將東西畫上去*/
	if(bRedraw)
	{
		HDC hDC = GetDC(m_pMainWnd->m_hWnd);
		if(hDC)
		{
			wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
			m_pMainWnd->DrawScene(); 
			SwapBuffers(hDC);
			wglMakeCurrent(0, 0);
		}
		ReleaseDC(m_pMainWnd->m_hWnd, hDC);
	}
}

//
// OnCreate()
// Called when the main window has been created. Initializes OpenGL.
//
void CApp::OnCreate()
{
	// Define pixel format
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;	
	memset(&pfd, NULL, sizeof(pfd));    
	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	// Set pixel format
	HDC hDC = GetDC(m_pMainWnd->m_hWnd);
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);

	/*
	  用來指定目前的Render Context
	  The wglMakeCurrent function makes a specified OpenGL rendering context 
	          the calling thread's current rendering context. 
	  All subsequent OpenGL calls made by the thread are drawn on the device identified by hdc
	*/
	// Create RC  產生Render Context
	m_pMainWnd->m_hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);

	/*初始化場景*/
	m_pMainWnd->InitScene();
	wglMakeCurrent(0, 0);

	// Clean up
	ReleaseDC(m_pMainWnd->m_hWnd, hDC);

	// Size viewport
	RECT rc;
	GetClientRect(m_pMainWnd->m_hWnd, &rc);
	OnSize(rc.right-rc.left, rc.bottom-rc.top);
}

//
// OnDestroy()
// Called when the main window is destroyed. Shuts down OpenGL
//
void CApp::OnDestroy()
{
	HDC hDC = GetDC(m_pMainWnd->m_hWnd);
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
	m_pMainWnd->KillScene();
  wglMakeCurrent(0, 0);
	wglDeleteContext(m_pMainWnd->m_hRC);
	ReleaseDC(m_pMainWnd->m_hWnd, hDC);
}

//
// OnSize()
// Called when the main window is resized. Resizes the OpenGL
// viewport.
//
void CApp::OnSize(int cx, int cy)
{
	if(cx==0 || cy ==0 || m_pMainWnd->m_hWnd==NULL)
		return;

	GLfloat fFovy  = 45.0f; // Field-of-view
	GLfloat fZNear = 1.0f;  // Near clipping plane
	GLfloat fZFar = 10000.0f; // Far clipping plane

	HDC hDC = GetDC(m_pMainWnd->m_hWnd);
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);
	
	// Calculate viewport aspect
	RECT rv;
	GetClientRect(m_pMainWnd->m_hWnd, &rv);
	GLfloat fAspect = (GLfloat)(rv.right-rv.left) / (GLfloat)(rv.bottom-rv.top);
	m_pMainWnd->SetPerspectiveParam (fFovy, fAspect, fZNear, fZFar);

	// Define viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fFovy, fAspect, fZNear, fZFar);
	glViewport(rv.left, rv.top /*+ 50*/, rv.right-rv.left, rv.bottom-rv.top/*-50*/);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	
	wglMakeCurrent(0, 0);
	ReleaseDC(m_pMainWnd->m_hWnd, hDC);
}

//
// OnPaint()
// Handles WM_PAINT messages. Redraws the OpenGL scene.
//
void CApp::OnPaint(HDC hDC)
{	
	wglMakeCurrent(hDC, m_pMainWnd->m_hRC);//將畫筆設定給畫布
	m_pMainWnd->DrawScene();//畫圖
	SwapBuffers(hDC);
	wglMakeCurrent(0, 0);
}
