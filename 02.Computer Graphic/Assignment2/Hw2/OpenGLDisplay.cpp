/******************************************
FILE
    CallBackFunction.h
Purpose
    �Ҧ�Call back function�H�ΩҦ�Call back function�ݭn�Ψ쪺�禡

NOTE
	

Author
    9757553 �C����
*******************************************/
#include <iostream>
#include "OpenGLDisplay.h"


using std::cout;

OpenGLParameter* par;
Scene* scene;


Mode mode = DEFAULT;

/*��l�ƾ��OpenGL�{��
  �ǤJ�GOpenGL�ѼƩM����
 */
void 
Initialize(int argv ,char** argc ,OpenGLParameter& p ,Scene& s) {
	par = &p;
	scene = &s;
	glutInit( &argv ,argc);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH );
	glutInitWindowSize( par->getGLWindowWidth() , par->getGLWindowHeight() );
	glutInitWindowPosition( par->getGLWindowX() ,par->getGLWindowY() );
	
	glutCreateWindow("HW2");
}
void 
setCallBackFunc() {
	glutKeyboardFunc(keyboard);//��keyboard������
	glutSpecialFunc(specialkey);
	glutMouseFunc(mouse);//��mouse ������
	glutMotionFunc(motion);//��mouse���ʦ�����
	glutDisplayFunc(display);//��ܥ�
	glutReshapeFunc(reshape);
}
#include "function.h"

/*display scene*/
void display() {
	
	//�]�wmodel view
	setModelView();
	
	//�]�wperceptive project 
	setPerspectiveView();

	//�]�wclip the view 
	glViewport(0, 0, par->getGLWindowWidth(), par->getGLWindowHeight() );

	
	//�ھ�mode�e����
	scene->display(mode);

	glutSwapBuffers();//double buffer�ϥ�

}
/*response the keyboard*/
void keyboard(unsigned char key, int x, int y) {
	char k = toupper(key);//�N�Ҧ��ȧ令�p�g 
	static bool zoomlight = false;
	switch (k) {
		case 'W'://�V�W���� ����V�e����
			if ( zoomlight ) {
				par->mLightByDelta( 0 ,0 , -1);
			}
			else par->mLightByDelta( 0 ,1 ,0 );
			break;
		case 'A'://�V������
			par->mLightByDelta(-1 ,0 ,0 );
			break;
		case 'D'://�V�k����
			par->mLightByDelta(1 ,0 ,0 );
			break;
		case 'S'://�V�U����
			if ( zoomlight ) par->mLightByDelta(0 ,0 ,1 );
			else par->mLightByDelta(0 ,-1 ,0 );
			break;
		case 'Z':
			zoomlight = ( zoomlight )? false : true;
			if (zoomlight) cout<<"�{�b�Nlight zoom in zoom out\n";
			else cout<<"�^��light �W�U���k mode\n";
			break;
		case 27:
			exit(0);
			break;
	}
	
	glutPostRedisplay();
}
void specialkey ( int key ,int x ,int y) {

	switch ( key ) {
		case GLUT_KEY_F1:
			mode = DLIGHTING;
			break;
		case GLUT_KEY_F2:
			mode = MIRROR;
			break;
		case GLUT_KEY_F3:
			mode = INDIRECT;
			break;
		case GLUT_KEY_F4:
			mode = DEFAULT;
			break;
		default:
			mode = DEFAULT;
			break;
	}
	
	glutPostRedisplay();
}

static int startx;
static int starty;
const int NOTANUMBER = -1;
static int prex = NOTANUMBER;
static int prey = NOTANUMBER;
static bool movez = false;//��camera z�b����
void mouse(int button, int state, int x, int y) {
	cout<<x<<" " <<y <<"\n";
	if ( button == GLUT_LEFT_BUTTON ) {//����
		if ( state == GLUT_DOWN ) {//���U
			startx = x;
			starty = y;
			if ( glutGetModifiers() == GLUT_ACTIVE_CTRL ) {
				movez = true;
				cout<<"camera ��z�b����\n";
			}
		}
		else {
			prex = NOTANUMBER;
			prey = NOTANUMBER;
		}
			
	}
}

void motion(int x, int y) {

	//�Y�Oprex�S����� �N�]�����U�h��(x ,y)
	if (prex == NOTANUMBER ) prex = startx;
	if (prey == NOTANUMBER) prey  = starty;

	if ( !movez) {
		
		double dispx = (x - prex)/7;
		double dispy = (prey - y)/7;
		printf("new (%d ,%d )\n" , x ,y);
		printf("dispx dispy = %lf %lf \n",dispx ,dispy);
		
		par->mCameraByDelta( dispx ,dispy , 0 );

	}
	else {
		double dispx = ( x - prex )/7;
		double dispz = ( y - prey )/7;
		par->mCameraByDelta( dispx , 0 ,dispz );
	}

	prex = x;
	prey = y;
	glutPostRedisplay();

}

void reshape(GLsizei w,GLsizei h) {
	if (par != NULL) {
		par->setGLWindowWidth(w);
		par->setGLWindowHeight(h);
	}
}
/*
   �]�wmodel view
*/
void setModelView() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if ( mode == DEFAULT ) {
		Camera camera = par->getCameraPosition();
		
		gluLookAt( camera.eyex    ,camera.eyey    ,camera.eyez ,
				   camera.centerx ,camera.centery ,camera.centerz ,
				   camera.upx     ,camera.upy     ,camera.upz );
		printf("%lf %lf %lf\n",camera.eyex ,camera.eyey ,camera.eyez );
		printf("%lf %lf %lf\n", camera.centerx ,camera.centery ,camera.centerz );
		printf("%lf %lf %lf\n", camera.upx     ,camera.upy     ,camera.upz );
		printf("============================\n");
	} 

	
	
	
}
/*�]�wperspectve*/
void setPerspectiveView() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(par->getFOV(), (GLfloat)par->getGLWindowWidth() /(GLfloat)par->getGLWindowHeight(), 
		           par->getNearPlane() ,par->getFarPlane());
}

