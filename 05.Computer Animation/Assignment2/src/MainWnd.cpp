// MainWnd.cpp: implementation of the CMainWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainWnd.h"
#include <fstream>
#include <string>
#include "GL/glut.h"
#include "display.h"

#pragma comment (lib, "GLaux.lib")

using namespace std;

Display displayer;//用來顯示AVTAR用的

static double Counter;


// for timing
#define FPS			30
double Freq;

void CMainWnd::light_init() 
{
    // set material properties
    GLfloat white8[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat white2[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat mat_shininess[] = {100.0f};	// Phong exponent

    GLfloat light0_position[] = {1.0f, 5.0f, 1.0f, 0.0f}; // directional light (w=0)

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white2);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white8);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white2);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);	// normalize normal vectors
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // two-sided lighting
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMainWnd::CMainWnd():
	m_hWnd(NULL),
	m_hRC(NULL)
{
	pMotion = NULL;
	pSkeleton = NULL;
	nFrameNum = 0;
}

CMainWnd::~CMainWnd()
{
}

//
// InitScene()
// Called when the OpenGL RC has been created. Sets the initial state for
// the OpenGL scene.
//
void CMainWnd::InitScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			//Background color

	glEnable(GL_DEPTH_TEST);

	light_init();
}

//
// KillScene()
// Called when the OpenGL RC is about to be destroyed. 
//
void CMainWnd::KillScene()
{
	// TODO: Use KillScene to free resources.
}

//
// DrawScene()
// Called each time the OpenGL scene has to be drawn.
//
void CMainWnd::DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers

	glLoadIdentity();
	gluLookAt(0,50,200,0,50,0,0,1,0);//會乘上目前正在使用的Matrix
	//gluLookAt(0,50,100,0,50,0,0,1,0);
	glPushMatrix();
		
		glEnable(GL_LIGHTING);

		GLfloat white2[] = {0.2f, 0.2f, 0.2f, 0.2f};
		GLfloat white6[] = {0.6f, 0.6f, 0.6f, 0.6f};
		GLfloat mat_shininess[] = {50.0f};		/* Phong exponent */
		GLfloat blue[] = {0.3f, 0.4f, 1.0f, 0.2f};

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white2);	/* no ambient */
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white6);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

		if (pSkeleton)
			displayer.show();//畫AVATAR
		
		
		//floor
		int count = 0;

		GLfloat  white[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat white8[] = {0.8f, 0.8f, 0.8f, 1.0f};
		GLfloat green8[] = {0.3f, 0.5f, 0.3f, 1.0f};
		GLfloat   grey[] = {0.4f, 0.4f, 0.4f, 1.0f};

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);


		float i, j;
		/*畫地板*/
		glBegin(GL_QUADS);
			for(i=-500.;i<=500.;i+=20) 
			{
				for(j=-500.;j<=500.;j+=20) 
				{			 
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
					if((count%2) == 0) 
					{
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white8);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white8);	
					}
					else 
					{
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green8);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
					}

					glNormal3f(0.0f,1.0f,0.0f);

					glVertex3f(j,  0, i);
					glVertex3f(j,  0, i+20);
					glVertex3f(j+20,0, i+20);
					glVertex3f(j+20,0, i);
					count++;
				}
			}
		glEnd();

		glDisable(GL_BLEND);

		/*畫坐標軸*/
		glDisable( GL_LIGHTING );
		glBegin(GL_LINES);
	    // draw x axis in red, y axis in green, z axis in blue 
	    glColor3f(1., 0.0, 0.0);
	    glVertex3f(0., 0., 0.);
	    glVertex3f(20, 0., 0.);

	    glColor3f(0.0, 1., 0.0);
	    glVertex3f(0., 0., 0.);
	    glVertex3f(0., 20, 0.);

	    glColor3f(0.0, 0.0, 1.);
	    glVertex3f(0., 0., 0.);
	    glVertex3f(0., 0., 20);

	    glEnd();

	glPopMatrix();

	glFlush();
}

//
// Tick()
// The applications "heartbeat" function. Called before another frame needs
// to be drawn. Use this function to calculate new positions for objects in 
// the scene, for instance. Set bRedrawScene = FALSE if the scene should not be 
// redrawn.
//
void CMainWnd::Tick(BOOL &bRedrawScene)
{
	UpdateFPS();
	/*
	  http://msdn.microsoft.com/en-us/library/ms644904%28VS.85%29.aspx
	  The QueryPerformanceCounter function retrieves the current value of the high-resolution performance counter. 
	*/
	/*http://msdn.microsoft.com/en-us/library/aa383713%28VS.85%29.aspx*/
    LARGE_INTEGER tempCounter;
    double temp = 0;
	QueryPerformanceCounter(&tempCounter);
	temp = (double) tempCounter.QuadPart; //Signed 64-bit integer.		
	if( temp-Counter > Freq / FPS )
	{
		if ( pMotion != NULL && pMotion->active == true ) {
			nFrameNum++;
			if(nFrameNum == pMotion->m_NumFrames  ) {
				if ( pMotion->loop == true )
					nFrameNum = 0;
				else pMotion->active = false;
			}
			displayer.m_pActor[0]->setPosture(pMotion->m_pPostures[nFrameNum]);
			Counter = temp;
		}
	}
}

//
// UpdateFPS()
// Calculates FPS and shows the result in the window caption on regular
// intervals. Needs to be called every frame update.
// 修改Frame Per Second的值並且修改Title
//
void CMainWnd::UpdateFPS()
{
	const int FRAMEINTERVAL = 1000;            // Show FPS every 1000th milliseconds
	static DWORD nFrames = 0;                  // Number of frames since last update
	static DWORD nLastTick = GetTickCount();   // Time of last update
	DWORD nTick = GetTickCount();              // Current time
	if(nTick-nLastTick>=FRAMEINTERVAL)//如果時間間隔超過範圍
	{	
		float fFPS = 1000.0f*(GLfloat)nFrames/(GLfloat)(nTick-nLastTick);//算出FPS
		nLastTick = nTick;
		nFrames = 0;
		char szFPS[256];
		sprintf_s(szFPS, "%.1f FPS", fFPS);
		/*http://msdn.microsoft.com/en-us/library/ms633546%28VS.85%29.aspx*/
		SetWindowText(m_hWnd, szFPS);
	}
	nFrames++;
}



void CMainWnd::LoadASF(string name) // replace the original one
{
	if(pSkeleton)
		delete pSkeleton;
	
	// Loading asf file:
	pSkeleton = new Skeleton(name, 1.0f);
	displayer.loadActor(pSkeleton);
}

void CMainWnd::LoadAMC(string name) // replace the original one
{
	if(pMotion)
		delete pMotion;
	
	// Loading asf file:
	if ( pSkeleton == NULL ) {
		throw "並沒有事先讀入ASF檔 因此發生錯誤\n";
		return;
	}

	pMotion = new Motion(name, 1.0f ,pSkeleton );
	displayer.loadMotion(pMotion); 
	displayer.m_pActor[0]->setPosture(pMotion->m_pPostures[0]);
}

void CMainWnd::KeyMsg(WPARAM key)
{
	switch(key)
	{
		case 'P':
			pMotion->active = !pMotion->active;			
			break;	
		case 'N':
			pMotion->DrawNextPosture();
	}
}

void CMainWnd::IK()
{
	if(pMotion)
		pMotion->IK();
}

void CMainWnd::SetPerspectiveParam (float fovy, float aspect, float znear, float zfar)
{
	m_fFovy   = fovy;
	m_fAspect = aspect;
	m_fZNear  = znear;
	m_fZFar   = zfar;
}