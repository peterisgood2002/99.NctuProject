#include "stdafx.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <assert.h>
#include <math.h>

#include "Particle.h"
#include "Phys.h"
#include "SimProps.h"
#include "VertMass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable:4244)      // I NEED TO CONVERT FROM DOUBLE TO FLOAT

/////////////////////////////////////////////////////////////////////////////
// CPhys

// INITIALIZE THE SIMULATION WORLD
CPhys::CPhys()
{
	Pick[0] = -1;
	Pick[1] = -1;//�ƹ��I�쪺�I
	//ParticleSys[0] = NULL;
	//ParticleSys[1] = NULL;
	//ParticleSys[2] = NULL;	// RESET BUFFER
	CurrentSys = TargetSys = InitSys = NULL;

	// THESE TEMP PARTICLE BUFFERS ARE NEEDED FOR THE MIDPOINT AND RK4 INTEGRATOR
	for (int i = 0; i < 5; i++)
		TempSys[i] = NULL;
	ParticleCnt = 0;//�w�]
	Contact = NULL;
	Spring = NULL;
	SpringCnt = 0;		

	UseGravity = TRUE;
	UseViscosity = true;
	DrawSprings = TRUE;
	DrawVertices	= TRUE;
	MouseForceActive = FALSE;

	MAKEVECTOR(Gravity, 0.0f, -0.2f, 0.0f)//�]�w���O�Ѽ�
	UserForceMag = 100.0;
	UserForceActive = FALSE;
	Kd	= 0.02f;	// DAMPING FACTOR
	Kr	= 0.8f;		// 1.0 = SUPERBALL BOUNCE 0.0 = DEAD WEIGHT
	Ksh	= 5.0f;		// HOOK'S SPRING CONSTANT
	Ksd	= 0.1f;		// SPRING DAMPING CONSTANT

	MouseForceKs = 0.08f;	// MOUSE SPRING CONSTANT

	// CREATE THE SIZE FOR THE SIMULATION WORLD
	WorldSizeX = 15.0f;
	WorldSizeY = 15.0f;
	WorldSizeZ = 15.0f;

	IntegratorType = EULER_INTEGRATOR;

	//CollisionPlane = (tCollisionPlane	*)malloc(sizeof(tCollisionPlane) * 6);//6�ӸI������
	CollisionPlane = new tCollisionPlane[6];
	CollisionPlaneCnt = 6;

	// MAKE THE TOP PLANE (CEILING)
	MAKEVECTOR(CollisionPlane[0].normal,0.0f, -1.0f, 0.0f)//�W�b����Normal�¤U
	CollisionPlane[0].d = WorldSizeY / 2.0f;

	// MAKE THE BOTTOM PLANE (FLOOR)
	MAKEVECTOR(CollisionPlane[1].normal,0.0f, 1.0f, 0.0f)//�U�b����Normal�¤W
	CollisionPlane[1].d = WorldSizeY / 2.0f;

	// MAKE THE LEFT PLANE
	MAKEVECTOR(CollisionPlane[2].normal,-1.0f, 0.0f, 0.0f)//���b����Normal��x�b�t��V
	CollisionPlane[2].d = WorldSizeX / 2.0f;

	// MAKE THE RIGHT PLANE
	MAKEVECTOR(CollisionPlane[3].normal,1.0f, 0.0f, 0.0f)//�k�b����Normal��x�b����V
	CollisionPlane[3].d = WorldSizeX / 2.0f;

	// MAKE THE FRONT PLANE
	MAKEVECTOR(CollisionPlane[4].normal,0.0f, 0.0f, -1.0f)
	CollisionPlane[4].d = WorldSizeZ / 2.0f;

	// MAKE THE BACK PLANE
	MAKEVECTOR(CollisionPlane[5].normal,0.0f, 0.0f, 1.0f)
	CollisionPlane[5].d = WorldSizeZ / 2.0f;

	CollisionRootFinding = FALSE;		// ONLY SET WHEN FINDING A COLLISION
}

CPhys::~CPhys()
{
	/*if ( CurrentSys )
		free( CurrentSys);
	if ( TargetSys )
		free( TargetSys );
	if (InitSys)
		free(InitSys);

	for (int i = 0; i < 5; i++)
	{
		if (TempSys[i])
			free(TempSys[i]);
	}
	if (Contact)
		free(Contact);
	if (Spring)
		free(Spring);
	free(CollisionPlane);*/

	FreeSystem();
}

///////////////////////////////////////////////////////////////////////////////
// Function:	RenderWorld
// Purpose:		Draw the current system (particles, springs, userforces)
///////////////////////////////////////////////////////////////////////////////
void CPhys::RenderWorld()
{
	tParticle	*tempParticle;
	tSpring		*tempSpring;

	// FIRST DRAW THE WORLD CONTAINER  
	glColor3f(1.0f,1.0f,1.0f);
    // do a big linestrip to get most of edges�@�@�e�W����
    glBegin(GL_LINE_STRIP);
        glVertex3f(-WorldSizeX/2.0f, WorldSizeY/2.0f,-WorldSizeZ/2.0f);
        glVertex3f( WorldSizeX/2.0f, WorldSizeY/2.0f,-WorldSizeZ/2.0f);
        glVertex3f( WorldSizeX/2.0f, WorldSizeY/2.0f, WorldSizeZ/2.0f);
        glVertex3f(-WorldSizeX/2.0f, WorldSizeY/2.0f, WorldSizeZ/2.0f);
        glVertex3f(-WorldSizeX/2.0f, WorldSizeY/2.0f,-WorldSizeZ/2.0f);
        glVertex3f(-WorldSizeX/2.0f,-WorldSizeY/2.0f,-WorldSizeZ/2.0f);
    glEnd();
    // fill in the stragglers�@�e�X�k�䥭��
    glBegin(GL_LINES);
        glVertex3f( WorldSizeX/2.0f, WorldSizeY/2.0f,-WorldSizeZ/2.0f);
        glVertex3f( WorldSizeX/2.0f,-WorldSizeY/2.0f,-WorldSizeZ/2.0f);

        glVertex3f( WorldSizeX/2.0f, WorldSizeY/2.0f, WorldSizeZ/2.0f);
        glVertex3f( WorldSizeX/2.0f,-WorldSizeY/2.0f, WorldSizeZ/2.0f);

        glVertex3f(-WorldSizeX/2.0f, WorldSizeY/2.0f, WorldSizeZ/2.0f);
        glVertex3f(-WorldSizeX/2.0f,-WorldSizeY/2.0f, WorldSizeZ/2.0f);
    glEnd();
    
    // draw floor�@�e�X�a�O
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);
        glColor3f(0.0f,0.2f,0.0f);
        glVertex3f(-WorldSizeX/2.0f,-WorldSizeY/2.0f,-WorldSizeZ/2.0f);
        glVertex3f( WorldSizeX/2.0f,-WorldSizeY/2.0f,-WorldSizeZ/2.0f);
        glVertex3f( WorldSizeX/2.0f,-WorldSizeY/2.0f, WorldSizeZ/2.0f);
        glVertex3f(-WorldSizeX/2.0f,-WorldSizeY/2.0f, WorldSizeZ/2.0f);
    glEnd();
    glEnable(GL_CULL_FACE);


	if ( CurrentSys )
	{
		if (Spring && DrawSprings)
		{
			glBegin(GL_LINES);
			glColor3f(0.0f,0.8f,0.8f);
			tempSpring = Spring;
			for (int loop = 0; loop < SpringCnt; loop++)
			{
				glVertex3fv((float *)&CurrentSys[tempSpring->p1].pos);
				glVertex3fv((float *)&CurrentSys[tempSpring->p2].pos);
				tempSpring++;
			}
			if (MouseForceActive)	// DRAW MOUSESPRING FORCE
			{
				if (Pick[0] > -1)
				{
					glColor3f(0.8f,0.0f,0.8f);
					glVertex3fv((float *)&CurrentSys[Pick[0]].pos);
					glVertex3fv((float *)&MouseDragPos[0]);
				}
				if (Pick[1] > -1)
				{
					glColor3f(0.8f,0.0f,0.8f);
					glVertex3fv((float *)&CurrentSys[Pick[1]].pos);
					glVertex3fv((float *)&MouseDragPos[1]);
				}
			}
			glEnd();
		}
		if (DrawVertices)
		{
			glBegin(GL_POINTS);
			tempParticle = CurrentSys;//�e�X�ثe��Particle System���I
			for (int loop = 0; loop < ParticleCnt; loop++)
			{
				// Picked particles are green then red for 1 and 2
				if (loop == Pick[0])
					glColor3f(0.0f,0.8f,0.0f);
				else if (loop == Pick[1])
					glColor3f(0.8f,0.0f,0.0f);
				// If particles are in contact, Draw them in Orange
				else if (tempParticle->contacting)
					glColor3f(1.0f,0.5f,0.0f);
				// Normally Yellow
				else
					glColor3f(0.8f,0.8f,0.0f);

				glVertex3fv((float *)&tempParticle->pos);
				tempParticle++;
			}
			glEnd();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	GetNearestP
// Purpose:		Use OpenGL Feedback to find the closest point to a mouseclick
// Arguments:	Screen coordinates of the hit
///////////////////////////////////////////////////////////////////////////////
void CPhys::GetNearestP(int x, int y)
{
/// Local Variables ///////////////////////////////////////////////////////////
	float *feedBuffer;//��m�Ҧ�feedback��T
	int hitCount;
	tParticle *tempParticle;
	int loop;
///////////////////////////////////////////////////////////////////////////////
	// INITIALIZE A PLACE TO PUT ALL THE FEEDBACK INFO (3 DATA, 1 TAG, 2 TOKENS)
	/*�ھ�Grammar
				   				    feedbackList
								/			       \
							feedbackItem        feedbackItem 
								|				     |
							passThru		       point	
							/		\		   /           \
		GL_PASS_THROUGH_TOKEN     value  GL_POINT_TOKEN   vertex

	   �`�@�n������Ƥ~��˱o�U
	*/
	feedBuffer = (float *)malloc(sizeof(GLfloat) * ParticleCnt * 6);
	// TELL OPENGL ABOUT THE BUFFER
	/*information about primitives that would have been rasterized is fed back to the application using the GL*/
	glFeedbackBuffer(ParticleCnt * 6,GL_3D,feedBuffer);//�]�wFeedBuffer�j�p
	/*�]�w��Rasterize mode
	  GL_RENDER
          Render mode. Primitives are rasterized, producing pixel fragments, which are written into the frame buffer. 
		  This is the normal mode and also the default mode.

	  GL_SELECT
          Selection mode. No pixel fragments are produced, and no change to the frame buffer contents is made. 
		  Instead, a record of the names of primitives that would have been drawn 
		      if the render mode was GL_RENDER is returned in a select buffer, which must be created (see glSelectBuffer) 
			  before selection mode is entered.

	  GL_FEEDBACK
		  Feedback mode. No pixel fragments are produced, and no change to the frame buffer contents is made. 
		  Instead, the coordinates and attributes of vertices that would have been drawn 
		  is returned in a feedback buffer, which must be created (see glFeedbackBuffer) 
		  before feedback mode is entered. 
	  �|�^��
	  The return value of glRenderMode is determined by the render mode at the time glRenderMode is called, 
	        rather than by mode
	  GL_RENDER
		Zero.
	  GL_SELECT
		The number of hit records transferred to the select buffer.
	  GL_FEEDBACK
		The number of values (not vertices) transferred to the feedback buffer. 
	*/
	(void)glRenderMode(GL_FEEDBACK);	// SET IT IN FEEDBACK MODE

	/*�NParticle�e�W�h �M��N�|�e�b�ù��W���I������*/
	tempParticle = CurrentSys;//�ثe�t�Ϊ��Ĥ@��Particle
	for (loop = 0; loop < ParticleCnt; loop++)
	{
		// PASS THROUGH A MARKET LETTING ME KNOW WHAT VERTEX IT WAS �C�@��Vertex�����@��marker�n���H���D�쩳����̤F
		glPassThrough((float)loop);//place a marker in the feedback buffer 
		// SEND THE VERTEX
		glBegin(GL_POINTS);
		glVertex3fv((float *)&tempParticle->pos);
		glEnd();
		tempParticle++;
	}
	hitCount = glRenderMode(GL_RENDER); // HOW MANY HITS DID I GET �ثe�OGL_FEEDBACK
	CompareDepthBuffer(hitCount,feedBuffer,(float)x,(float)y);		// CHECK THE HIT 
	free(feedBuffer);		// GET RID OF THE MEMORY
}
////// GetNearestP ////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	CompareDepthBuffer
// Purpose:		Check the feedback buffer to see if anything is hit
// Arguments:	Number of hits, pointer to buffer, point to test
///////////////////////////////////////////////////////////////////////////////
void CPhys::CompareDepthBuffer(int size, float *buffer,float x, float y)
{
/// Local Variables ///////////////////////////////////////////////////////////
	GLint count;
	GLfloat token,point[3];
	int loop,currentVertex,result = -1;
	long nearest = -1, dist;
///////////////////////////////////////////////////////////////////////////////
	count = size;
	while (count)
	{
		token = buffer[size - count];	// CHECK THE TOKEN  �q�e�Y�}�l��
		count--;
		if (token == GL_PASS_THROUGH_TOKEN)	// VERTEX MARKER 
		{
			/*
			 ��Grammar����passThru <- GL_PASS_THROUGH_TOKEN value
			 �᭱��Value�N�O��Vertex�۹�����m
			*/
			currentVertex = (int)buffer[size - count]; // WHAT VERTEX �ثe�n�B�z��Vertex
			count--;
		}
		else if (token == GL_POINT_TOKEN)
		{
			/*
			 ��Grammar����point	<- GL_POINT_TOKEN vertex
			 �ҥH�᭱��vertex��(x ,y ,z)�y��
			*/
			// THERE ARE THREE ELEMENTS TO A POINT TOKEN
			for (loop = 0; loop < 3; loop++)
			{
				point[loop] = buffer[size - count];
				count--;
			}
			dist = ((x - point[0]) * (x - point[0])) + ((y - point[1]) * (y - point[1]));//�P(x ,y)�̪��I
			if (result == -1 || dist < nearest)
			{
				nearest = dist;
				result = currentVertex;
			}
		}
	}

	if (nearest < 50.0f)
	{
		if (Pick[0] == -1)
			Pick[0] = result;
		else if (Pick[1] == -1)
			Pick[1] = result;
		else
		{
			Pick[0] = result;
			Pick[1] = -1;
		}
	}
}
////// CompareDepthBuffer //////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	SetWorldParticles
// Purpose:		Inform the System of the particles under control
// Arguments:	List of vertices and count
///////////////////////////////////////////////////////////////////////////////
void CPhys::SetWorldParticles(tVector *coords,int particleCnt)
{
	tParticle *tempParticle;

	/*�N��ƲM��*/
	if ( CurrentSys )
		//free( CurrentSys);
		delete [] CurrentSys;
	if ( TargetSys )
		delete [] TargetSys;
		//free( TargetSys );
	if (InitSys)
		delete [] InitSys;
		//free(InitSys);
	for (int i = 0; i < 5; i++)
	{
		if (TempSys[i])
			delete [] TempSys[i];
			//free(TempSys[i]);
	}
	if (Contact)
		delete [] Contact;
		//free(Contact);
	/*
	  �}�l�]�w�t�� 
	*/
	// THE SYSTEM IS DOUBLE BUFFERED TO MAKE THINGS EASIER �N�@�Ǹ�ƪ�l��
	//CurrentSys = (tParticle *)malloc(sizeof(tParticle) * particleCnt);
	CurrentSys = new tParticle[particleCnt];
	//TargetSys = (tParticle *)malloc(sizeof(tParticle) * particleCnt);
	TargetSys = new tParticle[particleCnt];
	//InitSys = (tParticle *)malloc(sizeof(tParticle) * particleCnt);
	InitSys = new tParticle[particleCnt];
	for (int i = 0; i < 5; i++)
	{
		//TempSys[i] = (tParticle *)malloc(sizeof(tParticle) * particleCnt);
		TempSys[i] = new tParticle[particleCnt];
	}
	ParticleCnt = particleCnt;

	// MULTIPLIED PARTICLE COUNT * 2 SINCE THEY CAN COLLIDE WITH MULTIPLE WALLS
	//Contact = (tContact *)malloc(sizeof(tContact) * particleCnt * 2);//?
	Contact = new tContact[particleCnt];
	ContactCnt = 0;

	/*�]�wCurrentSys��Particle����T*/
	tempParticle = CurrentSys;
	for (int loop = 0; loop < particleCnt; loop++)
	{
		MAKEVECTOR(tempParticle->pos, coords->x, coords->y, coords->z)
		MAKEVECTOR(tempParticle->v, 0.0f, 0.0f, 0.0f)
		MAKEVECTOR(tempParticle->f, 0.0f, 0.0f, 0.0f)
		tempParticle->oneOverM = 1.0f;							// MASS OF 1
		tempParticle->contacting = FALSE;
		tempParticle++;
		coords++;
	}

	/*�NCurrentSys�ƻs��TargetSys�H��ParticleSys[2]�W*/
	// COPY THE SYSTEM TO THE SECOND ONE ALSO  
	memcpy(TargetSys,CurrentSys,sizeof(tParticle) * particleCnt);
	// COPY THE SYSTEM TO THE RESET BUFFER ALSO
	memcpy( InitSys,CurrentSys,sizeof(tParticle) * particleCnt);

	
}
////// SetWorldParticles //////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	FreeSystem
// Purpose:		Remove all particles and clear it out
///////////////////////////////////////////////////////////////////////////////
void CPhys::FreeSystem()
{
	Pick[0] = -1;
	Pick[1] = -1;
	
	/*if(_heapchk()!=_HEAPOK)
		DebugBreak();*/
	if ( CurrentSys )
	{
		//free( CurrentSys );
		delete [] CurrentSys;
		CurrentSys = NULL;
	}
	

	if ( TargetSys )
	{
		//free( TargetSys );
		delete [] TargetSys;
		TargetSys = NULL;
	}
	
	
	if ( InitSys )
	{
		//free( InitSys );
		delete [] InitSys;
		InitSys = NULL;
	}
	for (int i = 0; i < 5; i++)
	{
		if (TempSys[i])
		{
			//free(TempSys[i]);
			delete [] TempSys[i];
			TempSys[i] = NULL;	// RESET BUFFER
		}
	}
	if (Contact)
	{
		//free(Contact);
		delete [] Contact;
		Contact = NULL;
	}
	if (Spring)
	{
		//free(Spring);
		delete [] Spring;
		Spring = NULL;
	}
	SpringCnt = 0;	
	ParticleCnt = 0;
}
////// FreeSystem //////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	LoadData
// Purpose:		Load a simulation system 
// Arguments:	File pointer
///////////////////////////////////////////////////////////////////////////////
void CPhys::LoadData(FILE *fp)
{
	fread(&UseGravity,sizeof(BOOL),1,fp);
	fread(&UseViscosity,sizeof(BOOL),1,fp);
	fread(&UserForceActive,sizeof(BOOL),1,fp);
	fread(&Gravity,sizeof(tVector),1,fp);
	fread(&UserForce,sizeof(tVector),1,fp);
	fread(&UserForceMag,sizeof(float),1,fp);
	fread(&Kd,sizeof(float),1,fp);
	fread(&Kr,sizeof(float),1,fp);
	fread(&Ksh,sizeof(float),1,fp);
	fread(&Ksd,sizeof(float),1,fp);
	fread(&ParticleCnt,sizeof(int),1,fp);
	//CurrentSys = (tParticle *)malloc(sizeof(tParticle) * ParticleCnt);
	CurrentSys = new tParticle[ParticleCnt];
	//TargetSys = (tParticle *)malloc(sizeof(tParticle) * ParticleCnt);
	TargetSys = new tParticle[ParticleCnt];
	//InitSys = (tParticle *)malloc(sizeof(tParticle) * ParticleCnt);
	InitSys = new tParticle[ParticleCnt];
	for (int i = 0; i < 5; i++)
	{
		//TempSys[i] = (tParticle *)malloc(sizeof(tParticle) * ParticleCnt);
		TempSys[i] = new tParticle[ParticleCnt];
	}
	
	//Contact = (tContact *)malloc(sizeof(tContact) * ParticleCnt);
	Contact = new tContact[ParticleCnt];
	fread(CurrentSys,sizeof(tParticle),ParticleCnt,fp);
	fread(TargetSys,sizeof(tParticle),ParticleCnt,fp);
	fread(InitSys,sizeof(tParticle),ParticleCnt,fp);
	fread(&SpringCnt,sizeof(int),1,fp);
	//Spring = (tSpring *)malloc(sizeof(tSpring) * (SpringCnt));
	Spring = new tSpring[SpringCnt];
	fread(Spring,sizeof(tSpring),SpringCnt,fp);
	fread(Pick,sizeof(int),2,fp);
}
////// LoadData //////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	SaveData
// Purpose:		Save a simulation system 
// Arguments:	File pointer
///////////////////////////////////////////////////////////////////////////////
void CPhys::SaveData(FILE *fp)
{
	fwrite(&UseGravity,sizeof(BOOL),1,fp);
	fwrite(&UseViscosity,sizeof(BOOL),1,fp);
	fwrite(&UserForceActive,sizeof(BOOL),1,fp);
	fwrite(&Gravity,sizeof(tVector),1,fp);
	fwrite(&UserForce,sizeof(tVector),1,fp);
	fwrite(&UserForceMag,sizeof(float),1,fp);
	fwrite(&Kd,sizeof(float),1,fp);
	fwrite(&Kr,sizeof(float),1,fp);
	fwrite(&Ksh,sizeof(float),1,fp);
	fwrite(&Ksd,sizeof(float),1,fp);
	fwrite(&ParticleCnt,sizeof(int),1,fp);
	fwrite(CurrentSys,sizeof(tParticle),ParticleCnt,fp);
	fwrite(TargetSys,sizeof(tParticle),ParticleCnt,fp);
	fwrite(InitSys,sizeof(tParticle),ParticleCnt,fp);
	fwrite(&SpringCnt,sizeof(int),1,fp);
	fwrite(Spring,sizeof(tSpring),SpringCnt,fp);
	fwrite(Pick,sizeof(int),2,fp);
}
////// SaveData //////////////////////////////////////////////////////////////

// RESET THE SIM TO INITIAL VALUES
void CPhys::ResetWorld()
{
	memcpy(CurrentSys,InitSys,sizeof(tParticle) * ParticleCnt);
	memcpy(TargetSys,InitSys,sizeof(tParticle) * ParticleCnt);
}

void CPhys::SetWorldProperties()
{
	CSimProps	dialog;
	dialog.m_CoefRest = Kr;
	dialog.m_Damping = Kd;
	dialog.m_GravX = Gravity.x;
	dialog.m_GravY = Gravity.y;
	dialog.m_GravZ = Gravity.z;
	dialog.m_SpringConst = Ksh;
	dialog.m_SpringDamp = Ksd;
	dialog.m_UserForceMag = UserForceMag;
	dialog.m_MouseSpring = MouseForceKs;

	/*http://msdn.microsoft.com/en-us/library/619z63f5%28VS.71%29.aspx*/
	if (dialog.DoModal() == IDOK)//��Modal Dialog����� �ҥH�L�|�w��
	{
		/*
		 ������]�w������
		 The default OnOK member function will validate and 
		 update the dialog-box data and close the dialog box with result IDOK
		*/
		Kr = dialog.m_CoefRest;
		Kd = dialog.m_Damping;
		Gravity.x = dialog.m_GravX;
		Gravity.y = dialog.m_GravY;
		Gravity.z = dialog.m_GravZ;
		UserForceMag = dialog.m_UserForceMag;
		Ksh = dialog.m_SpringConst;
		Ksd = dialog.m_SpringDamp;
		for (int loop = 0; loop < SpringCnt; loop++)
		{
			Spring[loop].Ks = Ksh;
			Spring[loop].Kd = Ksd;
		}

		// GET THE NEW MOUSESPRING FORCE
		MouseForceKs = dialog.m_MouseSpring;
	}
}


void CPhys::SetVertexMass()
{
	CVertMass	dialog;
	dialog.m_VertexMass = CurrentSys[Pick[0]].oneOverM;
	if (dialog.DoModal() == IDOK)
	{
		CurrentSys[Pick[0]].oneOverM = dialog.m_VertexMass;
		CurrentSys[Pick[1]].oneOverM = dialog.m_VertexMass;
		TargetSys[Pick[0]].oneOverM = dialog.m_VertexMass;
		TargetSys[Pick[1]].oneOverM = dialog.m_VertexMass;
		InitSys[Pick[0]].oneOverM = dialog.m_VertexMass;
		InitSys[Pick[1]].oneOverM = dialog.m_VertexMass;
	}
}

void CPhys::ApplyUserForce(tVector *force)
{
	ScaleVector(force,  UserForceMag, &UserForce);
	UserForceActive = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	SetMouseForce 
// Purpose:		Allows the user to interact with selected points by dragging
// Arguments:	Delta distance from clicked point, local x and y axes
///////////////////////////////////////////////////////////////////////////////
void CPhys::SetMouseForce(int deltaX,int deltaY, tVector *localX, tVector *localY)
{
/// Local Variables ///////////////////////////////////////////////////////////
	tVector tempX,tempY;
///////////////////////////////////////////////////////////////////////////////
	ScaleVector(localX,  (float)deltaX * 0.03f, &tempX);
	ScaleVector(localY,  -(float)deltaY * 0.03f, &tempY);
	if (Pick[0] > -1)
	{
		VectorSum(&CurrentSys[Pick[0]].pos,&tempX,&MouseDragPos[0]);
		VectorSum(&MouseDragPos[0],&tempY,&MouseDragPos[0]);//�[�W�ƹ��O �Ĥ@���I��MouseDragPos[0]
	}
	if (Pick[1] > -1)
	{
		VectorSum(&CurrentSys[Pick[1]].pos,&tempX,&MouseDragPos[1]);
		VectorSum(&MouseDragPos[1],&tempY,&MouseDragPos[1]);//�[�W�ƹ��O �ĤG���I��MouseDragPos[0]
	}
}
/// SetMouseForce /////////////////////////////////////////////////////////////

void CPhys::AddSpring()
{
	tSpring	*spring;
	// MAKE SURE TWO PARTICLES ARE PICKED
	if (Pick[0] > -1 && Pick[1] > -1)
	{
		/*�X�RSpring�H�K�[�Jspring*/
		//spring = (tSpring *)malloc(sizeof(tSpring) * (SpringCnt + 1));
		spring = new tSpring[SpringCnt+1];
		if (Spring != NULL)
			memcpy(spring,Spring,sizeof(tSpring) * SpringCnt);
		
		Spring = spring;
		/*�[�J�@��Spring*/
		spring = &Spring[SpringCnt++];
		spring->Ks = Ksh;
		spring->Kd = Ksd;
		spring->p1 = Pick[0];
		spring->p2 = Pick[1];
		spring->restLen = 
			sqrt(VectorSquaredDistance(&CurrentSys[Pick[0]].pos, 
									   &CurrentSys[Pick[1]].pos));
	}
}		

void CPhys::ComputeForces( tParticle	*system, BOOL duringIntegration )
{
	int loop;
	tParticle	*curParticle,*p1, *p2;
	tSpring		*spring;
	float		dist, Hterm, Dterm;
	tVector		springForce,deltaV,deltaP;
	float		FdotN,VdotN,Vmag;		//�I���Ϊ�

	curParticle = system;
	for (loop = 0; loop < ParticleCnt; loop++)//��C�@��Particle�p��Damping�O�M���O
	{
		MAKEVECTOR(curParticle->f,0.0f,0.0f,0.0f)		// CLEAR FORCE VECTOR  ���N�O�]�w��0

		if (UseGravity && curParticle->oneOverM != 0) // && curParticle->type != CONTACTING)
		{
			// TO DO 
			/*�[�J���O��*/
			float particlm = 1.0 / curParticle->oneOverM;//���q
			curParticle->f.x += particlm * Gravity.x;
			curParticle->f.y += particlm * Gravity.y;
			curParticle->f.z += particlm * Gravity.z;
		}

		if (UseViscosity)
		{
			// TO DO
			/*fv = -Kv * V*/
			curParticle->f.x += (-Kd  * curParticle->v.x);
			curParticle->f.y += (-Kd * curParticle->v.y);
			curParticle->f.z += (-Kd * curParticle->v.z);
			
			
		}
		else//Viscosity���� �ιw�]��Viscosity Factor
		{
			curParticle->f.x += (-DEFAULT_VISCOSITY * curParticle->v.x);
			curParticle->f.y += (-DEFAULT_VISCOSITY * curParticle->v.y);
			curParticle->f.z += (-DEFAULT_VISCOSITY * curParticle->v.z);
		}
		curParticle++;
	}

	// CHECK IF THERE IS A USER FORCE BEING APPLIED
	if (UserForceActive)
	{
		if (Pick[0] != -1)
		{
			VectorSum(&system[Pick[0]].f,&UserForce,&system[Pick[0]].f);
		}
		if (Pick[1] != -1)
		{
			VectorSum(&system[Pick[1]].f,&UserForce,&system[Pick[1]].f);
		}
		MAKEVECTOR(UserForce,0.0f,0.0f,0.0f);	// CLEAR USER FORCE
	}

	// NOW DO ALL THE SPRINGS  �N�Ҧ�Spring Force�[�J
	spring = Spring;
	for (loop = 0; loop < SpringCnt; loop++)
	{
		p1 = &system[spring->p1];
		p2 = &system[spring->p2];
		VectorDifference(&p1->pos,&p2->pos,&deltaP);	// Vector distance 
		dist = (float)VectorLength(&deltaP);					// Magnitude of deltaP
		
		// TO DO
		/* 
		  Calculate Spring Force
										 Xa - Xb
		  fs = -Ks( | Xa - Xb | - r ) -------------
									   | Xa - Xb |
					^^^^^^^^^^^		  ^^^^^^^^^^^^^
					   dist				deltaP
			   ^^^^^^^^^^^^^^^^^^^^^^
			        Hterm

		*/
		/*
				   Xa - Xb
		deltaP = -----------  
				  |Xa - Xb|
		*/
		NormalizeVector(&deltaP);
		Hterm = - spring->Ks * ( dist - spring->restLen );

		/*
		  �[�JDamping Force 
		
					( Va - Vb ) (Xa - Xb )     Xa - Xb
		  fd = -Kd ------------------------ --------------
		               | Xa - Xb |			  |Xa - Xb|

			   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ~~~~~~~~~~~~~~
				         Dterm					deltaP
			   
		*/
		
		VectorDifference( &p1->v ,&p2->v ,&deltaV );//deltaV = Va - Vb
		Dterm = DotProduct( &deltaV ,&deltaP );
		Dterm = - spring->Kd * Dterm;

		springForce.x = ( Hterm + Dterm ) * deltaP.x;
		springForce.y = ( Hterm + Dterm ) * deltaP.y;
		springForce.z = ( Hterm + Dterm ) * deltaP.z;

		VectorSum(&p1->f,&springForce,&p1->f);			// Apply to Particle 1
		VectorDifference(&p2->f,&springForce,&p2->f);	// - Force on Particle 2
		spring++;					// DO THE NEXT SPRING
	}

	// APPLY THE MOUSE DRAG FORCES IF THEY ARE ACTIVE
	if (MouseForceActive)
	{
		// APPLY TO EACH PICKED PARTICLE
		if (Pick[0] > -1)
		{
			p1 = &system[Pick[0]];
			VectorDifference(&p1->pos,&MouseDragPos[0],&deltaP);	// Vector distance -->MP
			dist = VectorLength(&deltaP);					// Magnitude of deltaP

			if (dist != 0.0f)
			{
				// TO DO (Bonus)
				/*�[�J�Ϥ�V��Vector���ͤO*/
				VectorDifference( &p1->f ,&deltaP ,&p1->f);
			}
		}
		if (Pick[1] > -1)
		{
			p1 = &system[Pick[1]];
			VectorDifference(&p1->pos,&MouseDragPos[1],&deltaP);	// Vector distance 
			dist = VectorLength(&deltaP);					// Magnitude of deltaP

			if (dist != 0.0f)
			{
				// TO DO (Bonus)
				VectorDifference( &p1->f ,&deltaP ,&p1->f);
			}
		}
	}
}   

///////////////////////////////////////////////////////////////////////////////
// Function:	IntegrateSysOverTime 
// Purpose:		Does the Integration for all the points in a system
// Arguments:	Initial Position, Source and Target Particle Systems and Time
// Notes:		Computes a single integration step
///////////////////////////////////////////////////////////////////////////////
void CPhys::IntegrateSysOverTime(const tParticle *initial,const tParticle *source, tParticle *target, float deltaTime)
{
	/*
	     .
	     X = f(X ,t)
	    X(t+deltaH) = X(t) + deltaH * f(X ,t )
	*/
/// Local Variables ///////////////////////////////////////////////////////////
	int loop;
	float deltaTimeMass;
///////////////////////////////////////////////////////////////////////////////
	for (loop = 0; loop < ParticleCnt; loop++)
	{
		deltaTimeMass = deltaTime * initial->oneOverM;
		// DETERMINE THE NEW VELOCITY FOR THE PARTICLE  ��X�t��
		target->v.x = initial->v.x + (source->f.x * deltaTimeMass);
		target->v.y = initial->v.y + (source->f.y * deltaTimeMass);
		target->v.z = initial->v.z + (source->f.z * deltaTimeMass);

		target->oneOverM = initial->oneOverM;

		// SET THE NEW POSITION�@��X��m
		target->pos.x = initial->pos.x + (deltaTime * source->v.x);
		target->pos.y = initial->pos.y + (deltaTime * source->v.y);
		target->pos.z = initial->pos.z + (deltaTime * source->v.z);

		initial++;
		source++;
		target++;
	}
}

void CPhys::EulerIntegrate( float DeltaTime)
{
	// JUST TAKE A SINGLE STEP
	IntegrateSysOverTime(CurrentSys,CurrentSys, TargetSys,DeltaTime);
}
/*
  �p��Final State�Pinitial State���t�Z
  �Ъ`�N deltaX���i��u���@deltaX->pos�MdeltaX->v���ΦӤw
  �ǤJ�G��l���A  
  �ǥX�GdeltaX�����G
*/
void 
CPhys::StateDifference( const tParticle *initial,const tParticle *final, tParticle *deltaX ) {
	
	for ( int i = 0; i < ParticleCnt; i++ ) {
		VectorDifference( &final->v ,&initial->v ,&deltaX->v );
		VectorDifference( &final->pos ,&initial->pos ,&deltaX->pos );
		
		initial++;
		final++;
		deltaX++;
	}
}
/*
  ���initial �MdeltaX�� ����state
*/
void
CPhys::FindHalfStateFromInitial( const tParticle* initial ,const tParticle* deltaX ,tParticle* halfstate ) {
	
	for ( int i = 0; i < ParticleCnt; i++ ) {
		ScaleVector( &deltaX->pos ,0.5 ,&halfstate->pos );
		ScaleVector( &deltaX->v ,0.5 ,&halfstate->v );
		VectorSum( &initial->pos ,&halfstate->pos ,&halfstate->pos );
		VectorSum( &initial->v ,&halfstate->v ,&halfstate->v );
		
		deltaX++;
		halfstate++;
		initial++;
	}
}
void CPhys::MidPointIntegrate( float DeltaTime)
{
	// TO DO
	/*
	  deltaX = h * f( X(t0) )
						 deltaX
	  fmid = f( X(t0) + -------- )
						   2

      X(t+deltaH) = X(t) + h * fmid

	  ���i�H���Ψ������ �]��Euler�k��O�ε��t�e�i �]���u�n��DeltaTime /2���ɶ������A �A�N�i�H�o��
	  X(t0) + deltaX /2
	*/
	//1.���deltaX
	//  ���z�LEuler Method���Particle System���U�@�Ӫ��A �æs�bTempSys[0]��
	IntegrateSysOverTime( CurrentSys ,CurrentSys ,TempSys[0],DeltaTime );//�z�LEuler�k��U�@�Ӫ��A
	StateDifference( CurrentSys ,TempSys[0] ,TempSys[0] );//���M�ثe�t�Ϊ����A�t

	/*
	  2.���t�Ϊ��A���@�b����m�b����
				deltaX
	  X(t0) + ----------
	              2
    */
	FindHalfStateFromInitial(CurrentSys ,TempSys[0] ,TempSys[0] );//���Z���ثe�t�Τ@�b���a��

	/*�p��TempSys[0]���sForce f*/
	ComputeForces( TempSys[0] ,true );
	
	/*X(t+deltaH) = X(t) + h * fmid*/
	IntegrateSysOverTime(CurrentSys ,TempSys[0] ,TargetSys ,DeltaTime );
}

void CPhys::RK4Integrate( float DeltaTime)
{
	// TO DO
	/*
	  �аѦҧ�v��
	  K1 = h*f(x0 ,t0 )------------------�P�ثe���A�����A�t  TempSys[0]
	                 K1        h
	  K2 = h*f( x0+ ---- ,to+ --- )------�P�ثe���A�����A�t  TmpSys[1]
					  2        2
					 K2		   h
	  K3 = h*f( x0+ ---- ,to+ --- )------�P�ثe���A�����A�t  TempSys[2]
					  2        2
					 
	  K4 = h*f( x0+ K3 ,to+ h )----------�P�ثe���A�����A�t  TempSys[3]

	  X(t+deltaH) = X(t) + 1/6( K1+2K2+2K3+K4)
	*/
	tParticle* sFourceSys = CurrentSys;//��m���ܪ��ӷ� �]�N�O�O�O�ѭ��@�Өt�ΨӪ�
	for ( int i = 0; i < 3; i++ ) {//�p��K1~K3 �s�bTempSys[0]~TempSys[2]��
		//1.���Ki
		IntegrateSysOverTime( CurrentSys ,sFourceSys ,TempSys[i],DeltaTime );//�z�LEuler�k��CurrentSys + Ki
		StateDifference(CurrentSys ,TempSys[i] ,TempSys[i] );//���t�Ϊ��A�t �æs��TempSys[i]��
		/*
		 2.���t�Ϊ��A���@�b����m�b����
				       K1
		   X(t0) + ----------
	                    2
		*/
		FindHalfStateFromInitial(CurrentSys ,TempSys[i] ,TempSys[i+1] );//���@�b���A���a��æs�bTempSys[i+1]
		/*�p��TempSys[i+1]���sForce �H�K�U�@��Step�o�{Ki+1*/
		ComputeForces( TempSys[i+1] ,true );
		sFourceSys = TempSys[i+1];
	}
	//��W�p��K4
	IntegrateSysOverTime( CurrentSys ,sFourceSys ,TempSys[3] ,DeltaTime );
	StateDifference( CurrentSys ,TempSys[3] ,TempSys[3] );

	
	/*�p��̫᪺���AX(t+deltaH)*/
	tParticle* curParticle = CurrentSys;
	tParticle* targetParticle = TargetSys;
	tParticle* K1 = TempSys[0];
	tParticle* K2 = TempSys[1];
	tParticle* K3 = TempSys[2];
	tParticle* K4 = TempSys[3];
	float weight = (float)1 / 6;
	for ( int loop = 0; loop < ParticleCnt; loop++ ) {
		//��@�Ǥ��������i��Assign
		targetParticle->contactN.x = curParticle->contactN.x;
		targetParticle->contactN.y = curParticle->contactN.y;
		targetParticle->contactN.z = curParticle->contactN.z;
		targetParticle->contacting = curParticle->contacting;
		targetParticle->oneOverM = curParticle->oneOverM;
		
		//�{�b�nUpdate ��m �Фp�ߤ@�I��!!
		//X(t+deltaH) = X(t) + 1/6( K1+2K2+2K3+K4)
		targetParticle->pos.x = curParticle->pos.x;
		targetParticle->pos.y = curParticle->pos.y;
		targetParticle->pos.z = curParticle->pos.z;//X(t)
		ScaleVector( &K1->pos ,weight ,&K1->pos );
		ScaleVector( &K2->pos ,2*weight ,&K2->pos );
		ScaleVector( &K3->pos ,2*weight ,&K3->pos );
		ScaleVector( &K4->pos ,weight ,&K4->pos );
		VectorSum( &targetParticle->pos ,&K1->pos ,&targetParticle->pos);
		VectorSum( &targetParticle->pos ,&K2->pos ,&targetParticle->pos);
		VectorSum( &targetParticle->pos ,&K3->pos ,&targetParticle->pos);
		VectorSum( &targetParticle->pos ,&K4->pos ,&targetParticle->pos);//Position Update����
		targetParticle->v.x = curParticle->v.x;
		targetParticle->v.y = curParticle->v.y;
		targetParticle->v.z = curParticle->v.z;//V(t)
		ScaleVector( &K1->v ,weight ,&K1->v );
		ScaleVector( &K2->v ,2*weight ,&K2->v );
		ScaleVector( &K3->v ,2*weight ,&K3->v );
		ScaleVector( &K4->v ,weight ,&K4->v );
		VectorSum( &targetParticle->v ,&K1->v ,&targetParticle->v);
		VectorSum( &targetParticle->v ,&K2->v ,&targetParticle->v);
		VectorSum( &targetParticle->v ,&K3->v ,&targetParticle->v);
		VectorSum( &targetParticle->v ,&K4->v ,&targetParticle->v);


		//����
		K1++;
		K2++;
		K3++;
		K4++;
		curParticle++;
		targetParticle++;
	}

}

int CPhys::CheckForCollisions( tParticle	*system )
{
    // TO DO
	tParticle* curParticle = system;
	
	/*
	  �ˬd�O�_Penetration
	*/
	for ( int loop = 0; loop < ParticleCnt; loop++ ) {
		tCollisionPlane* plane = CollisionPlane;
		for ( int i = 0; i < CollisionPlaneCnt; i++ ) {
			if ( DotProduct( &plane->normal ,&curParticle->pos ) + plane->d < 0 )
				return PENETRATING;
			
			plane++;
		}
		curParticle++;
	}
	/*
	 �I���ǫh
	 1.N * ( x - p ) < e  ===>ax+by+cz+d < e
	 2.N * v < 0
	*/
	curParticle = system;
	
	int iscollide = NOT_COLLIDING;
	for ( int loop = 0; loop < ParticleCnt; loop++ ) {
		/*��C�@�ӸI���������p��*/
		tCollisionPlane* plane = CollisionPlane;
		for ( int i = 0; i < CollisionPlaneCnt; i++ ) {

			double NdotV = DotProduct( &plane->normal ,&curParticle->v );
			double dist = DotProduct( &plane->normal ,&curParticle->pos ) + plane->d;
			if (  NdotV < 0 && (dist - 0.1) <= 0 ) {

				curParticle->contacting = true;
				curParticle->contactN.x = plane->normal.x;
				curParticle->contactN.y = plane->normal.y;
				curParticle->contactN.z = plane->normal.z;
				iscollide = COLLIDING;//�p�G�o�͸I�� �h�|�C�����|�B�z�L
			}

			plane++;
		}
		curParticle++;
	}
	
	return iscollide;
}

void CPhys::ResolveCollisions( tParticle	*system )
{
	// TO DO
	tParticle* curParticle = system;

	for ( int loop = 0; loop < ParticleCnt; loop++ ) {
		if ( curParticle->contacting ) {
			/*
			  �p�ҥ�P.261 Fig 7.15��

			  V(t) = V(t) - (1+k) V(t)N
			*/
			double vlen = VectorLength( &curParticle->v ); 
			double VdotN = DotProduct( &curParticle->v ,&curParticle->contactN );
			tVector Vn;
			ScaleVector( &curParticle->contactN ,VdotN ,&Vn );
			ScaleVector( &Vn ,1+Kr ,&Vn );
			VectorDifference( &curParticle->v ,&Vn ,&curParticle->v );
			
			
		}

		curParticle++;
	}
}

void CPhys::Simulate(float DeltaTime, BOOL running)
{
   float		CurrentTime = 0.0f;
    float		TargetTime = DeltaTime;
	tParticle	*tempSys;
	int			collisionState;

    while(CurrentTime < DeltaTime)
    {
		if (running)
		{
			ComputeForces(CurrentSys, FALSE);//�ھڥثe�t�έp��O

			if (CollisionRootFinding)
				EulerIntegrate(TargetTime-CurrentTime);
			else
			{
				switch (IntegratorType)
				{
				case EULER_INTEGRATOR:
					EulerIntegrate(TargetTime-CurrentTime);
					break;
				case MIDPOINT_INTEGRATOR:
					MidPointIntegrate(TargetTime-CurrentTime);
					break;
				case RK4_INTEGRATOR:
					RK4Integrate(TargetTime-CurrentTime);
					break;
				}
			}
		}

		collisionState = CheckForCollisions(TargetSys);//Tartget System�O�_���o�͸I���@

        if(collisionState == PENETRATING)
        {
			/*�p�G��penetration �ڭ̥i�H���e���p�@�I ���L���n����penetration*/
			// TELL THE SYSTEM I AM LOOKING FOR A COLLISION SO IT WILL USE EULER
			CollisionRootFinding = TRUE;
            // we simulated too far, so subdivide time and try again
            TargetTime = (CurrentTime + TargetTime) / 2.0f;

			/*�@�}�l�NPenetration�� �N����*/
            // blow up if we aren't moving forward each step, which is
            // probably caused by interpenetration at the frame start
            assert(fabs(TargetTime - CurrentTime) > EPSILON);
        }
        else
        {
            // either colliding or clear
            if(collisionState == COLLIDING)
            {
                int Counter = 0;
                do
                {
                    ResolveCollisions(TargetSys);
                    Counter++;
                } while((CheckForCollisions(TargetSys) ==
                            COLLIDING) && (Counter < 100));

                assert(Counter < 100);
				CollisionRootFinding = FALSE;	// FOUND THE COLLISION POINT BACK TO NORMAL
            }

            // we made a successful step, so swap configurations
            // to "save" the data for the next step
            
			CurrentTime = TargetTime;
			TargetTime = DeltaTime;

			// SWAP MY TWO PARTICLE SYSTEM BUFFERS SO I CAN DO IT AGAIN�@�NTarget System�����̷s��Particle System
			tempSys = CurrentSys;
			CurrentSys = TargetSys;
			TargetSys = tempSys;
			
        }
    }
}
