#include "StdAfx.h"
#include "Motion_Loader.h"

#define PI 3.14159265359

#include <string>
#include <GL/glut.h>

BodyPart::BodyPart()
{
	name = "";
	prevBodyPart = NULL;
	nextBodyParts.resize(0);

	localOffset[X] = 0;
	localOffset[Y] = 0;
	localOffset[Z] = 0;
	
	globalOffset[X] = 0;
	globalOffset[Y] = 0;
	globalOffset[Z] = 0;

	sphereRadius = 0.5;

	width = 2;
	length = 1;

	channelNames.resize(0);
}

BodyPart::~BodyPart()
{
	for(unsigned int i=0;i<nextBodyParts.size();i++)
		delete nextBodyParts[i];
	nextBodyParts.clear();
	channelNames.clear();
}

void BodyPart::SetLocalOffset(double offsets[3])
{
	localOffset[X] = offsets[X];
	localOffset[Y] = offsets[Y];
	localOffset[Z] = offsets[Z];
}

void BodyPart::SetGlobalOffset(double offsets[3])
{
	globalOffset[X] = offsets[X];
	globalOffset[Y] = offsets[Y];
	globalOffset[Z] = offsets[Z];
}

void BodyPart::SetOrientation(double xRot, double yRot, double zRot)
{
	orientation[X] = xRot;
	orientation[Y] = yRot;
	orientation[Z] = zRot; 
}

void BodyPart::AddChannel(string name)
{
	channelNames.push_back(name);
}

void BodyPart::InitBones()
{
	length = sqrt(localOffset[X]*localOffset[X]+localOffset[Y]*localOffset[Y]+localOffset[Z]*localOffset[Z]);

	orientation[X] = localOffset[X];
	orientation[Y] = localOffset[Y];
	orientation[Z] = localOffset[Z];

	SetDisplayList();

	for(int i = 0; i < int(nextBodyParts.size()); i++)
		nextBodyParts[i]->InitBones();
}

void BodyPart::SetDisplayList()
{
	GLUquadricObj *p;
	p = gluNewQuadric();
	gluQuadricDrawStyle(p, GLU_FILL);
	gluQuadricNormals(p, GLU_SMOOTH);

	ListNum = glGenLists(1);
	
	glNewList(ListNum, GL_COMPILE);
		glScalef(width,length/2, width);
		glTranslatef(0,1,0);
		gluSphere(p,1,10,10);
    glEndList();

	gluDeleteQuadric(p);
}

void BodyPart::Draw(Posture* pose)
{
	//Add your code here
}

void BodyPart::UpdateGlobal(Posture* pose)
{
	//Add your code here
}

BodyPart* BodyPart::SearchBodyPart(int n)
{
	if(ID == n)
		return this;
	else
	{
		for(int i=0;i<NumLinks();i++)
		{
			BodyPart* result = nextBodyParts[i]->SearchBodyPart(n);
			if(result != NULL)
				return result;
		}
	}
	return NULL;
}

BodyPart* BodyPart::SearchBodyPart(string bonename)
{
	if(name.compare(bonename) == 0)
		return this;
	else
	{
		for(int i=0;i<NumLinks();i++)
		{
			BodyPart* result = nextBodyParts[i]->SearchBodyPart(bonename);
			if(result != NULL)
				return result;
		}
	}
	return NULL;
}

int BodyPart::GetBodyPartID(string bonename)
{
	BodyPart* result = SearchBodyPart(bonename);
	return result->GetID();
}

void BodyPart::DelLinkToBodyPart(BodyPart* target)
{
	for(int i=0;i<NumLinks();i++)
	{
		if(nextBodyParts[i] == target)
			DelLinkToNextBodyPart(i);
		else
			nextBodyParts[i]->DelLinkToBodyPart(target);
	}
}


void BodyPart::DelLinkToBodyPart(int ID)
{
	for(int i=0;i<NumLinks();i++)
	{
		if(nextBodyParts[i]->GetID() == ID)
			DelLinkToNextBodyPart(i);
		else
			nextBodyParts[i]->DelLinkToBodyPart(ID);
	}
}


void BodyPart::DelLinkToBodyPart(string name)
{
	for(int i=0;i<NumLinks();i++)
	{
		if(name == nextBodyParts[i]->GetName())
			DelLinkToNextBodyPart(i);
		else
			nextBodyParts[i]->DelLinkToBodyPart(name);
	}
}

BodyPart& BodyPart::operator=(BodyPart & rhs)
{
	name = rhs.GetName();
	ListNum = rhs.GetListNum();

	// Joint
	double *lo = rhs.GetLocalOffset();
	int i;
	for(i=0;i<3;i++)
		localOffset[i] = lo[i];

	double *go = rhs.GetGlobalOffset();
	for(i=0;i<3;i++)
		globalOffset[i] = go[i];

	sphereRadius = rhs.GetRadius();

	// Bone
	length = rhs.GetLength();
	width = rhs.GetWidth();
	double *o = rhs.GetOrientation();
	for(int i=0;i<3;i++)
		orientation[i] = o[i];

	// Motion
	samplingRate = rhs.GetSamplingRate();

	channelNames.clear();
	channelNames = rhs.GetChannelName();

	for(unsigned int i=0;i<nextBodyParts.size();i++)
		delete nextBodyParts[i];
	nextBodyParts.clear();

	// nextbodypart point to the same bodypart
	nextBodyParts = rhs.GetNextBodyPart();

	return *this;
}
