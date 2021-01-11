/******************************************
FILE
    scence.h
Purpose
    �t�d�������@�ǳ]�w �åBrender�X����

NOTE
	

Author
    9757553 �C����
*******************************************/

#include "Scene.h"
#include "function.h"
#include "glut.h"
#include "Struct.h"
#include <iostream>
#include "math.h"

using std::cout;

Scene::Scene(const char* const* filename ,const int size) : marray(size) {

	for ( int i=0; i<size;i++ ) {
		mesh* m = new Model( filename[i] );
		marray[i] = m;
	}

}

Scene::~Scene() {
	for ( unsigned i = 0 ;i < marray.size(); i++)
		delete marray[i];
	
	
}

/*�ھ�mode����ܳ���
  �ǤJ�G�n�H�ƻ�mode���
*/
void 
Scene::display(Mode mode) {
	switch ( mode ) {
		case DLIGHTING:
			directLighting();
			break;
		case MIRROR:
			directLighting(true ,2);//��y��mirroring�ĪG
			break;
		case INDIRECT:
			indirectLighting();
			cout<<"���鰵indirect lighting\n";
			break;
		case DEFAULT:
		default:
			defaultDisplay();
			break;
	}
}

void 
Scene::defaultDisplay() {

	initCanvas();
	lighting();
	
	for ( unsigned i=0; i<marray.size(); i++) {
		mesh* model = marray[i];

		//�e�Xmodel
		for ( int i = 0; i < model->fTotal; i++ ) {
			glBegin(GL_POLYGON);
				for (int j=0;j<3;j++)
				{
					if ( model != NULL) {
						//�]�w��������
						material m = model->mat[ model->faceList[i][0].m ];
						glMaterialfv(GL_FRONT, GL_SPECULAR, m.Ks);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, m.Kd);
						glMaterialfv(GL_FRONT, GL_AMBIENT, m.Ka); 
						glMaterialf(GL_FRONT, GL_SHININESS,m.Ns);
						
						glTexCoord2fv( model->tList[ model->faceList[i][j].t ].ptr );
						//�]�I��normal
						glNormal3fv( model->nList[ model->faceList[i][j].n].ptr );
						//�]�I
						glVertex3fv( model->vList[ model->faceList[i][j].v].ptr );	
					}
				}
			glEnd();
		}
	}

}
/*���oRay������I�����I
  �ǤJ�GRay 
  �Ǧ^�G���L���� ���쪺mesh�Mface�b����
*/
const double* 
Scene::RayToPoint(Ray& r ,int& modeli ,int& facei ) {
	
	double* vec = NULL;

	//��Ҧ���model�������L�I�� �T�w�쩳���쪺�O�ƻ򰭪F�� �è��o�O���@��model�����@�ӭ��Q����
	for ( unsigned i = 0; i < marray.size(); i++ ) {
	
		Model* model = (Model*) marray[i];
		double tface = model->collideFace(r ,par->getNearPlane() ,par->getFarPlane() );
		if ( tface != Model::NOTANUMBER ) {//��ܦ�����F��
			facei = tface;
			modeli = i;
		}
	}
	
	if ( modeli != -1 && facei != Model::NOTANUMBER ) {
		vec = new double[3];
		for ( int i = 0; i < 3; i++ )
			vec[i] = r.e[i] + r.t * r.b[i];
		
	}
	return vec;
}

/*�i��direct Lighting
  �ǤJ�G�O�_�n��Y�@��model����mirror�ĪG
*/
void 
Scene::directLighting( bool ismirror ,int mi) {

	initCanvas();

	double lp[3] = { 0 };//���u�y��
	for ( int i = 0; i < 3; i++ )
		lp[i] = par ->getLight().lightp[i];
	/*�x�spixel color��*/
	int total = par->getGLWindowWidth() *par->getGLWindowHeight() * 3;
	float* color = new float[ total];
	for ( int i = 0; i < total; i++ )
		color[i] = 0;

	cout<<"Direct Lighting�}�l\n";
	for ( int j = 0; j < par->getGLWindowHeight(); j++  ) {//��C�@���I�i��render
		for ( int i = 0; i < par->getGLWindowWidth(); i++) {	
			//int i = 386,j =152;
			
			Ray r(i , j ,par );//���ͤ@���g�u
			
			const Color* c = getDLColor( r ,lp , ismirror  ,mi);

			if ( c != NULL ) {
				printf("%d %d �ݭn�W�C�� \n" ,i ,j);
				//�W���k�O���I�b���U��
				/*
				 /|\
				  |
				  |
				  --------------->
				  �ҥHj�n����
				*/
				int cell = ( par->getGLWindowWidth()) * (par->getGLWindowHeight() - j ) + i;
				
				color[ cell * 3 ] = c->r;
				color[ cell * 3 + 1] = c->g;
				color[ cell * 3 + 2] = c->b;
				//cout<<"==============================\n";

				
			}

			delete c;
		}
	}
	cout<<"Direct Lighting����\n";
	glDrawPixels(par->getGLWindowWidth() ,par->getGLWindowHeight() ,GL_RGB ,GL_FLOAT ,color); 
}
/*indirect lighting
  �ǤJ�G�O�_�n��Y�@��model����shadow�ĪG
*/
void 
Scene::indirectLighting( ) {
	initCanvas();

	double lp[3] = { 0 };//���u�y��
	for ( int i = 0; i < 3; i++ )
		lp[i] = par ->getLight().lightp[i];
	/*�x�spixel color��*/
	int total = par->getGLWindowWidth() *par->getGLWindowHeight() * 3;
	float* color = new float[ total];
	for ( int i = 0; i < total; i++ )
		color[i] = 0;

	cout<<"Indirect Lighting�}�l\n";
	/*----------------Begin Indirect Lighting----------------------*/
	for ( int j = 0; j < par->getGLWindowHeight(); j++  ) {//��C�@���I�i��render
		for ( int i = 0; i < par->getGLWindowWidth(); i++) {
			//int i = 134,j =79;
			
			Ray r(i , j ,par );//���ͤ@���g�u
			int modeli = -1 ,facei = -1;
			const double* vec = RayToPoint( r ,modeli ,facei );//���g�u��X�������I
			const Color* c = getDLColor( r ,lp ,true ,2);//����X���쪺�I�L���C���
			Color* obil = NULL;
			if ( vec != NULL ) {//��ܦ�����F��
				/*�s�@�Ϯg�u�� �еy��......*/
				Model* model = (Model*) marray[ modeli ];
				const double* normal = model->getNormal( vec ,facei );//�H�����覡�p��normal
				
				double ptoe[3] = { 0 };//�V�qpe
				for ( int i = 0; i < 3; i++ ) 
					ptoe[i] = r.e[i] - vec[i];
				double l = sqrt( ptoe[0] * ptoe[0] + ptoe[1] * ptoe[1] + ptoe[2] * ptoe[2] );//normalize
				for ( int i = 0; i < 3; i++ )
					ptoe[i] = ptoe[i] / l;

				Ray tmp = reflection( vec ,ptoe ,normal );
				/*�Ϯg�u�s�@����*/
				/*�NColor�[�W�s�������^�m��
				  �s�������^�m�q=�s�������j�� *�n�e�X���I��diffuse * cos(ray��V�P���I�k�V�q����)  
				*/
				const Color* newlight = getDLColor(tmp ,lp ,true ,2);//�������C��
				if ( newlight != NULL ) {
					const Color* d = model->getDiffuse(vec ,facei );
					double cost = innerProduct ( normal , tmp.normalizeB() );//Cos theta
					obil = new Color();

					obil->r = newlight->r * d->r * cost * 0.5;
					obil->g = newlight->g * d->g * cost * 0.5;
					obil->b = newlight->b * d->b * cost * 0.5;

					delete d;
				}

				//model->setDefault();//�]�^�w�]
				delete newlight;
				
					
			}

			delete vec;
			/*�W��F�u�}��!!*/
			int cell = ( par->getGLWindowWidth()) * (par->getGLWindowHeight() - j ) + i;
			if ( c != NULL ) {
				printf("%d %d �ݭn�W�C�� \n" ,i ,j);
				//�W���k�O���I�b���U��
				/*
				 /|\
				  |
				  |
				  --------------->
				  �ҥHj�n����
				*/
				
				color[ cell * 3 ] += c->r;
				color[ cell * 3 + 1] += c->g;
				color[ cell * 3 + 2] += c->b;
				cout<<"==============================\n";
			}
			if ( obil != NULL ) {
				color[ cell * 3 ] += obil->r;
				color[ cell * 3 + 1] += obil->g;
				color[ cell * 3 + 2] += obil->b;
			}

			
			delete c;
			delete obil;
		}
		
	}/*----------------End Indirect Lighting----------------------*/
	cout<<"Inirect Lighting����\n";
	glDrawPixels(par->getGLWindowWidth() ,par->getGLWindowHeight() ,GL_RGB ,GL_FLOAT ,color);

}
/*�o����I��Direct light��color
  �ǤJ�G�@��Ray Light����m �n���n��Y��mesh����shadow
  �Ǧ^�GRay����F�誺�C��� �p�G���ݭn�W�C�� �N�|�^��NULL
*/
const Color*
Scene::getDLColor(Ray& r ,const double* lp ,bool ismirror ,int mi ) {
	int modeli = -1 ,facei = -1;
	const double* vec = RayToPoint( r ,modeli ,facei );//���g�u��X�������I
	
	/*�e�C��*/	
	const Color* c = NULL;

	if ( vec != NULL) {
		//��Xshadow ray
		Ray sr( vec , lp );//�ݩ�u�q

		//�ˬdshawdow ray�O�_�P�����o�͸I�� �åB����sr.t�Ȥw�Q����p��
		Model* model = NULL;
		for ( unsigned t = 0; t < marray.size(); t++ ) {
			model = (Model*) marray[t];
			model->collideFace(sr ,par->getNearPlane() ,par->getFarPlane() );
		}

		/*����*/
		model = ( Model* ) marray[ modeli ];
		
		if ( sr.t < 1 ) //��ܪ���b���u�M�I���� �h���I�O���v���� �n�O�t��
			c = model->getPtrColor( r ,vec ,facei ,par->getLight() , true );
		else {//���� �ש󰵨�o�@�B�F
			if ( ismirror && modeli == mi ) //�p�G���nmirror�B�S��n�O�o��model�Q���� �Nmirror
				c = model->getPtrColor( r ,vec ,facei ,par->getLight() , false ,true ,this);
			else c = model->getPtrColor( r ,vec ,facei ,par->getLight() ,false );
		}
		//printf("�䥴�쪺�I�� %lf %lf %lf \n" ,vec[0] ,vec[1] ,vec[2] );
		//printf("���C��Ȭ�%f %f %f %f \n" ,c->r ,c->g ,c->b ,c->a);
	}

	delete vec;
	return c;
}
/*
�N�Y�ӳ����i�業���ʧ@
�ǤJ�G�������ӳ��� �����h��
*/
void 
Scene::tMeshTo(int i ,double to[3] ) {
	mesh* model = marray[i];
	
	for ( int i=0; i<model->vTotal;i++) {
		model->vList[i].ptr[0] += to[0];
		model->vList[i].ptr[1] += to[1];
		model->vList[i].ptr[2] += to[2];
	}

}



///*�i���ƴ��ե�*/
//void 
//Scene::test(Ray & r) {
//		Model* m = ( Model*) marray[0];
//		int j = m->collideFace(r ,1 ,1000);
//
//		if ( j != Model::NOTANUMBER ) {
//			cout<<"�A���쪺���O"<<j<<" ";
//			for ( int i = 0; i<3;i++ )
//				cout<<r.e[i] + r.t * r.b[i]<<" ";
//			cout<<"\n";
//		}
//
//	
//
//}