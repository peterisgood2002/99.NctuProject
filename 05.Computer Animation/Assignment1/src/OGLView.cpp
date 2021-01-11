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

/*�غc�l �NOpenGL�����F��غc�_��*/
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
	m_UseFixedTimeStep = FALSE;//TimeStep�j�p�|��
	m_MaxTimeStep = 0.01f;//TimeStep�j�p��0.01

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

/*Message Map�Ҧb���B*/
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
/*�w�qScreen Pixel���榡*/
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
    ppfd->cColorBits = 16;//���FAlpha��Color buffer�j�p
    ppfd->cDepthBits = 16;//Specifies the depth of the depth (z-axis) buffer.
    ppfd->cAccumBits = 0;//Specifies the total number of bitplanes in the accumulation buffer.
    ppfd->cStencilBits = 0;//Specifies the depth of the stencil buffer.

	/*
	  http://msdn.microsoft.com/en-us/library/dd318284%28VS.85%29.aspx
	  ��X����Pixel format descriptor
	  This function compares 
	  the generic pixel formats supported by Windows NT and 
	  any device pixel formats supported by special hardware accelerators with the pixel format you described, 
	  
	  and returns the best match
	*/
    pixelformat = ChoosePixelFormat(hdc, ppfd);

    if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0) {//�p�Gassign���ѫh�o�Ϳ��~�T��
        MessageBox("ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (pfd.dwFlags & PFD_NEED_PALETTE) {//���Hardware�O��8 bit�ҥH�ݭn��palette(�զ�O)
        MessageBox("Needs palette", "Error", MB_OK);
        return FALSE;
    }

	/*
	  http://msdn.microsoft.com/en-us/library/dd369049%28VS.85%29.aspx
	  �]�wDevice��Pixel format
	*/
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) {
        MessageBox("SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

/*
  �b�إ�GLView�ɷ|�I�s���禡��
  ���F��|�N�̨�OpenGL�������]�w���]�w�n
  �ҦpRender Context �H��Perspective view����
*/
int COGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
/// Local Variables ///////////////////////////////////////////////////////////
	RECT rect;
///////////////////////////////////////////////////////////////////////////////
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*http://msdn.microsoft.com/en-us/library/aa921543.aspx
	  ���Xwindows client area��Display Device Context
	*/
    m_hDC = ::GetDC(m_hWnd);
    if (!SetupPixelFormat(m_hDC))//���˸m�]�w���Q�n��Pixel Format
		PostQuitMessage (0);
	/*
	  To create a rendering context, you use the wglCreateContext function
	  wglCreateContext returns an HGLRC, which is a handle to the rendering context
	*/
    m_hRC = wglCreateContext(m_hDC);
	/*
	  �Ψӫ��w�ثe��Render Context
	  The wglMakeCurrent function makes a specified OpenGL rendering context 
	          the calling thread's current rendering context. 
	  All subsequent OpenGL calls made by the thread are drawn on the device identified by hdc
	*/
    wglMakeCurrent(m_hDC, m_hRC);
    GetClientRect(&rect);
    initializeGL(rect.right, rect.bottom);//��l��OpenGL

	// CREATE THE DISPLAY LIST FOR AN AXIS WITH ARROWS POINTING IN
	// THE POSITIVE DIRECTION Red = X, Green = Y, Blue = Z
	DeclareSomeCommandList();

	glDisable(GL_TEXTURE_2D);

	drawScene();
	return 0;
}
/*
  Display lists are groups of GL commands that have been stored for subsequent execution
  �i�H��glCallList�өI�s�ð���
  �o��u�O�ŧi�Ӥw
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
/*��l��OpenGL �D�n�O�]�wperspective �Mlighting*/
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
  ���ͤ@��message box�åB��XGL������T

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
			DeltaTime = Time - m_LastTime;//�ɶ����j
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

	/*���઺��ɴ���*/
	if (m_Skeleton.rot.y  > 360.0f) m_Skeleton.rot.y  -= 360.0f;
    if (m_Skeleton.rot.x   > 360.0f) m_Skeleton.rot.x   -= 360.0f;
    if (m_Skeleton.rot.z > 360.0f) m_Skeleton.rot.z -= 360.0f;

  //  glDisable(GL_DEPTH_TEST);	// TURN OFF DEPTH TEST FOR CLEAR

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glEnable(GL_DEPTH_TEST);	// ENABLE DEPTH TESTING

    glPushMatrix();

    // Set root skeleton's orientation and position
    glTranslatef(m_Skeleton.trans.x, m_Skeleton.trans.y, m_Skeleton.trans.z);//����

	// ROTATE THE ROOT
	glRotatef(m_Skeleton.rot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(m_Skeleton.rot.y, 0.0f, 1.0f, 0.0f);
 	glRotatef(m_Skeleton.rot.z, 0.0f, 0.0f, 1.0f); 

	// GRAB THE MATRIX AT THIS POINT SO I CAN USE IT FOR THE DEFORMATION
	glGetFloatv(GL_MODELVIEW_MATRIX,m_Skeleton.matrix.m);//���oModelView��Matrix m_Skeleton.matrix.m = MODELVIEW_MATRIX

	if (m_PickX > -1)//��ܦ��I��F��
		m_PhysEnv.GetNearestP(m_PickX,m_PickY);

	RunSim();//�]����
	
	//���X�T�b
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
		m_PickX = point.x;//�ƹ���X�b
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
/*�B�zKey�Q���U�h���ɭԸӰ��ƻ�ʧ@*/
void COGLView::HandleKeyUp(UINT nChar) 
{
	tVector userforce;
	switch (nChar)
	{
	case VK_RETURN://Enter��Q���J
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
			m_LastTime = GetTime() * m_TimeIterations;	// RESET THE SIM  ����Simulation�ثe�]����̤F
		m_StartTime = timeGetTime();//�]�w�ثe�_�l�ɶ� millisecond
		m_FrameCnt = 0;//�ثe�٨S������Frame
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

	MSG msg;//���XMessage��

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
					 ���n�������n�D�X��
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
					  ���Ӱ����{��
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
//			    �Ψӱ������			    
///////////////////////////////////////////////////////////////////////////////		
void COGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	tVector	localX,localY;

	if (nFlags & MK_LBUTTON > 0)//���䪺Key�Q���U��
	{
		// IF I AM HOLDING THE 'CONTROL' BUTTON ROTATE
		if ((nFlags & MK_CONTROL) > 0 && m_CurBone != NULL)
		{
		}	
		// ELSE ROTATE THE BONE
		else if ((nFlags & MK_SHIFT) > 0)
		{
			if ((point.x - m_mousepos.x) != 0)//x�b�����t�q�O�ثe�O�ݩ�y�b����
			{
				m_Skeleton.rot.y = m_Base_Rot_Y + ((float)ROTATE_SPEED * (point.x - m_mousepos.x));
				drawScene();
			}
			if ((point.y - m_mousepos.y) != 0)//y�b���t�q�ثe�O�ݩ�x�b����
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

			/*�԰ʷƹ����ͤO*/
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
	if (m_Skeleton.childCnt > 0)//��ܦ�Child Bone
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
	if (ext == "OBJ")//ŪObj�����
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
			ResetBone(m_CurBone,&m_Skeleton);//�N�ثe��m_CurBone->parent�]��m_Skeleton
			strcpy(m_CurBone->name,(LPCTSTR)baseName);//�|�O�ɮצW��
			m_CurBone->visuals = visual;//�]�wVertex
			m_CurBone->visualCnt = 1;
			m_Skeleton.childCnt = 1;//�i��Skeleton���s��
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
	  �x�s���[�t�ΦӤw
	  �`�@���T�˪F��n�x�s
	  1.�򥻰��[ m_Skeleton
	  2.�򥻰��[���p�� m_Skeleton.children
	  3.�򥻰��[���p�Ī��I��T
	  4.�٦�Particle�t�����ҰѼ�
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
/*set Simulation property�Q���U��*/
void COGLView::OnSimulationSetsimproperties() 
{
	m_PhysEnv.SetWorldProperties();		
}
/*Set Vertex Mass�Q���U��*/
void COGLView::OnSimulationSetVertexMass() 
{
	m_PhysEnv.SetVertexMass();		
}
/*Set Timing Properties�Q���U��*/
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
