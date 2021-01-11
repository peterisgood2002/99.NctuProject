/******************************************
FILE
    program.h
Purpose
    

NOTE
	

Author
    9757553 �C����
*******************************************/
#include "function.h"
#include "glut.h"

/*��l��Canvas�n�e�F��*/
void 
initCanvas() {

	glClearColor(0.0f,0.0f,0.0f,0.0f);//�]�wbackground color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);// �O�o�Ұʲ`�״���
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//���᪺matrix�i�Hpop��
		const Light light = par->getLight();
		glTranslatef( light.lightp[0] / light.lightp[3]  ,light.lightp[1] / light.lightp[3] ,light.lightp[2] / light.lightp[3] );
		
		glutSolidSphere(0.5, 10, 8);	// the Planet, ���ɪ��x�}�O I * ModelView * R * T * R
	glPopMatrix();//pop�̤W����matrix (�ѤW���T�ӯx�}�ۭ��Ө�),�|�^�� I * ModelView
	
}

/*default������*/
void 
lighting() {
	const Light light = par->getLight();
	//�]�w���u
	static const GLfloat* light_specular = light.lspecular;
	static const GLfloat* light_diffuse = light.ldiffuse;
	static const GLfloat* light_ambient = light.lambient;
	//�]�w���u�V�q
	static const GLfloat* lightp =  light.lightp;
	
	printf("������m�b(%f ,%f ,%f ,%f)\n" ,lightp[0] ,lightp[1] ,lightp[2] ,lightp[3] );

	glShadeModel( GL_SMOOTH );

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light.lightp);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//Cl
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//Csl
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//Ca

}

/*inner product*/
double 
innerProduct(const double* v1 ,const double* v2) {
	double ip = 0;
	for ( int i = 0; i < 3; i++ )
		ip += v1[i] * v2[i];

	return ip;
}
/*�Ϯg
  �ǤJ�G�_�l��m �J�g�u �k�u
  �ǥX�G�Ϯg�u
*/
Ray
reflection( const double * p ,const double* ptoe ,const double* normal ) {
	double ray[3] = { 0 };
	double dn = innerProduct( ptoe , normal );
	for ( int i = 0; i < 3; i++ ) 
		ray[i] = 2 * dn * normal[i] - ptoe[i];
	//��X���I�y��
	for ( int i = 0; i < 3; i++ )
		ray[i] += p[i];
	
	Ray tmp( p ,ray ,true , 100000);//�ݩ�Ray�ӫD�u�q

	return tmp;
}
