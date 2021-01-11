/***************************************************************************
   NAME
     Skeleton.h
   PURPOSE
     �ŧi�FSkeleton�Q�O�ΨӪ�ܤ@��AVATAR
	 
   NOTES
	 ���� �C�@��Bone(Link)����T�|�Q�w�q�bBone��
	 ��Skeleton�t�dŪ��ASF��Ʋ��ͤ@��AVATAR
   AUTHOR
     
   HISTORY
     Zhi-Chebg Yan - Match 3, 2010: Modified.
***************************************************************************/

#ifndef _SKELETON_H
#define _SKELETON_H

#include "Posture.h"

// Bone segment names used in ASF file
static int root = 0;

// this structure defines the property of each bone segment, including its connection to other bones,
// DOF (degrees of freedom), relative orientation and distance to the outboard bone 
struct Bone {
   
	struct Bone *sibling;		// Pointer to the sibling (branch bone) in the hierarchy tree 
	struct Bone *child;			// Pointer to the child (outboard bone) in the hierarchy tree 
   
	int idx;					// Bone index �bBoneArray����Bone�O�ĴX��Bone
	
	double dir[3];				// Unit vector describes the direction from local origin to 
								// the origin of the child bone �q����U�@��Bone���¦V
								// Notice: stored in local coordinate system of the bone  

	double length;				// Bone length  

	double axis_x, axis_y, axis_z;// orientation of each bone's local coordinate 
								 //system as specified in ASF file (axis field)

	double aspx, aspy;			// aspect ratio of bone shape

	int dof;					// number of bone's degrees of freedom ���h�֦ۥѫ�
	int dofx, dofy, dofz;		// degree of freedom mask in x, y, z axis (local)  �p�G�b�Ӷb�W������DOF�h��1�_�h��0 
	int doftx, dofty, doftz;//��ܾ֦��i�HTranslation��DOF
	int doftl;
								// dofx=1 if this bone has x degree of freedom, otherwise dofx=0.
	
	char name[256];
	// rotation matrix from the local coordinate of this bone to the local coordinate system of it's parent
	double rot_parent_current[4][4];			
	
	//�qAMCŪ�i�Ӫ����
	//Rotation angles for this bone at a particular time frame (as read from AMC file) in local coordinate system, 
	//they are set in the setPosture function before dispay function is called
	double drx, dry, drz;//��x y z�b������q
	double tx,ty,tz;
	double tl;
	int dofo[8];//?
};


class Skeleton {

  //Member functions
  public: 

	// The scale parameter adjusts the size of the skeleton. The default value is 0.06 (MOCAP_SCALE).
    // This creates a human skeleton of 1.7 m in height (approximately)
    Skeleton(string asf_filename, float scale = 0.06 );  
    ~Skeleton();                                

    //Get root node's address; for accessing bone data
    Bone* getRoot();

	//Set the skeleton's pose based on the given posture    
	void setPosture(Posture posture);        

	//Initial posture Root at (0,0,0)
	//All bone rotations are set to 0
    void setBasePosture();
	  

  private:

	//parse the skeleton (.ASF) file	
    void readASFfile(string asf_filename, float scale);

  public:
	//This recursive function traverces skeleton hierarchy 
	//and returns a pointer to the bone with index - bIndex
	//ptr should be a pointer to the root node 
	//when this function first called
	Bone* getBone(Bone *ptr, int bIndex);
      
  private:
	//This function sets sibling or child for parent bone
	//If parent bone does not have a child, 
	//then pChild is set as parent's child
	//else pChild is set as a sibling of parents already existing child
	int setChildrenAndSibling(int parent, Bone *pChild);

	//Rotate all bone's direction vector (dir) from global to local coordinate system
	//�N�Ҧ���Bone Direction���নLocal
	void RotateBoneDirToLocalCoordSystem();

  //Member Variables
  public:
	// root position in world coordinate system  �@�ɮy�Шt��Root��m
    double m_RootPos[3];
	int tx,ty,tz;
	int rx,ry,rz;
    // number of DOF of the skeleton
	//DEBUG: remove this variable???
//    int m_NumDOFs;
	int name2idx(const char *);
	char * idx2name(int);
	int NUM_BONES_IN_ASF_FILE;//��Skeleton Bone���ƶq���X��
	int MOV_BONES_IN_ASF_FILE;//�i���ʪ�Bone���X��
  private:
	Bone *m_pRootBone;		   // Pointer to the root bone, m_RootBone = &bone[0]  ����Root
	Bone  m_pBoneList[256];   // Array with all skeleton bones �x�s�Ҧ������Y

};

int numBonesInSkel(Bone item);
int movBonesInSkel(Bone item);

#endif
