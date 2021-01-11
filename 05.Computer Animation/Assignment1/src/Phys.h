#if !defined(AFX_PhysEnv_H__3DC11AC3_95FB_11D2_9D83_00105A124906__INCLUDED_)
#define AFX_PhysEnv_H__3DC11AC3_95FB_11D2_9D83_00105A124906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhysEnv.h : header file
//
#include "MathDefs.h"

#define EPSILON  0.00001f				// ERROR TERM
#define DEFAULT_VISCOSITY		0.002f

enum tCollisionTypes
{
	NOT_COLLIDING,
	PENETRATING,
	COLLIDING
};

enum tIntegratorTypes
{
	EULER_INTEGRATOR,
	MIDPOINT_INTEGRATOR,
	RK4_INTEGRATOR
};


// TYPE FOR A PLANE THAT THE SYSTEM WILL COLLIDE WITH
/*��Normal�զ�*/
struct tCollisionPlane
{
	tVector normal;			// inward pointing
    float	d;				// ax + by + cz + d = 0
};

// TYPE FOR A PHYSICAL PARTICLE IN THE SYSTEM
struct tParticle
{
	tVector pos;		// Position of Particle ��m
    tVector v;			// Velocity of Particle �t��
	tVector f;			// Force Acting on Particle �O
	float	oneOverM;	// 1 / Mass of Particle ��q
	tVector contactN;	// Normal of Contact
	BOOL	contacting;	//���L��Ĳ
};

// TYPE FOR CONTACTS THAT ARE FOUND DURING SIM
struct tContact
{
	int		particle;	// Particle Index
    tVector normal;		// Normal of Collision plane
	int		type;		// COLLIDING OR CONTACT
};

// TYPE FOR SPRINGS IN SYSTEM
struct tSpring
{
	int		p1,p2;		// PARTICLE INDEX FOR ENDS
	float	restLen;	// LENGTH OF SPRING AT REST
	float	Ks;			// SPRING CONSTANT
	float	Kd;			// SPRING DAMPING
};

class CPhys
{
// Construction
public:
	CPhys();
	void RenderWorld();
	void SetWorldParticles(tVector *coords,int particleCnt);
	void ResetWorld();
	void Simulate(float DeltaTime,BOOL running);
	void ApplyUserForce(tVector *force);
	void SetMouseForce(int deltaX,int deltaY, tVector *localX, tVector *localY);
	void GetNearestP(int x, int y);
	void AddSpring();
	void SetVertexMass();
	void SetWorldProperties();
	void FreeSystem();
	void LoadData(FILE *fp);
	void SaveData(FILE *fp);
	BOOL				UseGravity;			// SHOULD GRAVITY BE ADDED IN
	BOOL				UseViscosity;			// SHOULD Viscosity BE ON
	BOOL				UserForceActive;		// WHEN USER FORCE IS APPLIED
	BOOL				DrawSprings;			// DRAW THE SPRING LINES  �O�_�n�e�WSpring��
	BOOL				DrawVertices;			// DRAW VERTICES  �O�_�n�e�W�I��
	BOOL				MouseForceActive;		// MOUSE DRAG FORCE �ƹ��쪺�O
	BOOL				CollisionRootFinding;	// TRYING TO FIND A COLLISION
	int					IntegratorType;      //�n�����A�O�ƻ�

// Attributes
private:
	float				WorldSizeX,WorldSizeY,WorldSizeZ;
	tVector				Gravity;				// GRAVITY FORCE VECTOR  ���O��
	tVector				UserForce;			// USER FORCE VECTOR
	float				UserForceMag;			// MAGNITUDE OF USER FORCE  �ϥΪ̪��O
	float				Kd;					// VISCOSITY FACTOR
	float				Kr;					// COEFFICIENT OF RESTITUTION
	float				Ksh;					// HOOK'S SPRING CONSTANT
	float				Ksd;					// SPRING DAMPING
	float				Csf, Ckf;			// Static and Kinetic Particle
	float				MouseForceKs;			// MOUSE SPRING COEFFICIENT
	tCollisionPlane		*CollisionPlane;		// LIST OF COLLISION PLANES  �I������
	int					CollisionPlaneCnt;			
	tContact			*Contact;				// LIST OF POSSIBLE COLLISIONS
	int					ContactCnt;			// COLLISION COUNT
	/*�H�U�X�ӫ��з|�bSetWorldParticles() ��l��*/
	/*ParticleSys[2]�G��l���t�Ρ@ParticleSys[0]�G�ثe���A ParticleSys[1]�G�U�@�Өt�Ϊ��A*/
	//tParticle			*ParticleSys[3];		// LIST OF PHYSICAL PARTICLES  
	tParticle			*CurrentSys,*TargetSys ,*InitSys; //CurrentSys���n�e�X���t��
	tParticle			*TempSys[5];			// SETUP FOR TEMP PARTICLES USED WHILE INTEGRATING �n���n�Ϊ�
	int					ParticleCnt;
	tSpring				*Spring;				// VALID SPRINGS IN SYSTEM
	int					SpringCnt;		
	int					Pick[2];				// INDEX COUNTERS FOR SELECTING ��쪺���@��Particle index 
	tVector				MouseDragPos[2];		// POSITION OF DRAGGED MOUSE VECTOR�@�@�ƹ��Ԫ���m�@�Ψӫ��w�ƹ��Ԫ���V�P�Q
// Operations
private:
	//source���O���ӷ�
	void	IntegrateSysOverTime(const tParticle *initial,const tParticle *source, tParticle *target, float deltaTime);
	/*
	  �ϥ�Euler Method ���U�@�Ӫ��A�M�o�@�Ӫ��A�����ܶq
	  �Ъ`�N deltaX���i��u���@deltaX->pos�MdeltaX->v���ΦӤw
	  �ǤJ�G��l���A �n���ܪ��@�ǳ]�w 
	  �ǥX�GdeltaX�����G
	*/
	void    StateDifference( const tParticle *initial,const tParticle *final, tParticle *deltaX  );
	/*
	  ���initial �MdeltaX �� ����state
	*/
	void    FindHalfStateFromInitial( const tParticle* initial ,const tParticle* deltaX ,tParticle* halfstate );
	void	RK4Integrate( float DeltaTime);
	void	MidPointIntegrate( float DeltaTime);
	void	EulerIntegrate( float DeltaTime);
	void	ComputeForces( tParticle	*system, BOOL duringIntegration );
	int		CheckForCollisions( tParticle	*system );
	void	ResolveCollisions( tParticle	*system );
	void	CompareDepthBuffer(int size, float *buffer,float x, float y);

// Implementation
public:
	virtual ~CPhys();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PhysEnv_H__3DC11AC3_95FB_11D2_9D83_00105A124906__INCLUDED_)
