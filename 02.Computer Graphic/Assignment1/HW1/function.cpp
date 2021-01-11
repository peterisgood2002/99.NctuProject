#include "function.h"
#include <iostream>

using std::cout;
using std::string;
extern mesh* model;

/*�Nmodel�e�X��
  �ǤJ�G�O�_�[�Wtexture
*/
void 
drawPolygon( bool withtexture ) {

	glClearColor(0.0f,0.0f,0.0f,0.0f);//�]�wbackground color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	

	//�e��
	for (int i=0;i < model->fTotal;i++)
	{
		glBegin(GL_POLYGON);
		for (int j=0;j<3;j++)
		{
			//�]Texture�y��
			if ( !withtexture) glTexCoord2fv( model->tList[ model->faceList[i][j].t ].ptr );
			else {
				glMultiTexCoord2fv(GL_TEXTURE0_ARB,  model->tList[ model->faceList[i][j].t ].ptr );
				glMultiTexCoord2fv(GL_TEXTURE1_ARB,  model->tList[ model->faceList[i][j].t ].ptr );
			}

			//�]�I��normal
			glNormal3fv( model->nList[model->faceList[i][j].n].ptr );
			//�]�I
			glVertex3fv( model->vList[model->faceList[i][j].v].ptr );	
		}
		glEnd();
	}
	
}

/*
 ��@�~�@���I�s
*/
void 
program1() {
	cout<<"�ثe�i��lighting�Mmaterial \n";
	drawPolygonWithoutTexture();
}

/*
 ��@�~�G���I�s
 */
void 
program2() {
	drawPolygonWithTexture();
}
///*
// ��@�~�T���I�s
// */
//void 
//program3() {
//	bumpMapping();
//}

/*�]�w�Ѽƥ�*/
static bool setting = true;
void 
lighting() {
	//�]�w����
	static GLfloat mat_diffuse[] = {0.2, 0.2, 0.6, 1.0};
	static GLfloat mat_specular[] = {0.6, 0.6, 0.6, 1.0};
	static GLfloat mat_ambient[] = {0.6, 0.6, 0.6, 1.0};
	static GLfloat mat_shininess = 160.0;
	//�]�w���u
	static GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	//�]�w���u�V�q
	static GLfloat light_position[] = {0.0 ,20.0 ,27 , 1.0};

	if (! setting ) {
		setMaterialParameter(mat_diffuse ,mat_specular ,mat_shininess ,mat_ambient );
		setLightingParameter(light_diffuse ,light_specular ,light_ambient );
		cout<<"�O�_�]�w���u�V�q��default�Ȭ�({0.0 1.0 0.0 )(Y/N)�G";
		char yesno = 'n';
		cin >> yesno;
		if ( yesno == 'Y' || yesno == 'N' ) {
			cout<<"�п�J���u�V�q(x , y , z) = ";
			cin >> light_position[0] >> light_position[1] >>light_position[2];
		}
		setting = true;
	}
	
	glShadeModel( GL_SMOOTH );

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//Cl
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//Csl
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//Ca

	// set material property
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);//Cr   c = Cr  Cl  (n * r)
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);//Cs
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);//Cr'  c = Ca Cr' 
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);//p c = Csl Cs (h * n) ^ p
}

//�]�w���説�p
void 
setMaterialParameter(GLfloat* mat_d ,GLfloat* mat_s ,GLfloat mat_sh ,GLfloat* mat_a ) {
	char yesno = 'n';
	cout<<"�г]�w���������C��\n";
	cout<<"\t1.�O�_�]�wdiffuse�����C��Ψϥ�default value(RGBA = 1 ,0 ,0 ,1)(Y/N)�G";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tDiffuse�����C��(RGBA)�G";
		cin >> mat_d[0] >>  mat_d[1] >>  mat_d[2] >>  mat_d[3];
	}
	cout<<"\t2.�O�_�]�wSpecular�����C��Ψϥ�default value(RGBA = 0, 0, 1, 1)(Y/N)�G";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tSpecular�����C��(RGBA)�G";
		cin >> mat_s[0] >>  mat_s[1] >>  mat_s[2]  >>  mat_s[3];
	}
	cout<<"\t3.�O�_�]�wshinnig�{�שΨϥ�default value(70)(Y/N)�G";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tshinnig�{�סG";
		cin >> mat_sh;
	}

	if ( mat_a != NULL ){
		cout<<"\t4.�O�_�]�wambient�����C��Ψϥ�default value(RGBA = 0, 1, 0, 1)(Y/N)�G";
		cin >> yesno;
		yesno = tolower(yesno);
		if ( yesno =='y' ) {
			cout<<"\tambient�����C��(RGBA)�G";
			cin >> mat_a[0] >>  mat_a[1] >>  mat_a[2]  >>  mat_a[3];
		}
	}
	
}

//�]�w���u���p
void 
setLightingParameter(GLfloat* light_d  ,GLfloat* light_s ,GLfloat* light_a) {
	char yesno = 'n';
	cout<<"�г]�w�������u�C��\n";
	cout<<"\t1.�O�_�]�wdiffuse���u�C��Ψϥ�default value(RGBA = 1 ,1 ,1 ,1)(Y/N)�G";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tDiffuse���u�C��(RGBA)�G";
		cin >> light_d[0] >>  light_d[1] >>  light_d[2] >>  light_d[3];
	}
	cout<<"\t2.�O�_�]�wSpecular���u�C��Ψϥ�default value(RGBA = 1 ,1 ,1 ,1)(Y/N)�G";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tSpecular���u�C��(RGBA)�G";
		cin >> light_s[0] >>  light_s[1] >>  light_s[2] >>  light_s[3];
	}

	if ( light_a != NULL ) {//�p�G��ambient�ѼƮɨϥ�
		cout<<"\t3.�O�_�]�wAmbient���u�C��Ψϥ�default value(RGBA = 0 ,0 ,0 ,0)(Y/N)�G";
		cin >> yesno;
		yesno = tolower(yesno);
		if ( yesno =='y' ) {
			cout<<"\tAmbient���u�C��(RGBA)�G";
			cin >> light_a[0] >>  light_a[1] >>  light_a[2] >>  light_a[3];
		}
	}
}