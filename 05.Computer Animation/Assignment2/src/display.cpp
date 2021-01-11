#include "stdafx.h"
#ifdef WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "GL/glut.H"
#include "GL/gl.h"     
#include "skeleton.h"
#include "motion.h"
#include "display.h"
#include "transform.h"
//#include "types.h"


Display::Display()
{
	m_SpotJoint = -1;
	numActors = 0;
	m_pActor[0] = NULL;
	m_pMotion[0] = NULL;
}

Display::~Display()
{
   //if(m_pActor != NULL) delete m_pActor;
   //if(m_pMotion != NULL) delete m_pMotion;
}


//Draws the world coordinate axis
static void draw_bone_axis() 
{
 
   glBegin(GL_LINES);
   // draw x axis in red, y axis in green, z axis in blue 
   glColor3f(1., .2, .2);
   glVertex3f(0., 0., 0.);
   glVertex3f(10, 0., 0.);

   glColor3f(.2, 1., .2);
   glVertex3f(0., 0., 0.);
   glVertex3f(0., 10, 0.);

   glColor3f(.2, .2, 1.);
   glVertex3f(0., 0., 0.);
   glVertex3f(0., 0., 10);

   glEnd();
}

//Draws a line from point a to point b
static void draw_vector(double *a, double *b) 
{
   glBegin(GL_LINES);
   glColor3f(.5, .5, .5);
   glVertex3f(a[0], a[1], a[2]);
   glVertex3f(b[0], b[1], b[2]);
   glEnd();
}


//Pre-draw the bones using quadratic object drawing function
//and store them in the display list
//建立一堆display List好到時候畫上去
//Parameter:從哪一個Bone開始畫起 以及畫的順序 主要是畫球球的本體Code在這邊進行定義
void set_display_list(Bone *bone, GLuint *pBoneList)
{
	int numbones = numBonesInSkel(bone[0]);
	if(*pBoneList)//砍掉舊的list
	{
		glDeleteLists(*pBoneList,numbones);
	}

   int j;
   GLUquadricObj *qobj;

   *pBoneList = glGenLists(numbones);//generate a contiguous set of empty display lists  好進行Output
   /*
     gluNewQuadric creates and returns a pointer to a new quadrics object. 
     This object must be referred to when calling quadrics rendering and control functions.
	 建立一個二次方程式
   */
   qobj=gluNewQuadric();

   gluQuadricDrawStyle(qobj, (GLenum) GLU_FILL);
   gluQuadricNormals(qobj, (GLenum) GLU_SMOOTH);
   for(j=0;j<numbones;j++)
   {
      glNewList(*pBoneList + j, GL_COMPILE);
      glScalef(bone[j].aspx, bone[j].aspy, 1.);
      gluSphere(qobj, bone[j].length/2.0, 20, 20);
      glEndList();
   }
}


/*

	Define M_k = Modelview matrix at the kth node (bone) in the heirarchy
	M_k stores the transformation matrix of the kth bone in world coordinates
	Our goal is to draw the (k+1)th bone, using its local information and M_k
	
	  In the k+1th node, compute the following matrices:
		rot_parent_current: this is the rotation matrix that 
							takes us from k+1 to the kth local coordinate system 
		R_k+1 : Rotation matrix for the k+1 th node (bone)
				using angles specified by the AMC file in local coordinates
		T_k+1 : Translation matrix for the k+1th node

	The update relation is given by:
		M_k+1 = M_k * (rot_parent_current) * R_k+1 + T_k+1


    此為IK
    
	 另R i   :從第i coordinate system轉成第i+1個Coordinate System  
	    i+1  
	   V     :以第i+1個Coordinate System為準的向量
	    i+1
	     
	 V    = R i   V
	  i+1    i+1   i

     所以他先透過目前矩陣乘上 R i    = R i   的方式找到 R  0   ==>這樣就可以畫上東西了
	                            asf      i+1              i+1
*/
void Display::drawBone(Bone *pBone,int skelNum)
{
	static double z_dir[3] = {0., 0., 1.};//Global Z軸方向
	double r_axis[3], theta;

	//Tranform (rotate) from the local coordinate system of this bone to it's parent
	//This step corresponds to doing: ModelviewMatrix = M_k * (rot_parent_current)
	glMultMatrixd((double*)&pBone->rot_parent_current);  //將目前的Matrix乘上  iRasf   

	
	//Draw the local coordinate system for the selected bone.
	if(pBone->idx == m_SpotJoint)
		draw_bone_axis();

	//rotate AMC 
	//This step corresponds to doing: ModelviewMatrix *= R_k+1
	if(pBone->doftz) glTranslatef(0.,0.,pBone->tz);
	if(pBone->dofty) glTranslatef(0.,pBone->ty,0.);
	if(pBone->doftx) glTranslatef(pBone->tx,0.,0.);

	//Edited by spectral for project
	//bug:
	//與root之   order TX TY TZ RZ RY RX有關
	//
	if(pBone->idx == root)
	{
		if(pBone->dofz) glRotatef(pBone->drz, 0., 0., 1.);
		if(pBone->dofy) glRotatef(pBone->dry, 0., 1,  0.);
		if(pBone->dofx) glRotatef(pBone->drx, 1., 0., 0.);
	}
	else
	{
		if(pBone->dofz) glRotatef(pBone->drz, 0., 0., 1.);
		if(pBone->dofy) glRotatef(pBone->dry, 0., 1,  0.);
		if(pBone->dofx) glRotatef(pBone->drx, 1., 0., 0.);
	}

	float mcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	float mcolor2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mcolor2);
	glColor3f(1., 1., 0.1);

	//Store the current ModelviewMatrix (before adding the translation part)
	glPushMatrix();

	//Compute tx, ty, tz - translation from pBone to it's child (in local coordinate system of pBone)
	double tx = pBone->dir[0]*pBone->length;
	double ty = pBone->dir[1]*pBone->length;
	double tz = pBone->dir[2]*pBone->length;

	

	// Use the current ModelviewMatrix to display the current bone
	// Rotate the bone from its canonical position (elongated sphere 
	// with its major axis parallel to X axis) to its correct orientation
	if(pBone->idx == root)
		glCallList(m_BoneList[skelNum] + pBone->idx);
	else
	{ 
		//移到中心點以及進行相關設定以畫圖
		//translate to the center of the bone
		glTranslatef(tx/2, ty/2, tz/2);

		//Compute the angle between the canonical pose and the correct orientation 
		//(specified in pBone->dir) using cross product.
		//Using the formula: r_axis = z_dir x pBone->dir
		
		v3_cross(z_dir, pBone->dir, r_axis);

		theta =  GetAngle(z_dir, pBone->dir, r_axis);
		
		glRotatef(theta*180./3.14159265359, r_axis[0], r_axis[1], r_axis[2]);//對該軸旋轉
		
		glCallList(m_BoneList[skelNum] + pBone->idx);//畫上圓
	}

	glPopMatrix(); 

	// Finally, add the translation component to the ModelviewMatrix
	// This step corresponds to doing: M_k+1 = ModelviewMatrix += T_k+1
	glTranslatef(tx, ty, tz);

}

//Traverse the hierarchy starting from the root 
//Every node in the data structure has just one child pointer. 
//If there are more than one children for any node, they are stored as sibling pointers
//The algorithm draws the current node (bone), visits its child and then visits siblings
void Display::traverse(Bone *ptr,int skelNum)
{
   if(ptr != NULL)
   {
      /*先畫到End畫完在畫其他部分*/
      glPushMatrix();
      drawBone(ptr,skelNum);
      traverse(ptr->child,skelNum);
      glPopMatrix();
      traverse(ptr->sibling,skelNum);
   }
}

//Draw the skeleton
void Display::show()
{
	static int showed = 0;//第一次進來沒有東西
    if (showed == 0){
		for (int i = 0; i < numActors; i++)
			set_display_list(m_pActor[i]->getRoot(), &m_BoneList[i]);
        showed = 1;
    }

   draw_bone_axis();

   glPushMatrix();

   //Translate the root to the correct position (it is (0,0,0) if no motion is loaded)
//   glTranslatef(m_pActor->m_RootPos[0], m_pActor->m_RootPos[1], m_pActor->m_RootPos[2]);

   //draw the skeleton starting from the root
   for (int i = 0; i < numActors; i++)
   {
	    /*先將畫筆定位至Root的位置*/
		glPushMatrix();
		glTranslatef(m_pActor[i]->tx,m_pActor[i]->ty,m_pActor[i]->tz);

//		glRotatef(270,1,0,0);

		glRotatef(m_pActor[i]->rx,1,0,0);
		glRotatef(m_pActor[i]->ry,0,1,0);
		glRotatef(m_pActor[i]->rz,0,0,1);

	   traverse(m_pActor[i]->getRoot(),i);
   
		glPopMatrix();
   }
   glPopMatrix();

   /*畫IK的那些點*/
   if ( m_pMotion[0]->isIK ) {
	   glPushMatrix();
	   m_pMotion[0]->Draw();
	   glPopMatrix();
   }
  
}

void Display::loadMotion(Motion *pMotion)
{
	if (numActors - 1 > 2) return;
//set a pointer to the new motion

//	if(m_pMotion[numActors-1]!=NULL) 
//		delete m_pMotion[numActors-1];
	m_pMotion[0] = pMotion;
}

//Set actor for display
//edited by spectral for single actor 07/04/09
void Display::loadActor(Skeleton *pActor)
{
	numActors=1;
	//set a pointer to the actor info
	if (numActors > 2) return;

	//m_pActor[numActors++] = pActor;
	m_pActor[0] = pActor;

	//Create the display list for the skeleton.
	//All the bones are the elongated spheres centered at (0,0,0).
	//The axis of elongation is the X axis.
	set_display_list(m_pActor[0]->getRoot(), &m_BoneList[0]);
}



