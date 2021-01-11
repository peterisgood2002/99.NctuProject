#ifndef MOTION_LOADER_H
#define MOTION_LOADER_H

#include "Posture.h"

#define X 0
#define Y 1
#define Z 2

// A BodyPrat represent a bone in the skeleton
// The skeleton is represented using a tree structure
class BodyPart
{
public:
	BodyPart();
	~BodyPart();

	// accessor
	inline void SetName(string n){name = n;}
	inline string GetName(){return name;}
	inline void SetID(int n){ID = n;}
	inline int GetID(){return ID;}
	inline int GetListNum(){return ListNum;}		// for display list

	inline void SetLinkForPrevBodyPart(BodyPart *ptr){prevBodyPart = ptr;}
	inline void AddLinkToNextBodyPart(BodyPart *ptr){nextBodyParts.push_back(ptr);}
	inline void DelLinkToNextBodyPart(int index){nextBodyParts.erase(nextBodyParts.begin()+index);}
	inline void SetLinkToNextBodyPart(int index, BodyPart *ptr){nextBodyParts[index] = ptr;}
	inline int NumLinks(){return nextBodyParts.size();}
	inline BodyPart* GetPrevBodyPart(){return prevBodyPart;}
	inline BodyPart* GetNextBodyPart(int index){return nextBodyParts.at(index);}
	inline vector<BodyPart*> GetNextBodyPart(){return nextBodyParts;}
	BodyPart* SearchBodyPart(string name);		// find the bone for a specific name
	BodyPart* SearchBodyPart(int ID);
	int GetBodyPartID(string name);
	int GetBodyPartID(BodyPart*);
	void DelLinkToBodyPart(BodyPart*);
	void DelLinkToBodyPart(int ID);
	void DelLinkToBodyPart(string name);

	// Joint
	void SetLocalOffset(double offsets[3]);
	inline double* GetLocalOffset(){return localOffset;}
	void SetGlobalOffset(double offsets[3]);
	inline double* GetGlobalOffset(){return globalOffset;}
	void SetOrientation(double xRot, double yRot, double zRot);
	inline double* GetOrientation(){return orientation;}
	inline double GetRadius(){return sphereRadius;}
	// Bone
	inline double GetLength(){return length;}
	inline double GetWidth(){return width;}
	// Motion
	inline double GetSamplingRate(){return samplingRate;}
	void AddChannel(string name);
	inline int GetNumChannels(){return channelNames.size();}
	inline string GetChannelName(int index){return channelNames[index];}
	inline vector<string> GetChannelName(){return channelNames;}

	void SetDisplayList();
	void Draw(Posture* pose);
	void InitBones();

	void UpdateGlobal(Posture* pose);

	BodyPart& operator=(BodyPart & rhs);

private:

	string name;						// bone name
	int ID;								// ID, -1 for end effector
	BodyPart* prevBodyPart;				// point to farther node
	vector<BodyPart*> nextBodyParts;	// point to child nodes
	GLuint ListNum;						// display list ID
	
	//Joint
	double localOffset[3];				// the offset to parent joint
	double globalOffset[3];				// the space to store the calculated global coordinate 
	double sphereRadius;				// joint size

	// Bone
	double length;					// bone length
	double width;					// bone width
	double orientation[3];			// bone orientation

	// Motion
	double samplingRate;			// useless now
	vector<string> channelNames;	// DOF Name
};

#endif