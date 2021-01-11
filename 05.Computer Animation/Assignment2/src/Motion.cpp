#include "StdAfx.h"
#include <algorithm>
#include <time.h>			// function for playing rate
#include <fstream>
#include <string>
#include "Motion.h"
#include "transform.h"
#include "./Math/VectorRn.h"
#include "./Math/MatrixRmn.h"
#include "glut.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"

#include <iostream>

using std::ifstream;
using std::string;

/************************ Motion class functions **********************************/
Motion::Motion(int nNumFrames)
{
//	m_NumDOFs = pActor.m_NumDOFs;
	
	m_NumFrames = nNumFrames;
	offset = 0;

	//allocate postures array
	m_pPostures = new Posture [m_NumFrames];
	active = false;
	//Set all postures to default posture
	SetPosturesToDefault();
}

Motion::Motion(string amc_filename, float scale,Skeleton * pActor2)
{
	pActor = pActor2;
	
//	m_NumDOFs = actor.m_NumDOFs;
	offset = 0;
	m_NumFrames = 0;
	m_pPostures = NULL;
	active = false;
	isIK = false;
	LoadAMC(amc_filename, scale);	
}

Motion::Motion(string amc_filename, float scale)
{
//	m_NumDOFs = actor.m_NumDOFs;
	offset = 0;
	m_NumFrames = 0;
	m_pPostures = NULL;
	active = false;
	LoadAMC(amc_filename, scale);
}


Motion::~Motion()
{
	if (m_pPostures != NULL)
		delete [] m_pPostures;
}


//Set all postures to default posture
void Motion::SetPosturesToDefault()
{
	//for each frame
	//int numbones = numBonesInSkel(bone[0]);
	for (int i = 0; i < m_NumFrames; i++)
	{
		
		//set root position to (0,0,0)
		m_pPostures[i].root_pos.set(0,0,0);
		//set each bone orientation to (0,0,0)
		for (int j = 0; j < pActor->NUM_BONES_IN_ASF_FILE; j++) {
			//m_pPostures[i].bone_rotation[j].set(0,0,0);
			m_pPostures[i].bone_rotation.push_back( Vector3(0 ,0 ,0 ) );
			m_pPostures[i].bone_translation.push_back( Vector3(0 ,0 ,0 ) );
			m_pPostures[i].bone_position.push_back( Vector3(0 ,0 ,0 ) );

			m_pPostures[i].axisX.push_back( Vector3( 0,0,0 ) );
			m_pPostures[i].axisY.push_back( Vector3( 0,0,0 ) );
			m_pPostures[i].axisZ.push_back( Vector3( 0,0,0 ) );

		}

	}
}

//Set posture at spesified frame
void Motion::SetPosture(int nFrameNum, Posture InPosture)
{
	m_pPostures[nFrameNum] = InPosture; 	
}

int Motion::GetPostureNum(int nFrameNum)
{
	nFrameNum += offset;

	if (nFrameNum < 0)
		return 0;
	else if (nFrameNum >= m_NumFrames)
		return m_NumFrames-1;
	else
		return nFrameNum;
	return 0;
}

void Motion::SetTimeOffset(int n_offset)
{
	offset = n_offset;
}

void Motion::SetBoneRotation(int nFrameNum, Vector3 vRot, int nBone)
{
	m_pPostures[nFrameNum].bone_rotation[nBone] = vRot;
}

void Motion::SetRootPos(int nFrameNum, Vector3 vPos)
{
	m_pPostures[nFrameNum].root_pos = vPos;
}


Posture* Motion::GetPosture(int nFrameNum)
{
	if (m_pPostures != NULL) 
		return &m_pPostures[nFrameNum]; 
	else 
		return NULL;
}


int Motion::LoadAMC(string name, float scale)
{
	printf("READ AMC FILE \n" );
	// Add your code here
	ifstream input( name.c_str() );

	/*開始讀入資料 先把資料讀過一遍 確定他有多少Frame*/
	char line[1024] ="";
	int frame = 0;
	while ( !input.eof() ) {
		input.getline( line ,1024 );
		int i = atoi( line );
		if ( i != 0 ) frame = i;
	}

	//如果和目前的不一樣大 且已經有目前資料 則重新設定
	if ( m_NumFrames != frame && m_pPostures != NULL ) {
		delete [] m_pPostures;
		m_pPostures = NULL;
	}
	
	m_NumFrames = frame;
	if ( m_pPostures == NULL ) {
		m_pPostures = new Posture[ m_NumFrames ];

		SetPosturesToDefault();
	}
		
	
	
	
	/*開始讀入基本資料 先讀入不重要但是多出來的三行*/
	input.clear();//清掉EOF標籤
	input.seekg( 0 ,ios::beg );//到資料開頭重讀
	for ( int j = 0; j < 3;	j++ ) {
		input.getline( line , 1024 );
		//printf( "%s\n", line );
		
	}
	string keyword = "";
	Bone* root = pActor->getRoot();
	do {
		//讀入資料
		input >> frame;
		Posture* p = &m_pPostures[frame-1];

		//將所有可移動的資料填入
		for ( int i = 0; i < pActor->MOV_BONES_IN_ASF_FILE; i++ ) {
			
			input >> keyword;
			int boneid = pActor->name2idx( keyword.c_str() );
			Bone* bone = pActor->getBone( root ,boneid );
			
			//讀入所有角度
			//從Tx Ty Tz開始讀起至Rx Ry Rz
			if ( bone->doftx ) {
				input >> p->bone_translation[boneid].x;
				p->bone_translation[boneid].x *= scale;
			}
			if ( bone->dofty ) {
				input >> p->bone_translation[boneid].y;
				p->bone_translation[boneid].y *= scale;
			}
			if ( bone->doftz ) {
				input >> p->bone_translation[boneid].z;
				p->bone_translation[boneid].z *= scale;
			}
			if ( bone->dofx )
				input >> p->bone_rotation[boneid].x;
			if ( bone->dofy )
				input >> p->bone_rotation[boneid].y;
			if ( bone->dofz ) 
				input >> p->bone_rotation[boneid].z;
		}

	}while ( frame != m_NumFrames );

	input.close();
	return 1;
}

void Motion::IK()
{
	//Add your code here
	
	isIK = true;
	draw_Posture = 0;
	/*透過Forward Kinemetic計算 先計算出第一個frame的絕對位置 並儲存在Posture內*/
	Bone* root = pActor->getRoot();

	//對每一個Time Step 也就是每一個frame
	for ( int t = 0; t < m_NumFrames - 1; t++ ) {
		//1.取得目前Frame
		Posture* nowframe = GetPosture(t);
		//2.算出整個Posture的世界座標位置
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		BuildIKtree( *nowframe ,root );
		glPopMatrix();

		/*
		  3.設定Constraint 將右手放到頭上
			V = J  * theta
			目前只改變三個東西 就只是為了方便
				[ G - Ex ]
			V = [ G - Ey ]
				[ G - Ez ]
		*/
		//設定目的地 也就是設定Constraint
		int gi = pActor->name2idx("upperneck" );//頭的index = G
		int ei = pActor->name2idx( "rhand" );//右手的hand = E
		//int gi = pActor->name2idx( "upperback" );
		//int ei = pActor->name2idx( "lthumb" );
		//計算V矩陣
		double v[3] = { 0 };
		v[0] = nowframe->bone_position[gi].x - nowframe->bone_position[ei].x;
		v[1] = nowframe->bone_position[gi].y - nowframe->bone_position[ei].y;
		v[2] = nowframe->bone_position[gi].z - nowframe->bone_position[ei].z;
		VectorRn V( 3 );
		V.Load(v);
		//printf("%lf ,%lf ,%lf\n" ,v[0] ,v[1] ,v[2] );
		/*
		  4.產生Jacobian Matrix
			Jacobian Matrix其實很簡單
			每一行表示 該Joint對End Effector的效果
			從rclavicle 開始找Jacobian

		  5.而旋轉軸該怎麼決定呢 其實我們看一下公式 可以發現
		     R1  * R2  * ........* Ri+1 = Ri+1
			  0     1                i      0
		    這樣我們就可以發現一件事情 其實旋轉軸就是你的Global旋轉矩陣的三行

		*/
		//找到整個骨架有多少個自由度
		int dof = 0;
		//int rclaviclei = pActor->name2idx( "lclavicle" );
		int rclaviclei = pActor->name2idx( "rclavicle" );
		Bone* startbone = pActor->getBone( root ,rclaviclei );
		Bone* parent = startbone;
		while ( parent != NULL && parent->idx != ei ) {
			//printf("%d\n" ,parent->idx );
			dof += parent->dof;
			Bone* child = parent->child;
			if ( child == NULL ) 
				child = parent->sibling;
			
			parent = child;
		}

		//Jacobian的column值應該是所有的自由度
		MatrixRmn J( V.GetLength() , dof);
		J.SetZero();
		double R[4][4]= { 0 } ,Rx[4][4] = { 0 } ,Ry[4][4] = { 0 } ,Rz[4][4] = { 0 } ,tmpR[4][4] = { 0 };
		double axis[3] = { 0 };//代表旋轉軸
		double PiE[3] = { 0 };
		double cp[3] = { 0 };//Cross product的結果存在這邊
		parent = startbone;
		int ci = 0;
		while ( parent != NULL && parent->idx != ei ) {
			//找到 Pi - E
			//printf("%d dof ,%s = %d\n" ,dof ,parent->name ,ci );
			int bonei = parent->idx;
			PiE[0] = nowframe->bone_position[ei].x - nowframe->bone_position[bonei].x;
			PiE[1] = nowframe->bone_position[ei].y - nowframe->bone_position[bonei].y;
			PiE[2] = nowframe->bone_position[ei].z - nowframe->bone_position[bonei].z;
			//找出旋轉軸
			rotationX( Rx , parent->axis_x );
			rotationY( Ry , parent->axis_y );
			rotationZ( Rz , parent->axis_z );
			matrix_mult( Rx ,Ry ,tmpR );
			matrix_mult( tmpR ,Rz , R );
			/*matrix_print("Rx" ,Rx );
			matrix_print("Ry" ,Ry );
			matrix_print("Rz" ,Rz );
			matrix_print("tmpR" ,tmpR );
			matrix_print("R" ,R);

			printf("================\n");*/
			//找到和旋轉軸的Cross Product
			if ( parent->dofx ) {
				//axis[0] = R[0][0]; axis[1] = R[1][0]; axis[2] = R[2][0];
				axis[0] = nowframe->axisX[bonei].x;
				axis[1] = nowframe->axisX[bonei].y;
				axis[2] = nowframe->axisX[bonei].z;
				v3_cross( axis ,PiE ,cp );
				VectorRn x(3);
				x.Load(cp);
				
				J.SetColumn( ci++ ,x );

			}
			if ( parent->dofy ) {
				//axis[0] = R[0][1]; axis[1] = R[1][1]; axis[2] = R[2][1];
				axis[0] = nowframe->axisY[bonei].x;
				axis[1] = nowframe->axisY[bonei].y;
				axis[2] = nowframe->axisY[bonei].z;
				v3_cross( axis ,PiE ,cp );
				VectorRn x(3);
				x.Load(cp);
				
				J.SetColumn( ci++ ,x );
				
			}
			if( parent->dofz ) {
				//axis[0] = R[0][2]; axis[1] = R[1][2]; axis[2] = R[2][2];
				axis[0] = nowframe->axisZ[bonei].x;
				axis[1] = nowframe->axisZ[bonei].y;
				axis[2] = nowframe->axisZ[bonei].z;
				v3_cross( axis,PiE ,cp );
				VectorRn x(3);
				x.Load(cp);
				
				J.SetColumn( ci++ ,x );
			}

			Bone* child = parent->child;
			if ( child == NULL )
				child = parent->sibling;
			
			parent = child;
		}

		/*
		  5.透過Psuedo Inverse找到 角加速度
		*/
		int row = J.GetNumRows();
		int col = J.GetNumColumns();
		double* Jacobian = new double [row * col ];
		double* Jocobiani = new double [ row * col  ];
		CvMat MatJ = cvMat( row, col, CV_64FC1, Jacobian);
		CvMat InvtJ = cvMat( col, row, CV_64FC1, Jocobiani);
		for(int i = 0; i < row; i++)
		{
			for(int j = 0; j < col; j++)
			{
				cvmSet(&MatJ, i, j, *J.GetPtr(i ,j) );
				cvmSet(&InvtJ, j, i, 0);
			}
		}
		cvPseudoInverse( &MatJ, &InvtJ);	
		MatrixRmn Jinv( col ,row );
		for(int i = 0; i < col; i++)
		{
			for(int j = 0; j < row; j++)
			{
				Jinv.Set(i ,j ,cvmGet(&InvtJ, i, j) );
			}
		}
		
		VectorRn thetaV( dof );
		Jinv.Multiply( V ,thetaV);//J*thetaV = V

		/*
		  透過現在的Frame 對下一個Frame進行修正取得了 ThetaV後 我們可以慢慢的改變角度
		  偷偷改變角度
		*/
		Posture* nextframe = GetPosture(t+1);
		parent = startbone;
		int index = 0;
		while ( parent != NULL  && parent->idx != ei) {
			if ( parent->dofx ) 
				//nowframe->bone_rotation[parent->idx].x = thetaV[index++];
				nextframe->bone_rotation[ parent->idx ].x =  nowframe->bone_rotation[parent->idx].x + 4 * thetaV[index++];
			if ( parent->dofy ) 
				//nowframe->bone_rotation[parent->idx].y = thetaV[index++];
				nextframe->bone_rotation[ parent->idx ].y =  nowframe->bone_rotation[parent->idx].y + 4 * thetaV[index++];
			if ( parent->dofz ) 
				//nowframe->bone_rotation[parent->idx].z = thetaV[index++];
				nextframe->bone_rotation[ parent->idx ].z =  nowframe->bone_rotation[parent->idx].z + 4 * thetaV[index++];

			//到下一個Child
			Bone* child = parent->child;
			if ( child == NULL )
				child = parent->sibling;
		
			parent = child;
		}
	}

	printf("修改完畢\n");
	
	

	
}

/*
  透過FK求出每一個Bone的位置
  和DrawBone差不多的動作 我們使用OpenGL的Push和Pop方便 要不然照助教給的這種資料結構 可能要自己寫就超麻煩的
  並且程式是根據課本 Figure 5.13之演算法
*/
void Debug() {
	/*Debug用*/
	double a[16] = { 0 };
	glGetDoublev(GL_MODELVIEW_MATRIX ,a );
	
	int index = 0;
	printf("M:\n");
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ )
			printf("%8.3f\t" ,a[index++] );
		printf("\n");
	}
	printf( "\n" );
	/*Debug用*/
}
void 
Motion::BuildIKtree(Posture& p ,const Bone* b)
{
    /*
	  1.先計算  R i+1  = R asf   *  R
				   i        i         amc
	*/
	
	glMultMatrixd((double*)&b->rot_parent_current);  //將目前的Matrix乘上  iRasf  
	//對目前的Matrix做相對應的旋轉
	int bonei = pActor->name2idx( b->name );
	
	
	if( b->doftz ) glTranslatef( 0. ,0. ,p.bone_translation[bonei].z );//平移
	if( b->dofty ) glTranslatef( 0. ,p.bone_translation[bonei].y ,0. );
	if( b->doftx ) glTranslatef( p.bone_translation[bonei].x ,0. ,0. );
	if( b->dofz ) glRotatef( p.bone_rotation[bonei].z , 0., 0., 1.);//旋轉
	if( b->dofy ) glRotatef( p.bone_rotation[bonei].y , 0., 1,  0.);
	if( b->dofx ) glRotatef( p.bone_rotation[bonei].x , 1., 0., 0.);
	/*
	  R i+1 準備好了 取出Matrix   也準備好 R 0
	     i                                   i
    */
	double d[16] = { 0 };
	glGetDoublev(GL_MODELVIEW_MATRIX ,d );

	double M[4][4] = { 0 };
	int index = 0;
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ ) 
			M[j][i] = d[ index++ ];//Column Major

	/*char str[10] = "";
	sprintf( str,"%d M" ,bonei );
	matrix_print(str ,M );*/

	double pos[3] = { 0 };//骨頭長度以及方向 以方便到下一個位置去
	double dir[3] = { 0 };
	dir[0] = b->dir[0] * b->length; dir[1] = b->dir[1] * b->length; dir[2] = b->dir[2] * b->length ;
	matrix_transform_affine( M ,dir[0] ,dir[1] ,dir[2] ,pos );
	////printf("================\n");
	////system("pause");
	p.bone_position[bonei].set( pos[0] ,pos[1] ,pos[2] );//Pw = M*v

	matrix_transform_affine( M ,1 ,0 ,0 ,pos );
	p.axisX[bonei].set( pos[0] ,pos[1] ,pos[2] );
	matrix_transform_affine( M ,0 ,1 ,0 ,pos );
	p.axisY[bonei].set( pos[0] ,pos[1] ,pos[2] );
	matrix_transform_affine( M ,0 ,0 ,1 ,pos );
	p.axisZ[bonei].set( pos[0] ,pos[1] ,pos[2] );

	/**/
	if ( b->child != NULL ) {
		glPushMatrix();//放入目前的Matrix
		
		glTranslatef( dir[0] ,dir[1] ,dir[2] );//M = Rv + T  改變目前的Matrix
		//Debug();
		BuildIKtree( p ,b->child );
		glPopMatrix();//丟出目前的Matrix
	}
	if ( b->sibling != NULL ) {
		//glPopMatrix();//Pop以丟出還沒被加進Child時的Matrix
		Bone* sibling = b->sibling;
		/*printf( "兄弟%d 名字%sM" ,bonei ,child->name );
		matrix_print(str ,M );
		Debug();*/
		while ( sibling != NULL ) {
			glPushMatrix();
			//Debug();
			BuildIKtree( p ,sibling );
			glPopMatrix();
			sibling = sibling->sibling;
		}
	}
	
	
}
void 
Motion::DrawNextPosture() {
	if ( draw_Posture < m_NumFrames )
		draw_Posture++;
}
//畫IK的點以防止出問題
void 
Motion::Draw() {
	
	Posture* p = GetPosture( draw_Posture);
	BuildIKtree( *p ,pActor->getRoot() );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float mcolor[4] = { 1.0 ,0.0 ,0.0 ,1.0};
	for ( int i = 0; i< pActor->NUM_BONES_IN_ASF_FILE; i++ ) {
		
		glPushMatrix();

		glTranslatef( p->bone_position[i].x ,p->bone_position[i].y ,p->bone_position[i].z );
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mcolor);
		glutSolidSphere(1.0 ,20 ,10 );
		glPopMatrix();
		
	}
	glPopMatrix();
}