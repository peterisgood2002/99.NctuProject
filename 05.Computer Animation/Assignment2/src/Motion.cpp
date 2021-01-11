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

	/*�}�lŪ�J��� ������Ū�L�@�M �T�w�L���h��Frame*/
	char line[1024] ="";
	int frame = 0;
	while ( !input.eof() ) {
		input.getline( line ,1024 );
		int i = atoi( line );
		if ( i != 0 ) frame = i;
	}

	//�p�G�M�ثe�����@�ˤj �B�w�g���ثe��� �h���s�]�w
	if ( m_NumFrames != frame && m_pPostures != NULL ) {
		delete [] m_pPostures;
		m_pPostures = NULL;
	}
	
	m_NumFrames = frame;
	if ( m_pPostures == NULL ) {
		m_pPostures = new Posture[ m_NumFrames ];

		SetPosturesToDefault();
	}
		
	
	
	
	/*�}�lŪ�J�򥻸�� ��Ū�J�����n���O�h�X�Ӫ��T��*/
	input.clear();//�M��EOF����
	input.seekg( 0 ,ios::beg );//���ƶ}�Y��Ū
	for ( int j = 0; j < 3;	j++ ) {
		input.getline( line , 1024 );
		//printf( "%s\n", line );
		
	}
	string keyword = "";
	Bone* root = pActor->getRoot();
	do {
		//Ū�J���
		input >> frame;
		Posture* p = &m_pPostures[frame-1];

		//�N�Ҧ��i���ʪ���ƶ�J
		for ( int i = 0; i < pActor->MOV_BONES_IN_ASF_FILE; i++ ) {
			
			input >> keyword;
			int boneid = pActor->name2idx( keyword.c_str() );
			Bone* bone = pActor->getBone( root ,boneid );
			
			//Ū�J�Ҧ�����
			//�qTx Ty Tz�}�lŪ�_��Rx Ry Rz
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
	/*�z�LForward Kinemetic�p�� ���p��X�Ĥ@��frame�������m ���x�s�bPosture��*/
	Bone* root = pActor->getRoot();

	//��C�@��Time Step �]�N�O�C�@��frame
	for ( int t = 0; t < m_NumFrames - 1; t++ ) {
		//1.���o�ثeFrame
		Posture* nowframe = GetPosture(t);
		//2.��X���Posture���@�ɮy�Ц�m
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		BuildIKtree( *nowframe ,root );
		glPopMatrix();

		/*
		  3.�]�wConstraint �N�k�����Y�W
			V = J  * theta
			�ثe�u���ܤT�ӪF�� �N�u�O���F��K
				[ G - Ex ]
			V = [ G - Ey ]
				[ G - Ez ]
		*/
		//�]�w�ت��a �]�N�O�]�wConstraint
		int gi = pActor->name2idx("upperneck" );//�Y��index = G
		int ei = pActor->name2idx( "rhand" );//�k�⪺hand = E
		//int gi = pActor->name2idx( "upperback" );
		//int ei = pActor->name2idx( "lthumb" );
		//�p��V�x�}
		double v[3] = { 0 };
		v[0] = nowframe->bone_position[gi].x - nowframe->bone_position[ei].x;
		v[1] = nowframe->bone_position[gi].y - nowframe->bone_position[ei].y;
		v[2] = nowframe->bone_position[gi].z - nowframe->bone_position[ei].z;
		VectorRn V( 3 );
		V.Load(v);
		//printf("%lf ,%lf ,%lf\n" ,v[0] ,v[1] ,v[2] );
		/*
		  4.����Jacobian Matrix
			Jacobian Matrix����²��
			�C�@���� ��Joint��End Effector���ĪG
			�qrclavicle �}�l��Jacobian

		  5.�ӱ���b�ӫ��M�w�O ���ڭ̬ݤ@�U���� �i�H�o�{
		     R1  * R2  * ........* Ri+1 = Ri+1
			  0     1                i      0
		    �o�˧ڭ̴N�i�H�o�{�@��Ʊ� ������b�N�O�A��Global����x�}���T��

		*/
		//����Ӱ��[���h�֭Ӧۥѫ�
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

		//Jacobian��column�����ӬO�Ҧ����ۥѫ�
		MatrixRmn J( V.GetLength() , dof);
		J.SetZero();
		double R[4][4]= { 0 } ,Rx[4][4] = { 0 } ,Ry[4][4] = { 0 } ,Rz[4][4] = { 0 } ,tmpR[4][4] = { 0 };
		double axis[3] = { 0 };//�N�����b
		double PiE[3] = { 0 };
		double cp[3] = { 0 };//Cross product�����G�s�b�o��
		parent = startbone;
		int ci = 0;
		while ( parent != NULL && parent->idx != ei ) {
			//��� Pi - E
			//printf("%d dof ,%s = %d\n" ,dof ,parent->name ,ci );
			int bonei = parent->idx;
			PiE[0] = nowframe->bone_position[ei].x - nowframe->bone_position[bonei].x;
			PiE[1] = nowframe->bone_position[ei].y - nowframe->bone_position[bonei].y;
			PiE[2] = nowframe->bone_position[ei].z - nowframe->bone_position[bonei].z;
			//��X����b
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
			//���M����b��Cross Product
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
		  5.�z�LPsuedo Inverse��� ���[�t��
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
		  �z�L�{�b��Frame ��U�@��Frame�i��ץ����o�F ThetaV�� �ڭ̥i�H�C�C�����ܨ���
		  �������ܨ���
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

			//��U�@��Child
			Bone* child = parent->child;
			if ( child == NULL )
				child = parent->sibling;
		
			parent = child;
		}
	}

	printf("�ק粒��\n");
	
	

	
}

/*
  �z�LFK�D�X�C�@��Bone����m
  �MDrawBone�t���h���ʧ@ �ڭ̨ϥ�OpenGL��Push�MPop��K �n���M�ӧU�е����o�ظ�Ƶ��c �i��n�ۤv�g�N�W�·Ъ�
  �åB�{���O�ھڽҥ� Figure 5.13���t��k
*/
void Debug() {
	/*Debug��*/
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
	/*Debug��*/
}
void 
Motion::BuildIKtree(Posture& p ,const Bone* b)
{
    /*
	  1.���p��  R i+1  = R asf   *  R
				   i        i         amc
	*/
	
	glMultMatrixd((double*)&b->rot_parent_current);  //�N�ثe��Matrix���W  iRasf  
	//��ثe��Matrix���۹���������
	int bonei = pActor->name2idx( b->name );
	
	
	if( b->doftz ) glTranslatef( 0. ,0. ,p.bone_translation[bonei].z );//����
	if( b->dofty ) glTranslatef( 0. ,p.bone_translation[bonei].y ,0. );
	if( b->doftx ) glTranslatef( p.bone_translation[bonei].x ,0. ,0. );
	if( b->dofz ) glRotatef( p.bone_rotation[bonei].z , 0., 0., 1.);//����
	if( b->dofy ) glRotatef( p.bone_rotation[bonei].y , 0., 1,  0.);
	if( b->dofx ) glRotatef( p.bone_rotation[bonei].x , 1., 0., 0.);
	/*
	  R i+1 �ǳƦn�F ���XMatrix   �]�ǳƦn R 0
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

	double pos[3] = { 0 };//���Y���ץH�Τ�V �H��K��U�@�Ӧ�m�h
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
		glPushMatrix();//��J�ثe��Matrix
		
		glTranslatef( dir[0] ,dir[1] ,dir[2] );//M = Rv + T  ���ܥثe��Matrix
		//Debug();
		BuildIKtree( p ,b->child );
		glPopMatrix();//��X�ثe��Matrix
	}
	if ( b->sibling != NULL ) {
		//glPopMatrix();//Pop�H��X�٨S�Q�[�iChild�ɪ�Matrix
		Bone* sibling = b->sibling;
		/*printf( "�S��%d �W�r%sM" ,bonei ,child->name );
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
//�eIK���I�H����X���D
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