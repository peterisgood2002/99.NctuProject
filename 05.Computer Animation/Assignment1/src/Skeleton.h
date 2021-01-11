
/***************************************************************************
   NAME
     Skeleton.h
   PURPOSE
     定義了跟Skeleton相關的data structure
   NOTES
      
   AUTHOR
    
   HISTORY
    
***************************************************************************/

#if !defined(SKELETON_H__INCLUDED_)
#define SKELETON_H__INCLUDED_

#define ushort	unsigned short
/// Bone Definitions /////////////////////////////////////////////////////////
#define BONE_ID_ROOT				1		// ROOT BONE
///////////////////////////////////////////////////////////////////////////////

/// Channel Definitions ///////////////////////////////////////////////////////
#define CHANNEL_TYPE_NONE			0		// NO CHANNEL APPLIED
#define CHANNEL_TYPE_SRT			1		// SCALE ROTATION AND TRANSLATION
#define CHANNEL_TYPE_TRANS			2		// CHANNEL HAS TRANSLATION (X Y Z) ORDER
#define CHANNEL_TYPE_RXYZ			4		// ROTATION (RX RY RZ) ORDER
#define CHANNEL_TYPE_RZXY			8		// ROTATION (RZ RX RY) ORDER
#define CHANNEL_TYPE_RYZX			16		// ROTATION (RY RZ RX) ORDER
#define CHANNEL_TYPE_RZYX			32		// ROTATION (RZ RY RX) ORDER
#define CHANNEL_TYPE_RXZY			64		// ROTATION (RX RZ RY) ORDER
#define CHANNEL_TYPE_RYXZ			128		// ROTATION (RY RX RZ) ORDER
#define CHANNEL_TYPE_S				256		// SCALE ONLY
#define CHANNEL_TYPE_T				512		// TRANSLATION ONLY (X Y Z) ORDER
#define CHANNEL_TYPE_INTERLEAVED	1024	// THIS DATA STREAM HAS MULTIPLE CHANNELS
///////////////////////////////////////////////////////////////////////////////

// COUNT OF NUMBER OF FLOATS FOR EACH CHANNEL TYPE
static int s_Channel_Type_Size[] = 
{
	0,
	9,
	6,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3
};

#include "MathDefs.h"		// GET THE TYPE FOR QUATERNION

/// Structure Definitions ///////////////////////////////////////////////////////
/*Object的相關資料*/
struct t_Visual
{
	int		dataFormat;     //資料是屬於 有無Normal或有無Texture coordinate
	float	*vertexData;	// INTERLEAVED VERTEX DATA IN DATAFORMAT
	long	vertexCnt;		// NUMBER OF VERTICES IN VISUAL
	BOOL	reuseVertices;	// DO I WANT TO USED INDEXED ARRAYS
	ushort	*faceIndex;		// INDEXED VERTEX DATA IF VERTICES ARE REUSED
	int		vSize;			// NUMBER OF FLOATS IN A VERTEX    一個Vertex的資料大小 可能儲存著Vertex ,Normal ,Texture Coordinate
	long	faceCnt;		// NUMBER OF FACES IN VISUAL
	tVector *faceNormal;	// POINTER TO FACE NORMALS
	long	vPerFace;		// VERTICES PER FACE, EITHER 3 OR 4
	tVector Ka,Kd,Ks;		// COLOR FOR OBJECT
	float	Ns;				// SPECULAR COEFFICIENT
	char    map[255];		// TEXTURE MAP
	int		glTex;
	tVector bbox[8];		// BBOX COORDS
	tVector transBBox[8];	
};

// THIS STRUCTURE DEFINES A BONE IN THE ANIMATION SYSTEM
// A BONE IS ACTUALLY AN ABSTRACT REPRESENTATION OF A OBJECT
// IN THE 3D WORLD.  A CHARACTER COULD BE MADE OF ONE BONE
// WITH MULTIPLE VISUALS OF ANIMATION ATTACHED.  THIS WOULD
// BE SIMILAR TO A QUAKE CHARACTER.  BY MAKING IT HAVE LEVELS
// OF HIERARCHY AND CHANNELS OF ANIMATION IT IS JUST MORE FLEXIBLE
/*骨架系統 動畫用*/
struct t_Bone
{
	long	id;							// BONE ID
	char	name[80];					// BONE NAME
	long	flags;						// BONE FLAGS
	// HIERARCHY INFO
	t_Bone	*parent;					// POINTER TO PARENT BONE
	int 	childCnt;					// COUNT OF CHILD BONES
	t_Bone	*children;					// POINTER TO CHILDREN
	// TRANSFORMATION INFO
	tVector	b_scale;					// BASE SCALE FACTORS---------放大縮小
	tVector	b_rot;						// BASE ROTATION FACTORS------旋轉
	tVector	b_trans;					// BASE TRANSLATION FACTORS---平移
	tVector	scale;						// CURRENT SCALE FACTORS------目前的
	tVector	rot;						// CURRENT ROTATION FACTORS
	tVector	trans;						// CURRENT TRANSLATION FACTORS
	tQuaternion quat;					// QUATERNION USEFUL FOR ANIMATION
	tMatrix matrix;						// PLACE TO STORE THE MATRIX

	// ANIMATION INFO
	long	primChanType;				// WHAT TYPE OF PREIMARY CHAN IS ATTACHED
	float	*primChannel;				// POINTER TO PRIMARY CHANNEL OF ANIMATION
	float 	primFrameCount;				// FRAMES IN PRIMARY CHANNEL
	float	primSpeed;					// CURRENT PLAYBACK SPEED
	float	primCurFrame;				// CURRENT FRAME NUMBER IN CHANNEL
	long	secChanType;				// WHAT TYPE OF SECONDARY CHAN IS ATTACHED
	float	*secChannel;				// POINTER TO SECONDARY CHANNEL OF ANIMATION
	float	secFrameCount;				// FRAMES IN SECONDARY CHANNEL
	float	secCurFrame;				// CURRENT FRAME NUMBER IN CHANNEL
	float	secSpeed;					// CURRENT PLAYBACK SPEED
	float	animBlend;					// BLENDING FACTOR (ANIM WEIGHTING)
	// DOF CONSTRAINTS
	int		min_rx, max_rx;				// ROTATION X LIMITS
	int		min_ry, max_ry;				// ROTATION Y LIMITS
	int		min_rz, max_rz;				// ROTATION Z LIMITS
	float	damp_width, damp_strength;	// DAMPENING SETTINGS
	// VISUAL ELEMENTS
	int		visualCnt;					// COUNT OF ATTACHED VISUAL ELEMENTS
	t_Visual	*visuals;					// POINTER TO VISUALS/BITMAPS
	int		*CV_ptr;					// POINTER TO CONTROL VERTICES
	float	*CV_weight;					// POINTER TO ARRAY OF WEIGHTING VALUES
	// COLLISION ELEMENTS
	float	bbox[6];					// BOUNDING BOX (UL XYZ, LR XYZ)
	tVector	center;						// CENTER OF OBJECT (MASS)
	float	bsphere;					// BOUNDING SPHERE (RADIUS)  
	// PHYSICS
	tVector	length;						// BONE LENGTH VECTOR
	float	mass;						// MASS
	float	elast;						// ELASTICITY
};

///////////////////////////////////////////////////////////////////////////////

/// Support Function Definitions //////////////////////////////////////////////

void DestroySkeleton(t_Bone *root);
void ResetBone(t_Bone *bone,t_Bone *parent);
void BoneSetFrame(t_Bone *bone,int frame);
void BoneAdvanceFrame(t_Bone *bone,int direction,BOOL doChildren);

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(SKELETON_H__INCLUDED_)
