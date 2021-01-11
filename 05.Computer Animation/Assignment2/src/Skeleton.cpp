#include "Stdafx.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Skeleton.h"
#include "transform.h"

#include <iostream>
#include <string>
using namespace std;



/***********************************************************************************************************

   Read skeleton file

***********************************************************************************************************/
/*找出到底有幾個Bone在Skeleton中*/
int numBonesInSkel(Bone item)
{
	Bone * tmp = item.sibling;
	int i = 0;
	while (tmp != NULL) 
	{
		if (tmp->child != NULL)
			i+= numBonesInSkel(*(tmp->child));
		i++; tmp = tmp->sibling; 
	}
if (item.child != NULL)
	return i+1+numBonesInSkel(*item.child);
else
	return i+1;
}

int movBonesInSkel(Bone item)
{
	Bone * tmp = item.sibling;
	int i = 0;

	if (item.dof > 0) i++;
	
	while (tmp != NULL) 
	{
		if (tmp->child != NULL)
			i+= movBonesInSkel(*(tmp->child));
		if (tmp->dof > 0) i++; tmp = tmp->sibling; 
	}

if (item.child != NULL)
	return i+movBonesInSkel(*item.child);
else
	return i;
}

// helper function to convert ASF part name into bone index
int Skeleton::name2idx(const char *name)
{
int i=0;
	while(strcmp(m_pBoneList[i].name, name) != 0 && i++ < NUM_BONES_IN_ASF_FILE);
		return m_pBoneList[i].idx;
}

char * Skeleton::idx2name(int idx)
{
	int i=0;
	while(m_pBoneList[i].idx != idx && i++ < NUM_BONES_IN_ASF_FILE);
		return m_pBoneList[i].name;
}
void compute_rotation_parent_child(Bone *parent, Bone *child);
void Skeleton::readASFfile(string asf_filename, float scale)
{
	// Add your code here
	/*
	  *因為ASF File的所有東西都為global coordinate所以要把他轉成Local coordinate以便儲存
	
	*/
	ifstream file( asf_filename.c_str() );
	
	
	string str ="";
	file >> str;
	while ( !file.eof() ) {
		file >> str;
		
		if ( str ==  ":root" ) {
			//READ Root Part  
			printf( "READ ROOT PART\n" );
			
			while ( str !=  ":bonedata" ) {//還沒讀到bonedata
				file >> str;
				//cout << str<<"\n";
				if ( str == "position"  ) {
					file >> tx >> ty >> tz;
				}
				if ( str == "orientation" ) {
					file >> rx >> ry >> rz;
				}
			}
		}
		if ( str ==  ":bonedata"  ) {
			/*
			 READ Bone Section
			 1.本程式將會忽略id的部分
			*/
			printf( "READ BONE SECTION\n" );

			file >> str;
			Bone* bone = NULL;
			do {
				/*將資料放入相對應Bone中 接下來會一次讀到下一個END為止*/
				if ( str == "begin" ) {
					//開始填入資料 處理一些Default Setting
					bone = &m_pBoneList[NUM_BONES_IN_ASF_FILE];
					bone->child = NULL; bone->sibling = NULL;
					bone->idx = NUM_BONES_IN_ASF_FILE;

					bone->dir[0] = bone->dir[1] = bone->dir[2] = 0.;//從此到下一個Bone的朝向
					bone->length = 0;

					//orientation of each bone's local coordinate system as specified in ASF file (axis field)
					bone->axis_x = bone->axis_y = bone->axis_z = 0.;

					bone->aspx = bone->aspy = 1;// aspect ratio of bone shape

					//自由度設定
					bone->dof = bone->dofx = bone->dofy = bone->dofz = 0;//目前沒有自由度
					bone->doftx = bone->dofty = bone->doftz = 0;//表示擁有可以Translation的DOF
					bone->doftl = 0;

					//目前沒有AMC資料設定
					bone->drx = bone->dry = bone->drz = 0;//對x y z軸之旋轉量
					bone->tx = bone->ty = bone->tz = 0;
					bone->tl = 0;
					
					file >> str;
				}

				if ( bone != NULL ) {//必須有Bone才能填資料
					if ( str == "id" ) {
						file >> str >> str;
					}
					if ( str == "name" ) 
						file >> bone->name >> str;
					if ( str == "direction" ) 
						//目前還不知道階層 所以dir[3]的方向為World coordinate請注意 在目前階段他有沒有normalize都不重要
						file >> bone->dir[0] >> bone->dir[1]  >> bone->dir[2] >> str;
					
					if ( str == "length" ) 
						file >>bone->length >> str;
					
					if ( str == "axis" ) {
						double euler[3] = { 0 };//Euler Angle旋轉角度
						file >> euler[0] >> euler[1] >> euler[2] >> str;
						unsigned i = str.find("X");
						bone->axis_x = euler[i];
						i = str.find("Y");
						bone->axis_y = euler[i];
						i = str.find("Z");
						bone->axis_z = euler[i];

						file >> str;
					}

					if ( str == "dof" ) {
						char line[50]="";
						file.getline( line ,50 );
						char* keyword = strtok( line ," " );
						int dof = 0;
						do {
							if ( strcmp( keyword ,"rx" ) == 0 )
								bone->dofx = 1;
							else if ( strcmp( keyword ,"ry" ) == 0 )
								bone->dofy = 1;
							else if ( strcmp( keyword ,"rz" ) == 0 ) 
								bone->dofz = 1;
							else if ( strcmp ( keyword ,"tx" ) == 0 ) 
								bone->doftx = 1;
							else if ( strcmp ( keyword ,"ty" ) == 0 )
								bone->dofty = 1;
							else if ( strcmp( keyword ,"tz" ) == 0 ) 
								bone->doftz = 1;
							
							dof++;
							keyword = strtok( NULL ," " );
						}while ( keyword != NULL );
						
						bone->dof = dof;
						MOV_BONES_IN_ASF_FILE++;//有DOF表示可以移動或轉動
						file >> str;
					}
					if ( str == "limits" ) {
						//目前沒有用到Motion edition的功能 所以沒甚麼用
					}
					if ( str == "end" ) {
						bone = NULL;//表示填入資料結束 必須等待下一次Begin才能填資料
						NUM_BONES_IN_ASF_FILE++;
					}
				}

				file >> str;
			} while ( str != ":hierarchy" );

		}
		if ( str ==  ":hierarchy" ) {
			/*
			 READ Hierarchy Section
			 1.請算出  R i+1  後記住 要將dir[3]換回來
			              i
			*/
			printf( "READ HIERARCHY SECTION\n" );
			file >> str;
			file.get();
			if ( str == "begin" ) {

				char line[200] ="";
				while ( !file.eof() && strstr(line ,"end" ) == NULL ) {
					
					file.getline( line , 200 );
					//cout << line << "\n";

					char* name = strtok( line ," " );
					int pi = name2idx( name );
					Bone* parent = &m_pBoneList[pi];

					name = strtok( NULL ," " );
					while( name != NULL ) {
						int ci = name2idx( name );
						Bone* child = &m_pBoneList[ci];

						setChildrenAndSibling( pi ,child );

						name = strtok( NULL ," " );
					}
				}
			}


			
		}
		
			
	}
	
	
	file.close();

	

}


/*
   This recursive function traverces skeleton hierarchy 
   and returns a pointer to the bone with index - bIndex
   ptr should be a pointer to the root node 
   when this function first called
*/
Bone* Skeleton::getBone(Bone *ptr, int bIndex)
{
   static Bone *theptr;
   if(ptr==NULL) 
      return(NULL);
   else if(ptr->idx == bIndex)
   {
      theptr=ptr;
      return(theptr);
   }
   else
   { 
      getBone(ptr->child, bIndex);
      getBone(ptr->sibling, bIndex);
      return(theptr);
   }
}

/*
  This function sets sibling or child for parent bone
  If parent bone does not have a child, 
  then pChild is set as parent's child
  else pChild is set as a sibling of parents already existing child
*/
int Skeleton::setChildrenAndSibling(int parent, Bone *pChild)
{
	Bone *pParent;  
   
	//Get pointer to root bone
	pParent = getBone(m_pRootBone, parent);

	if(pParent==NULL)
	{
		printf("inbord bone is undefined\n"); 
		return(0);
	}
	else
	{
		//if pParent bone does not have a child
		//set pChild as parent bone child
		if(pParent->child == NULL)   
		{
			pParent->child = pChild;
		}
		else
		{ 
			//if pParent bone already has a child 
			//set pChils as pParent bone's child sibling
			pParent=pParent->child;              
			while(pParent->sibling != NULL) 
				pParent = pParent->sibling;            

			pParent->sibling = pChild;
		}
		return(1);
	}
}

/* 
	Return the pointer to the root bone
*/	
Bone* Skeleton::getRoot()
{
   return(m_pRootBone);
}


/***************************************************************************************
  Compute relative orientation and translation between the 
  parent and child bones. That is, represent the orientation 
  matrix and translation vector in the local coordinate of parent body 
*****************************************************************************************/


/*
	This function sets rot_parent_current data member.
	Rotation from this bone local coordinate system 
	to the coordinate system of its parent
*/
void compute_rotation_parent_child(Bone *parent, Bone *child)
{

   double Rx[4][4], Ry[4][4], Rz[4][4], tmp[4][4], tmp1[4][4], tmp2[4][4];

   if(child != NULL)
   { 
     
     // The following openGL rotations are precalculated and saved in the orientation matrix. 
     //
     // glRotatef(-inboard->axis_x, 1., 0., 0.);
     // glRotatef(-inboard->axis_y, 0., 1,  0.);
     // glRotatef(-inboard->axis_z, 0., 0., 1.);
     // glRotatef(outboard->axis_z, 0., 0., 1.);
     // glRotatef(outboard->axis_y, 0., 1,  0.);
     // glRotatef(outboard->axis_x, 1., 0., 0.);

     /*       T
	   ( R 0 )
		   p
	 */
     rotationZ(Rz, -parent->axis_z);      
     rotationY(Ry, -parent->axis_y);  
     rotationX(Rx, -parent->axis_x);      
     matrix_mult(Rx, Ry, tmp);
     matrix_mult(tmp, Rz, tmp1);
     /*
	    R 0
		  c
	 */
     rotationZ(Rz, child->axis_z);
     rotationY(Ry, child->axis_y);
     rotationX(Rx, child->axis_x);
     matrix_mult(Rz, Ry, tmp);
     matrix_mult(tmp, Rx, tmp2);

     matrix_mult(tmp1, tmp2, tmp);
     matrix_transpose(tmp, child->rot_parent_current);    
   }
}


// loop through all bones to calculate local coordinate's direction vector and relative orientation  
void ComputeRotationToParentCoordSystem(Bone *bone)
{
	int i;
	double Rx[4][4], Ry[4][4], Rz[4][4], tmp[4][4], tmp2[4][4];

	//Compute rot_parent_current for the root 

	//Compute tmp2, a matrix containing root 
	//joint local coordinate system orientation
	rotationZ(Rz, bone[root].axis_z);
	rotationY(Ry, bone[root].axis_y);
	rotationX(Rx, bone[root].axis_x);
	matrix_mult(Rz, Ry, tmp);
	matrix_mult(tmp, Rx, tmp2);
	//set bone[root].rot_parent_current to transpose of tmp2
	matrix_transpose(tmp2, bone[root].rot_parent_current);    



	//Compute rot_parent_current for all other bones
	int numbones = numBonesInSkel(bone[0]);
	for(i=0; i<numbones; i++) 
	{
		if(bone[i].child != NULL)
		{
			compute_rotation_parent_child(&bone[i], bone[i].child);
		
			// compute parent child siblings...
			Bone * tmp = NULL;
			if (bone[i].child != NULL) tmp = (bone[i].child)->sibling;
			while (tmp != NULL)
			{
				compute_rotation_parent_child(&bone[i], tmp);
				tmp = tmp->sibling;
			}
		}
	}
}

/*
	Transform the direction vector (dir), 
	which is defined in character's global coordinate system in the ASF file, 
	to local coordinate
*/
void Skeleton::RotateBoneDirToLocalCoordSystem()
{
	int i;

	for(i=1; i<NUM_BONES_IN_ASF_FILE; i++) 
	{
		//Transform dir vector into local coordinate system
		vector_rotationXYZ(&m_pBoneList[i].dir[0], -m_pBoneList[i].axis_x, -m_pBoneList[i].axis_y, -m_pBoneList[i].axis_z); 
	}

}


/******************************************************************************
Interface functions to set the pose of the skeleton 
******************************************************************************/

//Initial posture Root at (0,0,0)
//All bone rotations are set to 0
void Skeleton::setBasePosture()
{
   int i;
   m_RootPos[0] = m_RootPos[1] = m_RootPos[2] = 0.0;

   for(i=0;i<NUM_BONES_IN_ASF_FILE;i++)
      m_pBoneList[i].drx = m_pBoneList[i].dry = m_pBoneList[i].drz = 0.0;
}


// set the skeleton's pose based on the given posture
/*
  設定Skeleton的Posture 也就是讓dr* 以及d*的參數有值
*/
void Skeleton::setPosture(Posture posture) 
{
    m_RootPos[0] = posture.root_pos.x;
    m_RootPos[1] = posture.root_pos.y;
    m_RootPos[2] = posture.root_pos.z;

    for(int j=0;j<NUM_BONES_IN_ASF_FILE;j++)
    {
		// if the bone has rotational degree of freedom in x direction
		if(m_pBoneList[j].dofx) 
		   m_pBoneList[j].drx = posture.bone_rotation[j].x;  

		if(m_pBoneList[j].doftx)
			m_pBoneList[j].tx = posture.bone_translation[j].x;

		// if the bone has rotational degree of freedom in y direction
		if(m_pBoneList[j].dofy) 
		   m_pBoneList[j].dry = posture.bone_rotation[j].y;    

		if(m_pBoneList[j].dofty)
			m_pBoneList[j].ty = posture.bone_translation[j].y;

		// if the bone has rotational degree of freedom in z direction
		if(m_pBoneList[j].dofz) 
		   m_pBoneList[j].drz = posture.bone_rotation[j].z;  

		if(m_pBoneList[j].doftz)
			m_pBoneList[j].tz= posture.bone_translation[j].z;
		
		if(m_pBoneList[j].doftl)
			m_pBoneList[j].tl = posture.bone_length[j].x;
    }
}


//Set the aspect ratio of each bone 
void set_bone_shape(Bone *bone)
{
   bone[root].aspx=1;          bone[root].aspy=1;
   	printf("READ %d\n",numBonesInSkel(bone[0]));
	printf("MOV %d\n",movBonesInSkel(bone[0]));
	int numbones = numBonesInSkel(bone[0]);
   for(int j=1;j<numbones;j++)
    {
		bone[j].aspx=0.25;   bone[j].aspy=0.25;
    }

}


/*
Constructor 
Skeleton的
*/
Skeleton::Skeleton(string asf_filename, float scale)
{
	/*初始化Root的資料*/
	sscanf("root","%s",m_pBoneList[0].name);
	NUM_BONES_IN_ASF_FILE = 1;
	MOV_BONES_IN_ASF_FILE = 1;
    m_pBoneList[0].dofo[0] = 4;
	m_pBoneList[0].dofo[1] = 5;
	m_pBoneList[0].dofo[2] = 6;
    m_pBoneList[0].dofo[3] = 1;
	m_pBoneList[0].dofo[4] = 2;
	m_pBoneList[0].dofo[5] = 3;
	m_pBoneList[0].dofo[6] = 0;
	//Initializaton
	m_pBoneList[0].idx = root;   // root of hierarchy
	m_pRootBone = &m_pBoneList[0];
	m_pBoneList[0].sibling = NULL;//因為是Root所以沒有小孩或是隔壁鄰居
	m_pBoneList[0].child = NULL; 
	m_pBoneList[0].dir[0] = 0; m_pBoneList[0].dir[1] = 0.; m_pBoneList[0].dir[2] = 0.;
	m_pBoneList[0].axis_x = 0; m_pBoneList[0].axis_y = 0.; m_pBoneList[0].axis_z = 0.;
	m_pBoneList[0].length = 0.05;
	m_pBoneList[0].dof = 6;
	m_pBoneList[0].dofx = m_pBoneList[0].dofy = m_pBoneList[0].dofz=1;
	m_pBoneList[0].drx = m_pBoneList[0].dry = m_pBoneList[0].drz = 0;
	//added by spectral 08/08/06
	m_pBoneList[0].doftx = m_pBoneList[0].dofty = m_pBoneList[0].doftz=1;
	m_pBoneList[0].tx = m_pBoneList[0].ty = m_pBoneList[0].tz=0;
	
	m_pBoneList[0].doftl = 0;

	//世界座標系的Root位置
	m_RootPos[0] = m_RootPos[1]=m_RootPos[2]=0;
//	m_NumDOFs=6;
	tx = ty = tz = rx = ry = rz = 0;
	/*初始化結束*/

	// build hierarchy and read in each bone's DOF information
	readASFfile(asf_filename, scale);  

	//transform the direction vector for each bone from the world coordinate system 
	//to it's local coordinate system
	/*
	  計算 R p = R p  * R 0   ---->rot_parent_current
			 c     0      c
	*/
	RotateBoneDirToLocalCoordSystem();

	//Calculate rotation from each bone local coordinate system to the coordinate system of its parent
	//store it in rot_parent_current variable for each bone
	ComputeRotationToParentCoordSystem(m_pRootBone);

	//Set the aspect ratio of each bone 
	set_bone_shape(m_pRootBone);

	setBasePosture();
}

Skeleton::~Skeleton()
{
}



