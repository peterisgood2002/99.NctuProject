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
/*由Normal組成*/
struct tCollisionPlane
{
	tVector normal;			// inward pointing
    float	d;				// ax + by + cz + d = 0
};

// TYPE FOR A PHYSICAL PARTICLE IN THE SYSTEM
struct tParticle
{
	tVector pos;		// Position of Particle 位置
    tVector v;			// Velocity of Particle 速度
	tVector f;			// Force Acting on Particle 力
	float	oneOverM;	// 1 / Mass of Particle 質量
	tVector contactN;	// Normal of Contact
	BOOL	contacting;	//有無接觸
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
	BOOL				DrawSprings;			// DRAW THE SPRING LINES  是否要畫上Spring用
	BOOL				DrawVertices;			// DRAW VERTICES  是否要畫上點用
	BOOL				MouseForceActive;		// MOUSE DRAG FORCE 滑鼠拖的力
	BOOL				CollisionRootFinding;	// TRYING TO FIND A COLLISION
	int					IntegratorType;      //積分型態是甚麼

// Attributes
private:
	float				WorldSizeX,WorldSizeY,WorldSizeZ;
	tVector				Gravity;				// GRAVITY FORCE VECTOR  重力場
	tVector				UserForce;			// USER FORCE VECTOR
	float				UserForceMag;			// MAGNITUDE OF USER FORCE  使用者的力
	float				Kd;					// VISCOSITY FACTOR
	float				Kr;					// COEFFICIENT OF RESTITUTION
	float				Ksh;					// HOOK'S SPRING CONSTANT
	float				Ksd;					// SPRING DAMPING
	float				Csf, Ckf;			// Static and Kinetic Particle
	float				MouseForceKs;			// MOUSE SPRING COEFFICIENT
	tCollisionPlane		*CollisionPlane;		// LIST OF COLLISION PLANES  碰撞平面
	int					CollisionPlaneCnt;			
	tContact			*Contact;				// LIST OF POSSIBLE COLLISIONS
	int					ContactCnt;			// COLLISION COUNT
	/*以下幾個指標會在SetWorldParticles() 初始化*/
	/*ParticleSys[2]：初始的系統　ParticleSys[0]：目前狀態 ParticleSys[1]：下一個系統狀態*/
	//tParticle			*ParticleSys[3];		// LIST OF PHYSICAL PARTICLES  
	tParticle			*CurrentSys,*TargetSys ,*InitSys; //CurrentSys為要畫出的系統
	tParticle			*TempSys[5];			// SETUP FOR TEMP PARTICLES USED WHILE INTEGRATING 積分要用的
	int					ParticleCnt;
	tSpring				*Spring;				// VALID SPRINGS IN SYSTEM
	int					SpringCnt;		
	int					Pick[2];				// INDEX COUNTERS FOR SELECTING 選到的那一個Particle index 
	tVector				MouseDragPos[2];		// POSITION OF DRAGGED MOUSE VECTOR　　滑鼠拉的位置　用來指定滑鼠拉的方向與利
// Operations
private:
	//source為力的來源
	void	IntegrateSysOverTime(const tParticle *initial,const tParticle *source, tParticle *target, float deltaTime);
	/*
	  使用Euler Method 找到下一個狀態和這一個狀態的改變量
	  請注意 deltaX中可能只有　deltaX->pos和deltaX->v有用而已
	  傳入：初始狀態 要改變的一些設定 
	  傳出：deltaX的結果
	*/
	void    StateDifference( const tParticle *initial,const tParticle *final, tParticle *deltaX  );
	/*
	  找到initial 和deltaX 的 中間state
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
