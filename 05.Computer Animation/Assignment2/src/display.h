/*
    display.h

	Display the skeleton, ground plane and other objects.			
 
    Revision 1 - Steve Lin, Jan. 14, 2002
    Revision 2 - Alla and Kiran, Jan 18, 2002
*/

#ifndef _DSIPLAY_H
#define _DISPLAY_H

#include <GL/gl.h>
#include "skeleton.h"
#include "motion.h"


class Display 
{

	//member functions
	public: 
		Display();
		~Display();

		//set actor for display
		void loadActor(Skeleton *pActor);
		//set motion for display
		void loadMotion(Motion *pMotion);
      
		//display the scene (actor, ground plane ....)
		void show();
	
	private:
		//Draw a particular bone
		void drawBone(Bone *ptr,int skelNum);
		//Draw the skeleton hierarchy
		void traverse(Bone *ptr,int skelNum);
   
	
	//member variables	
	public: 
		int m_SpotJoint;		//joint whose local coordinate system is drawn
		int numActors;//¦³´X­ÓActor
		Skeleton *m_pActor[2];		//pointer to current actor
		Motion *m_pMotion[2];		//pointer to current motion	

    private:  
		GLuint m_BoneList[2];		//display list with bones
};

#endif
