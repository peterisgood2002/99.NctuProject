/***************************************************************************
   NAME
     Skeleton.h
   PURPOSE
     宣告了Skeleton被別用來表示一個AVATAR
	 
   NOTES
	 首先 每一個Bone(Link)的資訊會被定義在Bone中
	 而Skeleton負責讀取ASF資料產生一個AVATAR
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
   
	int idx;					// Bone index 在BoneArray中此Bone是第幾個Bone
	
	double dir[3];				// Unit vector describes the direction from local origin to 
								// the origin of the child bone 從此到下一個Bone的朝向
								// Notice: stored in local coordinate system of the bone  

	double length;				// Bone length  

	double axis_x, axis_y, axis_z;// orientation of each bone's local coordinate 
								 //system as specified in ASF file (axis field)

	double aspx, aspy;			// aspect ratio of bone shape

	int dof;					// number of bone's degrees of freedom 有多少自由度
	int dofx, dofy, dofz;		// degree of freedom mask in x, y, z axis (local)  如果在該軸上有旋轉DOF則為1否則為0 
	int doftx, dofty, doftz;//表示擁有可以Translation的DOF
	int doftl;
								// dofx=1 if this bone has x degree of freedom, otherwise dofx=0.
	
	char name[256];
	// rotation matrix from the local coordinate of this bone to the local coordinate system of it's parent
	double rot_parent_current[4][4];			
	
	//從AMC讀進來的資料
	//Rotation angles for this bone at a particular time frame (as read from AMC file) in local coordinate system, 
	//they are set in the setPosture function before dispay function is called
	double drx, dry, drz;//對x y z軸之旋轉量
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
	//將所有的Bone Direction都轉成Local
	void RotateBoneDirToLocalCoordSystem();

  //Member Variables
  public:
	// root position in world coordinate system  世界座標系的Root位置
    double m_RootPos[3];
	int tx,ty,tz;
	int rx,ry,rz;
    // number of DOF of the skeleton
	//DEBUG: remove this variable???
//    int m_NumDOFs;
	int name2idx(const char *);
	char * idx2name(int);
	int NUM_BONES_IN_ASF_FILE;//此Skeleton Bone的數量有幾個
	int MOV_BONES_IN_ASF_FILE;//可移動的Bone有幾個
  private:
	Bone *m_pRootBone;		   // Pointer to the root bone, m_RootBone = &bone[0]  指到Root
	Bone  m_pBoneList[256];   // Array with all skeleton bones 儲存所有的骨頭

};

int numBonesInSkel(Bone item);
int movBonesInSkel(Bone item);

#endif
