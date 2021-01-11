#include "stdafx.h"
#include <mmsystem.h>
#include "Particle.h"
#include "OGLView.h"
#include "LoadOBJ.h"
#include "TimeProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// Application Definitions ///////////////////////////////////////////////////
#define OGL_AXIS_DLIST		1		// OPENGL DISPLAY LIST ID
#define ROTATE_SPEED		1.0		// SPEED OF ROTATION

#define LERP(a,b,c)  (a + ((b - a) * c))
///////////////////////////////////////////////////////////////////////////////

/// Global Variables //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// COGLView

/*建構子 將OpenGL相關東西建構起來*/
COGLView::COGLView()
{
	// INITIALIZE THE MODE KEYS
	m_DrawGeometry = TRUE;
	m_SimRunning = FALSE;
	m_CurBone = NULL;
	ResetBone(&m_Skeleton, NULL);
	m_Skeleton.id = -1;
	strcpy(m_Skeleton.name,"Skeleton");
	m_Skeleton.b_trans.z = -100.0f;
	m_Skeleton.trans.z = -100.0f;

	m_FrameCnt = 0;

	m_TimeIterations = 10;
	m_UseFixedTimeStep = FALSE;//TimeStep大小會變
	m_MaxTimeStep = 0.01f;//TimeStep大小為0.01

	m_hDC = NULL;

	m_PickX = -1;
	m_PickY = -1;
}

COGLView::~COGLView()
{
	DestroySkeleton(&m_Skeleton);
}


BOOL COGLView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
/// Local Variables ///////////////////////////////////////////////////////////
	//t_Visual	*visual = NULL;
///////////////////////////////////////////////////////////////////////////////
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/*Message Map所在之處*/
BEGIN_MESSAGE_MAP(COGLView, CWnd)
	//{{AFX_MSG_MAP(COGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

float COGLView::GetTime( void )
{
    static DWORD StartMilliseconds;
    if(!StartMilliseconds)
    {
        // yes, the first time through will be a 0 timestep
		/*http://msdn.microsoft.com/en-us/library/dd757629%28VS.85%29.aspx*/
        StartMilliseconds = timeGetTime();
    }

    DWORD CurrentMilliseconds = timeGetTime();
    return float(CurrentMilliseconds - StartMilliseconds) / 1000.0f;
}


/////////////////////////////////////////////////////////////////////////////
// COGLView message handlers
/*定義Screen Pixel的格式*/
BOOL COGLView::SetupPixelFormat(HDC hdc)
{
/// Local Variables ///////////////////////////////////////////////////////////
	/*http://msdn.microsoft.com/en-us/library/dd368826%28VS.85%29.aspx*/
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;
///////////////////////////////////////////////////////////////////////////////
    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;//Specifies the version of this data structure. This value should be set to 1
    ppfd->dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;//specify properties of the pixel buffer
    //ppfd->dwLayerMask = PFD_MAIN_PLANE;//Ignored. Earlier implementations of OpenGL used this member, but it is no longer used.
    ppfd->iPixelType = PFD_TYPE_RGBA;//Specifies the type of pixel data
    ppfd->cColorBits = 16;//除了Alpha之Color buffer大小
    ppfd->cDepthBits = 16;//Specifies the depth of the depth (z-axis) buffer.
    ppfd->cAccumBits = 0;//Specifies the total number of bitplanes in the accumulation buffer.
    ppfd->cStencilBits = 0;//Specifies the depth of the stencil buffer.

	/*
	  http://msdn.microsoft.com/en-us/library/dd318284%28VS.85%29.aspx
	  找出接近之Pixel format descriptor
	  This function compares 
	  the generic pixel formats supported by Windows NT and 
	  any device pixel formats supported by special hardware accelerators with the pixel format you described, 
	  
	  and returns the best match
	*/
    pixelformat = ChoosePixelFormat(hdc, ppfd);

    if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0) {//如果assign失敗則發生錯誤訊息
        MessageBox("ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (pfd.dwFlags & PFD_NEED_PALETTE) {//表示Hardware是用8 bit所以需要有palette(調色板)
        MessageBox("Needs palette", "Error", MB_OK);
        return FALSE;
    }

	/*
	  http://msdn.microsoft.com/en-us/library/dd369049%28VS.85%29.aspx
	  設定Device之Pixel format
	*/
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) {
        MessageBox("SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

/*
  在建立GLView時會呼叫的函式區
  此東西會將依些OpenGL的相關設定都設定好
  例如Render Context 以及Perspective view等等
*/
int COGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
/// Local Variables ///////////////////////////////////////////////////////////
	RECT rect;
///////////////////////////////////////////////////////////////////////////////
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*http://msdn.microsoft.com/en-us/library/aa921543.aspx
	  取出windows client area之Display Device Context
	*/
    m_hDC = ::GetDC(m_hWnd);
    if (!SetupPixelFormat(m_hDC))//讓裝置設定為想要的Pixel Format
		PostQuitMessage (0);
	/*
	  To create a rendering context, you use the wglCreateContext function
	  wglCreateContext returns an HGLRC, which is a handle to the rendering context
	*/
    m_hRC = wglCreateContext(m_hDC);
	/*
	  用來指定目前的Render Context
	  The wglMakeCurrent function makes a specified OpenGL rendering context 
	          the calling thread's current rendering context. 
	  All subsequent OpenGL calls made by the thread are drawn on the device identified by hdc
	*/
    wglMakeCurrent(m_hDC, m_hRC);
    GetClientRect(&rect);
    initializeGL(rect.right, rect.bottom);//初始化OpenGL

	// CREATE THE DISPLAY LIST FOR AN AXIS WITH ARROWS POINTING IN
	// THE POSITIVE DIRECTION Red = X, Green = Y, Blue = Z
	DeclareSomeCommandList();

	glDisable(GL_TEXTURE_2D);

	drawScene();
	return 0;
}
/*
  Display lists are groups of GL commands that have been stored for subsequent execution
  可以用glCallList來呼叫並執行
  這邊只是宣告而已
*/
void 
COGLView::DeclareSomeCommandList() {
	glNewList(OGL_AXIS_DLIST,GL_COMPILE_AND_EXECUTE);
		glPushMatrix();
		glScalef(4.0,4.0,4.0);
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);	// X AXIS STARTS - COLOR RED
			glVertex3f(-0.2f,  0.0f, 0.0f);
			glVertex3f( 0.2f,  0.0f, 0.0f);
			glVertex3f( 0.2f,  0.0f, 0.0f);	// TOP PIECE OF ARROWHEAD
			glVertex3f( 0.15f,  0.04f, 0.0f);
			glVertex3f( 0.2f,  0.0f, 0.0f);	// BOTTOM PIECE OF ARROWHEAD
			glVertex3f( 0.15f, -0.04f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);	// Y AXIS STARTS - COLOR GREEN
			glVertex3f( 0.0f,  0.2f, 0.0f);
			glVertex3f( 0.0f, -0.2f, 0.0f);			
			glVertex3f( 0.0f,  0.2f, 0.0f);	// TOP PIECE OF ARROWHEAD
			glVertex3f( 0.04f,  0.15f, 0.0f);
			glVertex3f( 0.0f,  0.2f, 0.0f);	// BOTTOM PIECE OF ARROWHEAD
			glVertex3f( -0.04f,  0.15f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);	// Z AXIS STARTS - COLOR BLUE
			glVertex3f( 0.0f,  0.0f,  0.2f);
			glVertex3f( 0.0f,  0.0f, -0.2f);
			glVertex3f( 0.0f,  0.0f, 0.2f);	// TOP PIECE OF ARROWHEAD
			glVertex3f( 0.0f,  0.04f, 0.15f);
			glVertex3f( 0.0f, 0.0f, 0.2f);	// BOTTOM PIECE OF ARROWHEAD
			glVertex3f( 0.0f, -0.04f, 0.15f);
		glEnd();
		glPopMatrix();
	glEndList();
}
/* OpenGL code */
GLvoid COGLView::resize( GLsizei width, GLsizei height )
{
// Local Variables ///////////////////////////////////////////////////////////
    GLfloat aspect;
///////////////////////////////////////////////////////////////////////////////

    glViewport(0, 0, width, height);

    aspect = (GLfloat)width/(GLfloat)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(10.0, aspect, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}    
/*初始化OpenGL 主要是設定perspective 和lighting*/
GLvoid COGLView::initializeGL(GLsizei width, GLsizei height)
{
/// Local Variables ///////////////////////////////////////////////////////////
    GLfloat aspect;
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightpos[] = { 0.30f, 0.3f, 1.0f, 0.0f };		// .5 .5 1.0
	GLfloat ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
///////////////////////////////////////////////////////////////////////////////

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
//    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    aspect = (GLfloat)width/(GLfloat)height;
	// Establish viewing volume
	gluPerspective(10.0, aspect,1, 2000);
    glMatrixMode(GL_MODELVIEW);

	// SET SOME OGL INITIAL STATES SO THEY ARE NOT DONE IN THE DRAW LOOP
	glPolygonMode(GL_FRONT,GL_FILL);
//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineWidth(2.0f);
	glPointSize(8.0f);
	glDisable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

//	glShadeModel(GL_SMOOTH);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
//	glMaterialfv(GL_FRONT,GL_AMBIENT, ambient);
//	glMaterialfv(GL_FRONT,GL_DIFFUSE, diffuse);
//	glMaterialfv(GL_FRONT,GL_SPECULAR, specular);
//	glMaterialf(GL_FRONT,GL_SHININESS, 100.0f);		// 12
//	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glDisable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);

}

/* 
  GET THE INFO ON THE VERSION OF OPENGL RUNNING
  產生一個message box並且輸出GL相關資訊

*/
void COGLView::GetGLInfo() 
{
//// Local Variables ////////////////////////////////////////////////////////////////
	char *who, *which, *ver, *ext, *message;
	int len;
/////////////////////////////////////////////////////////////////////////////////////
	who = (char *)::glGetString( GL_VENDOR );
	which = (char *)::glGetString( GL_RENDERER );
	ver = (char *)::glGetString( GL_VERSION );
	ext = (char *)::glGetString( GL_EXTENSIONS );

	len = 200 + strlen(who) + strlen(which) + strlen(ver) + strlen(ext);

	message = (char *)malloc(len);
	sprintf(message,"Who:\t%s\nWhich:\t%s\nVersion:\t%s\nExtensions:\t%s",
		who, which, ver, ext);

	::MessageBox(NULL,message,"GL Info",MB_OK);

	free(message);
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	RunSim
// Purpose:		Actual simulation loop
// Notes:		Allows you to adjust the rate of simulation or to change it
//				to fixed time steps or actual timesteps.
///////////////////////////////////////////////////////////////////////////////		
void COGLView::RunSim()
{
/// Local Variables ///////////////////////////////////////////////////////////
	float Time;
	float DeltaTime;
///////////////////////////////////////////////////////////////////////////////

    if (m_UseFixedTimeStep)
		Time = m_LastTime + (m_MaxTimeStep * m_TimeIterations);
	else
		Time = GetTime() * m_TimeIterations;

	if (m_SimRunning)
	{
		while(m_LastTime < Time)
		{
			DeltaTime = Time - m_LastTime;//時間間隔
			if(DeltaTime > m_MaxTimeStep)
			{
				DeltaTime = m_MaxTimeStep;
			}

	 		m_PhysEnv.Simulate(DeltaTime,m_SimRunning);
			m_LastTime += DeltaTime;
		}
		m_LastTime = Time;
	}
	/*else
	{
		m_PhysEnv.Simulate(DeltaTime,m_SimRunning);
	}*/
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Procedure:	drawModel
// Purpose:		Draws the model associated with a bone
// Notes:		Currently uses a global model not associated with the bone
//              The data uses Quads with shared vertices and vertex coloring 
//				so I chose to use indexed vertex arrays
///////////////////////////////////////////////////////////////////////////////		
GLvoid COGLView::drawModel(t_Bone *curBone)
{
	if (curBone->visualCnt > 0 && curBone->visuals[0].vertexData != NULL)
	{
		glColor3f(1.0f,1.0f,1.0f);	
		// Declare the Array of Data
		glInterleavedArrays(curBone->visuals[0].dataFormat,0,(GLvoid *)curBone->visuals[0].vertexData);
		if (curBone->visuals[0].reuseVertices)
		{
			// HANDLE EITHER QUADS OR TRIS
			if (curBone->visuals[0].vPerFace == 3)
				glDrawElements(GL_TRIANGLES,curBone->visuals[0].faceCnt * 3,GL_UNSIGNED_SHORT,curBone->visuals[0].faceIndex);
			else
				glDrawElements(GL_QUADS,curBone->visuals[0].faceCnt * 4,GL_UNSIGNED_SHORT,curBone->visuals[0].faceIndex);
		}
		else
		{
			// HANDLE EITHER QUADS OR TRIS
			if (curBone->visuals[0].vPerFace == 3)
				glDrawArrays(GL_TRIANGLES,0,curBone->visuals[0].faceCnt * 3);
			else
				glDrawArrays(GL_QUADS,0,curBone->visuals[0].faceCnt * 4);
		}
	}
}
// drawModel


///////////////////////////////////////////////////////////////////////////////
// Procedure:	drawScene
// Purpose:		Draws the current OpenGL scene
///////////////////////////////////////////////////////////////////////////////		
GLvoid COGLView::drawScene(GLvoid)
{
/// Local Variables ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	/*旋轉的邊界測試*/
	if (m_Skeleton.rot.y  > 360.0f) m_Skeleton.rot.y  -= 360.0f;
    if (m_Skeleton.rot.x   > 360.0f) m_Skeleton.rot.x   -= 360.0f;
    if (m_Skeleton.rot.z > 360.0f) m_Skeleton.rot.z -= 360.0f;

  //  glDisable(GL_DEPTH_TEST);	// TURN OFF DEPTH TEST FOR CLEAR

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glEnable(GL_DEPTH_TEST);	// ENABLE DEPTH TESTING

    glPushMatrix();

    // Set root skeleton's orientation and position
    glTranslatef(m_Skeleton.trans.x, m_Skeleton.trans.y, m_Skeleton.trans.z);//平移

	// ROTATE THE ROOT
	glRotatef(m_Skeleton.rot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(m_Skeleton.rot.y, 0.0f, 1.0f, 0.0f);
 	glRotatef(m_Skeleton.rot.z, 0.0f, 0.0f, 1.0f); 

	// GRAB THE MATRIX AT THIS POINT SO I CAN USE IT FOR THE DEFORMATION
	glGetFloatv(GL_MODELVIEW_MATRIX,m_Skeleton.matrix.m);//取得ModelView的Matrix m_Skeleton.matrix.m = MODELVIEW_MATRIX

	if (m_PickX > -1)//表示有點到東西
		m_PhysEnv.GetNearestP(m_PickX,m_PickY);

	RunSim();//跑模擬
	
	//劃出三軸
	glCallList( OGL_AXIS_DLIST );
	m_PhysEnv.RenderWorld();		// DRAW THE SIMULATION

	glPopMatrix();
    glFinish();

	if (m_hDC)
		SwapBuffers(m_hDC);

// PLAYING WITH CHECKING FRAMETIMING
/*	if (m_SimRunning)
	{
		m_FrameCnt++;
	    char message[80];
		DWORD end = timeGetTime();
		float diff = (float)(m_FrameCnt * 1000)/(float)(end - m_StartTime);

		sprintf(message,"%.2f",diff);
		m_ptrStatusBar->SetPaneText(0,message);
	}*/
	m_PickX = -1;
	m_PickY = -1;
}
// 	drawScene

void COGLView::OnDestroy() 
{
	CWnd::OnDestroy();
	if (m_hRC)
		wglDeleteContext(m_hRC);
    if (m_hDC)
		::ReleaseDC(m_hWnd,m_hDC);
    m_hRC = 0;
    m_hDC = 0;
}

void COGLView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	drawScene();
	// Do not call CWnd::OnPaint() for painting messages
}

void COGLView::OnSize(UINT nType, int cx, int cy) 
{
	// RESIZE THE OPENGL WINDOW
	m_ScreenWidth = cx;
	m_ScreenHeight = cy;
	resize( cx,cy );
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	OnLButtonDown
// Purpose:		Left button down grabs the current point pos so I can use it
///////////////////////////////////////////////////////////////////////////////		
void COGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_mousepos = point;
	m_Base_Rot_X = 	m_Skeleton.rot.x;
	m_Base_Rot_Y = 	m_Skeleton.rot.y;
	m_Base_Rot_Z = 	m_Skeleton.rot.z;
	if ((nFlags & MK_SHIFT) == 0)
	{
		m_PickX = point.x;//滑鼠的X軸
		m_PickY = m_ScreenHeight - point.y;
		drawScene();
	}
	SetCapture( );
	CWnd::OnLButtonDown(nFlags, point);
}

void COGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_PhysEnv.MouseForceActive = FALSE;		// STOP APPLYING MOUSE FORCE
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	OnRButtonDown
// Purpose:		Right button down grabs the current point pos so I can use it
///////////////////////////////////////////////////////////////////////////////		
void COGLView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_mousepos = point;
	m_Base_Rot_X = 	m_Skeleton.rot.x;
	m_Base_Rot_Y = 	m_Skeleton.rot.y;
	m_Base_Rot_Z = 	m_Skeleton.rot.z;
	CWnd::OnRButtonDown(nFlags, point);
}

void COGLView::HandleKeyDown(UINT nChar) 
{
}
/*處理Key被按下去的時候該做甚麼動作*/
void COGLView::HandleKeyUp(UINT nChar) 
{
	tVector userforce;
	switch (nChar)
	{
	case VK_RETURN://Enter鍵被壓入
		m_PhysEnv.AddSpring();
		break;
	case 'G':
		m_PhysEnv.UseGravity = !m_PhysEnv.UseGravity;
		break;
	case '1': m_curVisual = 0;
		break;
	case '2': m_curVisual = 1;
		break;
	case 'O': 
		glPolygonMode(GL_FRONT,GL_LINE);
//		glPolygonMode(GL_FRONT,GL_FILL);
		break;
	case 'R':
		m_SimRunning = !m_SimRunning;
		if (m_SimRunning)
			m_LastTime = GetTime() * m_TimeIterations;	// RESET THE SIM  紀錄Simulation目前跑到哪裡了
		m_StartTime = timeGetTime();//設定目前起始時間 millisecond
		m_FrameCnt = 0;//目前還沒有任何Frame
		break;
	case 'T':
		m_PhysEnv.ResetWorld();
		break;
	case VK_HOME:
		userforce.x = m_Skeleton.matrix.m[1];
		userforce.y = m_Skeleton.matrix.m[5];
		userforce.z = m_Skeleton.matrix.m[9];
		m_PhysEnv.ApplyUserForce(&userforce);
		break;
	case VK_END:
		userforce.x = -m_Skeleton.matrix.m[1];
		userforce.y = -m_Skeleton.matrix.m[5];
		userforce.z = -m_Skeleton.matrix.m[9];
		m_PhysEnv.ApplyUserForce(&userforce);
		break;
	case VK_RIGHT:
		userforce.x = m_Skeleton.matrix.m[0];
		userforce.y = m_Skeleton.matrix.m[4];
		userforce.z = m_Skeleton.matrix.m[8];
		m_PhysEnv.ApplyUserForce(&userforce);
		break;
	case VK_LEFT:
		userforce.x = -m_Skeleton.matrix.m[0];
		userforce.y = -m_Skeleton.matrix.m[4];
		userforce.z = -m_Skeleton.matrix.m[8];
		m_PhysEnv.ApplyUserForce(&userforce);
		break;
	case VK_UP:
		userforce.x = -m_Skeleton.matrix.m[2];
		userforce.y = -m_Skeleton.matrix.m[6];
		userforce.z = -m_Skeleton.matrix.m[10];
		m_PhysEnv.ApplyUserForce(&userforce);
		break;
	case VK_DOWN:
		userforce.x = m_Skeleton.matrix.m[2];
		userforce.y = m_Skeleton.matrix.m[6];
		userforce.z = m_Skeleton.matrix.m[10];
		m_PhysEnv.ApplyUserForce(&userforce);
		break;
	}

	MSG msg;//取出Message用

	if (m_SimRunning)
	{
		while (m_SimRunning == TRUE)
		{
			/*http://msdn.microsoft.com/en-us/library/ms644943%28VS.85%29.aspx*/
			while (::PeekMessage(&msg,0,0,0,PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					/*
					 http://msdn.microsoft.com/en-us/library/ms632641%28v=VS.85%29.aspx
					 The WM_QUIT message indicates a request to terminate an application and 
					 is generated when the application calls the PostQuitMessage function.
					 有要關閉的要求出來
					*/
					m_SimRunning = FALSE;
					m_hDC = NULL;
					/*
					  The PostQuitMessage function posts a WM_QUIT message to the thread's message queue and returns immediately; 
					  the function simply indicates to the system that the thread is requesting to quit at some time in the future.
					*/
					PostQuitMessage(0);
					break;
				}
				if (msg.message == WM_CLOSE)
				{
					/*
					  http://msdn.microsoft.com/en-us/library/ms632617%28VS.85%29.aspx
					  The WM_CLOSE message is sent as a signal that a window or an application should terminate.
					  應該停掉程式
					*/
					m_SimRunning = FALSE;
				}

				/*
				  Used by class CWinApp  to translate window messages 
				  before they are dispatched to the TranslateMessage  and DispatchMessage  Windows functions
				*/
				// Dispatch any messages as needed
				if (!AfxGetApp()->PreTranslateMessage(&msg))
				{
					/*
					 http://msdn.microsoft.com/en-us/library/ms644955%28VS.85%29.aspx
					 translates virtual-key messages into character messages
					*/
					::TranslateMessage(&msg);
					/*
					  http://msdn.microsoft.com/en-us/library/ms644934%28VS.85%29.aspx
					  It is typically used to dispatch a message retrieved by the GetMessage function.
					*/
					::DispatchMessage(&msg);
				}
				
				// Give the Idle system some time
				AfxGetApp()->OnIdle(0);
				AfxGetApp()->OnIdle(1);

			}
			if (m_SimRunning) drawScene();
		}
	}
	else
		Invalidate(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	OnMouseMove
// Purpose:		Handle mouse moves while pressed
//			    用來旋轉場景			    
///////////////////////////////////////////////////////////////////////////////		
void COGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	tVector	localX,localY;

	if (nFlags & MK_LBUTTON > 0)//左邊的Key被按下時
	{
		// IF I AM HOLDING THE 'CONTROL' BUTTON ROTATE
		if ((nFlags & MK_CONTROL) > 0 && m_CurBone != NULL)
		{
		}	
		// ELSE ROTATE THE BONE
		else if ((nFlags & MK_SHIFT) > 0)
		{
			if ((point.x - m_mousepos.x) != 0)//x軸的偏差量是目前是屬於y軸旋轉
			{
				m_Skeleton.rot.y = m_Base_Rot_Y + ((float)ROTATE_SPEED * (point.x - m_mousepos.x));
				drawScene();
			}
			if ((point.y - m_mousepos.y) != 0)//y軸偏差量目前是屬於x軸旋轉
			{
				m_Skeleton.rot.z = m_Base_Rot_Z + ((float)ROTATE_SPEED * (point.y - m_mousepos.y));
				drawScene();
			}
		}
		else if (m_SimRunning)	// NO MODIFIERS, JUST DRAG SO USE MOUSESPRINGS
		{
			// NEED TO GET THE VECTORS FOR THE LOCAL X AND Y AXES
			localY.x = m_Skeleton.matrix.m[1];
			localY.y = m_Skeleton.matrix.m[5];
			localY.z = m_Skeleton.matrix.m[9];

			localX.x = m_Skeleton.matrix.m[0];
			localX.y = m_Skeleton.matrix.m[4];
			localX.z = m_Skeleton.matrix.m[8];

			/*拉動滑鼠產生力*/
			m_PhysEnv.SetMouseForce(point.x - m_mousepos.x,point.y - m_mousepos.y,&localX,&localY);
			m_PhysEnv.MouseForceActive = TRUE;
		}
	}
	else if ((nFlags & MK_RBUTTON) == MK_RBUTTON)
	{
		if ((nFlags & MK_CONTROL) > 0 && m_CurBone != NULL)
		{
		}
		else if ((nFlags & MK_SHIFT) > 0)
		{
			if ((point.x - m_mousepos.x) != 0)
			{
				m_Skeleton.rot.x = m_Base_Rot_X + ((float)ROTATE_SPEED * (point.x - m_mousepos.x));
				drawScene();
			}
		}
		else
		{
			if ((point.x - m_mousepos.x) != 0)
			{
				m_Skeleton.rot.y = m_Base_Rot_Y + ((float)ROTATE_SPEED * (point.x - m_mousepos.x));
				drawScene();
			}
			if ((point.y - m_mousepos.y) != 0)
			{
				m_Skeleton.rot.z = m_Base_Rot_Z + ((float)ROTATE_SPEED * (point.y - m_mousepos.y));
				drawScene();
			}
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	OnLButtonDblClk
// Purpose:		Left Double click, get dialog for Orientation
///////////////////////////////////////////////////////////////////////////////		
void COGLView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	NewSystem
// Purpose:		Clears the Simulation
///////////////////////////////////////////////////////////////////////////////		
void COGLView::NewSystem()
{
	m_PhysEnv.FreeSystem();
	m_SimRunning = FALSE;
	if (m_Skeleton.childCnt > 0)//表示有Child Bone
	{
		if (m_Skeleton.children->visuals->vertexData)
			free(m_Skeleton.children->visuals->vertexData);
		if (m_Skeleton.children->visuals->faceIndex)
			free(m_Skeleton.children->visuals->faceIndex);
		free(m_Skeleton.children->visuals);
		free(m_Skeleton.children);
		m_Skeleton.childCnt = 0;
	}
	drawScene();
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	LoadFiles
// Purpose:		Loads the OBJ files into memory
///////////////////////////////////////////////////////////////////////////////		
void COGLView::LoadFile(CString file1,CString baseName,CString ext) 
{
/// Local Variables ///////////////////////////////////////////////////////////
	t_Bone	*children;
	t_Visual *visual;
	FILE	*fp;
///////////////////////////////////////////////////////////////////////////////
	ext.MakeUpper();
	if (ext == "OBJ")//讀Obj文件檔
	{
		visual = (t_Visual *)malloc(sizeof(t_Visual));
		NewSystem();	// CLEAR WHAT DATA IS THERE
		// I WANT TO LOAD JUST THE VERTICES AND PUT THEM IN A INDEXED FORMAT
		if (file1.GetLength() > 0 && LoadOBJ((char *)(LPCTSTR)file1 ,visual,
				LOADOBJ_VERTEXONLY | LOADOBJ_REUSEVERTICES))
		{
			// INFORM THE PHYSICAL SIMULATION OF THE PARTICLES
			m_PhysEnv.SetWorldParticles((tVector *)visual->vertexData,visual->vertexCnt);

			children = (t_Bone *)malloc(sizeof(t_Bone));
			m_CurBone = &children[m_Skeleton.childCnt];
			ResetBone(m_CurBone,&m_Skeleton);//將目前的m_CurBone->parent設給m_Skeleton
			strcpy(m_CurBone->name,(LPCTSTR)baseName);//會是檔案名稱
			m_CurBone->visuals = visual;//設定Vertex
			m_CurBone->visualCnt = 1;
			m_Skeleton.childCnt = 1;//進行Skeleton之連結
			m_Skeleton.children = children;
		}
		else
		{
			MessageBox("Must Be A Valid OBJ File","Error",MB_OK);
			free(visual);
		}
	}
	else	// LOAD SIM SYSTEM
	{
		if (file1.GetLength())
		{
			fp = fopen(file1,"rb");
			if (fp != NULL)
			{
				NewSystem();	// CLEAR WHAT DATA IS THERE
				fread(&m_Skeleton,sizeof(t_Bone),1,fp);
				if (m_Skeleton.childCnt > 0)
				{
					m_Skeleton.children = (t_Bone *)malloc(sizeof(t_Bone));
					fread(m_Skeleton.children,sizeof(t_Bone),1,fp);
					if (m_Skeleton.children->visualCnt > 0)
					{
						m_Skeleton.children->visuals = (t_Visual *)malloc(sizeof(t_Visual));
						visual = m_Skeleton.children->visuals;
						fread(visual,sizeof(t_Visual),1,fp);
						if (visual->reuseVertices)
						{
							visual->vertexData = (float *)malloc(sizeof(float) * visual->vSize * visual->vertexCnt);
							visual->faceIndex = (unsigned short *)malloc(sizeof(unsigned short) * visual->faceCnt * visual->vPerFace);
							fread(visual->vertexData,sizeof(float),visual->vSize * visual->vertexCnt,fp);
							fread(visual->faceIndex,sizeof(unsigned short),visual->faceCnt * visual->vPerFace,fp);
						}
						// SAVE THE PHYSICAL SIMULATION OF THE PARTICLES
						m_PhysEnv.LoadData(fp);
					}
				}
				fclose(fp);
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	SaveFiles
// Purpose:		Saves the Particle System 
///////////////////////////////////////////////////////////////////////////////		
void COGLView::SaveFile(CString file1,CString baseName) 
{
/// Local Variables ///////////////////////////////////////////////////////////
	t_Visual *visual;
	FILE	*fp;
///////////////////////////////////////////////////////////////////////////////
	/*
	  儲存骨架系統而已
	  總共有三樣東西要儲存
	  1.基本骨架 m_Skeleton
	  2.基本骨架的小孩 m_Skeleton.children
	  3.基本骨架的小孩的點資訊
	  4.還有Particle系統環境參數
	*/
	if (file1.GetLength() > 0)
	{
		fp = fopen(file1,"wb");
		if (fp != NULL)
		{
			fwrite(&m_Skeleton,sizeof(t_Bone),1,fp);
			if (m_Skeleton.childCnt > 0)
			{
				fwrite(m_Skeleton.children,sizeof(t_Bone),1,fp);
				if (m_Skeleton.children->visualCnt > 0)
				{
					visual = m_Skeleton.children->visuals;
					fwrite(visual,sizeof(t_Visual),1,fp);
					if (visual->reuseVertices)
					{
						fwrite(visual->vertexData,sizeof(float),visual->vSize * visual->vertexCnt,fp);
						fwrite(visual->faceIndex,sizeof(unsigned short),visual->faceCnt * visual->vPerFace,fp);
					}
					// SAVE THE PHYSICAL SIMULATION OF THE PARTICLES
					m_PhysEnv.SaveData(fp);
				}
			}
			fclose(fp);
		}
	}
}

void COGLView::OnClose() 
{
	
	CWnd::OnClose();
}
/*set Simulation property被按下時*/
void COGLView::OnSimulationSetsimproperties() 
{
	m_PhysEnv.SetWorldProperties();		
}
/*Set Vertex Mass被按下時*/
void COGLView::OnSimulationSetVertexMass() 
{
	m_PhysEnv.SetVertexMass();		
}
/*Set Timing Properties被按下時*/
void COGLView::OnSetTimeProperties() 
{
	CTimeProps dialog;
	dialog.m_Iterations = m_TimeIterations;
	dialog.m_FixedTimeSteps = m_UseFixedTimeStep;
	dialog.m_MaxTimeStep = m_MaxTimeStep;
	if (dialog.DoModal())
	{
		m_TimeIterations = dialog.m_Iterations;
		m_UseFixedTimeStep = dialog.m_FixedTimeSteps;
		m_MaxTimeStep = dialog.m_MaxTimeStep;
	}
}


void COGLView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	
	CWnd::OnMButtonDown(nFlags, point);
}
