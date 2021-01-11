/***************************************************************************
   NAME
     Motion.h
   PURPOSE
     宣告了Motion類別 用來讀AMC用

   NOTES
     
   AUTHOR
     
   HISTORY
     Zhi-Chebg Yan - Match 3, 2010: Modified.
***************************************************************************/

#ifndef MOTION_H
#define MOTION_H

//#include "Motion_Loader.h"
#include "Skeleton.h"

/*class Motion
{
public:
	Motion();
	Motion(string name);
	~Motion() {delete root;};

	static Motion* Import(fstream &inFile);				// load mutiple motion files
	bool LoadASF(string name);							// load single one motion file
	bool LoadAMC(string name);							// load single one motion file

	void Init();										// set Timer, set DisplayList, add some data to buffer
	void Draw();

	void BuildIKtree(Posture& p);
	void IK();

private:
	// for load and write function, should be ignored
	void AddFrameData(ifstream &inFile, BodyPart *node, int index);
	bool GenerateSkelton(ifstream &inFile, BodyPart *node, BodyPart *prevNode);
	// for draw function, should be ignored
	static void SetSystemTimer();
	

public:
	// information about the raw data
	vector<Posture> vecPose;							// store the raw data of motions
	// information about the synthesized results
	vector<Posture> vecBuffer;							// store the ready poses for playing

	// control parameters
	bool loop;											// if the status is true, the motion playing will be repeated
	bool active;										// if the status is false, the motion playing will be paused

private:
	BodyPart* root;										// the pointer of the root joint
	int iNumFrames;										// the total frames of all motion files
	int iNumJoints;										// the total number of joints for the skeleton
};*/

class Motion 
{
	//member functions 
    public:
		//Include Actor (skeleton) ptr
		Motion(string amc_filename, float scale,Skeleton * pActor);
		//Use to creating motion from AMC file
		Motion(string amc_filename, float scale);
		//Use to create default motion with specified number of frames
		Motion(int nFrameNum);
		//delete motion
       ~Motion();

       // scale is a parameter to adjust the translational parameter
       // This value should be consistent with the scale parameter used in Skeleton()
       // The default value is 0.06
       int LoadAMC(string name, float scale);

		/*
		  做IK要用到函式 
		  BuildIKTree是FK找到Posture在世界座標系的位置 並儲存在Posture中
		*/
		void BuildIKtree(Posture& p ,const Bone* b);
		void IK();
		void Draw();//畫IK的點以防止出問題
		void DrawNextPosture();

	   //Set all postures to default posture
	   //Root position at (0,0,0), orientation of each bone to (0,0,0)
	   void SetPosturesToDefault();

	   //Set posture at spesified frame
	   void SetPosture(int nFrameNum, Posture InPosture);
		int GetPostureNum(int nFrameNum);
		void SetTimeOffset(int n_offset);
	   Posture* GetPosture(int nFrameNum);
	   void SetBoneRotation(int nFrameNum, Vector3 vRot, int nBone);
	   void SetRootPos(int nFrameNum, Vector3 vPos);

	//data members
public:
    int m_NumFrames; //Number of frames in the motion 
	int offset;

	//int m_NumDOFs;	//Overall number of degrees of freedom (summation of degrees of freedom for all bones)
	Skeleton * pActor;
	//Root position and all bone rotation angles for each frame (as read from AMC file)
	Posture* m_pPostures; 

	// control parameters
	bool loop;											// if the status is true, the motion playing will be repeated
	bool active;										// if the status is false, the motion playing will be paused

	bool isIK;
	int draw_Posture;
};

#endif