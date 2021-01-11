#include <iostream>
#include "function.h"
#include "callBackFunction.h"
#include "ctype.h"
#include "math.h"


using std::cout;

extern int windowheight;
extern int windowwidth;
extern Camera* eye;

void 
setCallBackFunc() {
	glutKeyboardFunc(keyboard);//��keyboard������
	glutSpecialFunc(specialkey);
	glutMouseFunc(mouse);//��mouse ������
	glutMotionFunc(motion);//��mouse���ʦ�����
	glutDisplayFunc(display);//��ܥ�
	glutReshapeFunc(reshape);
}

enum Mode { DEFAULT = 0 ,P1 ,P2  };//���{���������Ҧ�
Mode mode =  DEFAULT;

void display() {

	//�]�wmodel view
	setModelView();
	
	//�]�wperceptive project 
	setPerspectiveView();

	//�]�wclip the view 
	glViewport(0, 0, windowwidth, windowheight);

	//�Ҧ��ĪG�٭�
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glUseProgramObjectARB(0);//�]�^�w�]��
	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);	

	
	//�ھڼҦ��]�wpolygon
	switch ( mode) {
		case DEFAULT:
			drawPolygon( false);
			break;
		case P1:
			if (openPhongShading) {
				glUseProgramObjectARB( phongShading );
			}
			else {
				glUseProgramObjectARB(0);//�]�^�w�]��
 			}
			
			program1();
			break;

		case P2:
			if ( openBumpmapping) {
				glUseProgramObjectARB( bumpMapping );
			}
			else {
				glUseProgramObjectARB(0);//�]�^�w�]��
 			}
			program2();
			break;
	}
		
	

	glutSwapBuffers();//double buffer�ϥ�

}

static double dispx = 0;
static double dispy = 0;
static double dispz = -23;
static int DeltaR = 30;
static double theta = 0;//���F�H��ƹ��ϥ�
static double htheta = 0;//camera��������
static double vtheta = 0;//camera��������
static double modeltheta = 0;
static bool zoommode = false;
static bool camerar = false;//Camera�O�_����
/**
  ��L�]�w�D�n����camera
      Q W E R
	  A S D

	  Z  
  W�V�W���ʩ�Zoom in Camera�V�W���
  A�V�k���� Camera�V�k���
  D�V������ Camera�V�����
  S�V�U���ʩ�Zoom out Camera�V�U���
  Q model�V��rotate
  E model�V�krotate
  R�N��L�������ϥ�Camera����\��
  
  �Y�Q�nZoom in Zoom out�Ы��U Z
*/
void keyboard(unsigned char key, int x, int y) {
	char k = toupper(key);//�N�Ҧ��ȧ令�p�g 

	switch (k) {
		case 'W'://�V�W���� ����V�e����
			if ( zoommode ) dispz--;
			else if ( camerar ) {
				htheta += DeltaR;
				
			}
			else dispy++;
			break;
		case 'A'://�V������
			if ( camerar ) 
				vtheta -= DeltaR;
			else dispx--;
			break;
		case 'D'://�V�k����
			if ( camerar ) 
				vtheta += DeltaR;
			else dispx++;
			break;
		case 'S'://�V�U����
			if ( zoommode ) dispz++;
			else if ( camerar ) 
				htheta -= DeltaR;
			else dispy--;
			break;
		case 'Q':
			modeltheta -=DeltaR;
			break;
		case 'E':
			modeltheta +=DeltaR;
			break;
		case 'Z':
			zoommode = (zoommode)? false : true;
			if (zoommode) cout<<"�{�b�i�Hzoom in zoom out\n";
			else cout<<"�^��W�U���k mode\n";
			break;
		case 'R':
			camerar = (camerar)? false : true;
			if ( camerar) cout<<"�i����camera\n";
			else cout<<"�^��W�U���kmode\n";
			break;
		case 27:
			exit(0);
			break;
	}
	
	glutPostRedisplay();
	
}


void specialkey ( int key ,int x ,int y) {
	system("cls");

	switch ( key ) {
		case GLUT_KEY_F1:
			if ( openBumpmapping )
				glUseProgramObjectARB(0);//�]�^�w�]��
			cout<<"�ثe�B��Gouraud shading program\n";
			openPhongShading = false;
			mode = P1;
			break;

		case GLUT_KEY_F2:
			cout<<"�ثe�B�� Gouraud shading with texture\n";
			if ( openPhongShading )
				glUseProgramObjectARB(0);//�]�^�w�]��
			openBumpmapping = false;
			mode = P2;
			break;

		case GLUT_KEY_F3:
			cout<<"�ثe�B�� Bump mapping\n";
			openBumpmapping = true;
			mode = P2;
			break;

		case GLUT_KEY_F4:
			cout<<"�ثe�B��Phong Shading\n";
			openPhongShading = true;
			mode = P1;
			break;
		default:
			mode = DEFAULT;
			break;
	}
	cout<<"opernPhong= "<<openPhongShading <<" openBumpmapping = "<<openBumpmapping<<" mode ="<<mode <<"\n";
	glutPostRedisplay();
}
/*
 �ƹ��������ƥ� 
 1.���U����즲�i�W�U���k����
 2.���U�k��즲�i����Camera
   �Y���UCtrl��i����model
 *
*/
static int startx = 0;
static int starty = 0;
const int NOTANUMBER = -1;
const double PI = 3.1415926;
static int prex = NOTANUMBER;
static int prey = NOTANUMBER;
static bool setmodel = false;//model�O�_����
void mouse(int button, int state, int x, int y) {
	if ( button == GLUT_MIDDLE_BUTTON ) {//������button�Ψ�Zoom in zoom out
		
	}
	else if ( button == GLUT_LEFT_BUTTON ) {//����
		if ( state == GLUT_DOWN ) {//���U
			startx = x;
			starty = y;
			printf("(%d ,%d )" , x ,y);
		}
		else {
			prex = NOTANUMBER;
			prey = NOTANUMBER;
		}
			
	}
	else {//�k��
		if ( state == GLUT_DOWN ) {
			startx = x;
			starty = y;
			if ( glutGetModifiers() == GLUT_ACTIVE_CTRL ) {
				setmodel = true;
				cout<<"model����}�l\n";
			}
			else {
				camerar = true;
				cout<<"Camera����}�l\n";
			}
		}
		else {
			cout<<"�k��Ѱ�\n";
			prex = NOTANUMBER;
			prey = NOTANUMBER;
			camerar = false;
			setmodel = false;
		}
	}

	//glutPostRedisplay();

}

void motion(int x, int y) {

	//����θ��
	int vpx = prex - startx;//���e���V�q
	int vpy = prey - starty;
	int vnx = x - startx;//�{�b���V�q
	int vny = y - starty;

	//�Y�Oprex�S����� �N�]�����U�h��(x ,y)
	if (prex == NOTANUMBER ) prex = startx;
	if (prey == NOTANUMBER) prey  = starty;
	//printf("new (%d ,%d )\n" , x ,y);
	if ( !camerar && !setmodel) {//���ncamera�����
		
		dispx = dispx + (x - prex)/3;
		dispy = dispy + (prey - y)/3;
		printf("new (%d ,%d )\n" , x ,y);
		printf("dispx dispy = %lf %lf \n",dispx ,dispy);

	}
	else if ( camerar ){ //camera���� 
		/*�b�����W�H���y�T���Ϊ��覡 �ڭ̥i�H�D�ohtheta �H��vtheta*/
		//���o�y��b�|
		double r = eye->distanceToObject();
		//�Dhtheta�H��vtheta�q
		vpx = x - prex;
		vpy = y - prey;
		double cost = ( r * r - vpx * vpx) / (2 * r * r);
		if ( -1 <= cost && cost <= 1) {
			double theta= 180 * acos ( cost ) /PI;
			/*cout << r <<"\n";
			printf("( x , y) = %d ,%d ( prex ,prey) = %d ,%d (vpx ,vpy) = %d %d " ,x ,y ,prex ,prey ,vpx ,vpy);
			cout<<"�������:"<<theta<<" "<< acos ( cost ) << " " << cost << "\n";*/
			htheta = htheta + ( theta )/39;
		}
		cost = ( r * r - vpy * vpy) / (2 * r * r);
		if ( -1 <= cost && cost <= 1) {
			theta= 180 * acos ( cost ) /PI;
			//cout<<"�������:"<<theta<<" "<< acos ( cost ) <<" "<< cost <<"\n";
			vtheta = vtheta + ( theta )/39;
			/*printf("htheta ,vtheta = %lf %lf \n" ,htheta , vtheta ); 
			cout<<" ===============\n";*/
		}
	}
	else if ( setmodel ) {//model����
		static int m = 0;
		if ( (m++) % 25 == 0 ) {
			//�~�n>=0�f������V���� �~�n<0��������V����
			double crossproduct = ( vpy ) * ( vnx )  - ( vny ) * ( vpx);

			if ( crossproduct >= 0 )  modeltheta += DeltaR;
			else modeltheta -= DeltaR;
		}
	
	}

	prex = x;
	prey = y;
	glutPostRedisplay();
}

void reshape(GLsizei w,GLsizei h) {
	windowheight = h;
	windowwidth = w;
}
/*
   �]�wmodel view
   OpenGL�|�@������᭱�h
   teapot�b�]�wLookat���ɭ��`�O�ǩǪ� �ҥH�ڭ̤��ϥ�Lookat�H��K�e�{
   �L���y���ഫ�@�����ڭ̫ܧx�Z
   z
   |
   |
   |
   |
   ------------------->x
  

   �D�n�O model�� ���� camera��(camera�H��������A�H��������
*/
void setModelView() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 

	printf("%lf %lf %lf\n" ,dispx ,dispy ,dispz );
	glTranslatef(dispx , dispy ,dispz);//camera����
	
	gluLookAt( eye->getEyeX()     ,eye->getEyeY()    ,eye->getEyeZ() //eye
			  ,eye->getCenterX()  ,eye->getCenterY() ,eye->getCenterZ()//center of object
			  ,eye->getUpX()      ,eye->getUpY()     ,eye->getUpZ()    ); //up  

	glRotatef(modeltheta ,0.0 ,0.0 ,1.0);
	
	glRotatef( htheta, 1.0 ,0.0, 0.0 );//����x�b����
	glRotatef( vtheta, 0.0, 1.0, 0.0 );//�b��z�b���� M = T * Rm * L * Rx * Rz
}
/*�]�wperspectve*/
void setPerspectiveView() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)windowwidth/(GLfloat)windowheight, 0.1 ,30);
}
