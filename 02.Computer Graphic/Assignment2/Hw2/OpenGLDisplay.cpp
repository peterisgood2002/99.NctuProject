/******************************************
FILE
    CallBackFunction.h
Purpose
    所有Call back function以及所有Call back function需要用到的函式

NOTE
	

Author
    9757553 顏志晟
*******************************************/
#include <iostream>
#include "OpenGLDisplay.h"


using std::cout;

OpenGLParameter* par;
Scene* scene;


Mode mode = DEFAULT;

/*初始化整個OpenGL程式
  傳入：OpenGL參數和場景
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
	glutKeyboardFunc(keyboard);//對keyboard有反應
	glutSpecialFunc(specialkey);
	glutMouseFunc(mouse);//對mouse 有反應
	glutMotionFunc(motion);//對mouse移動有反應
	glutDisplayFunc(display);//顯示用
	glutReshapeFunc(reshape);
}
#include "function.h"

/*display scene*/
void display() {
	
	//設定model view
	setModelView();
	
	//設定perceptive project 
	setPerspectiveView();

	//設定clip the view 
	glViewport(0, 0, par->getGLWindowWidth(), par->getGLWindowHeight() );

	
	//根據mode畫場景
	scene->display(mode);

	glutSwapBuffers();//double buffer使用

}
/*response the keyboard*/
void keyboard(unsigned char key, int x, int y) {
	char k = toupper(key);//將所有值改成小寫 
	static bool zoomlight = false;
	switch (k) {
		case 'W'://向上移動 物體向前移動
			if ( zoomlight ) {
				par->mLightByDelta( 0 ,0 , -1);
			}
			else par->mLightByDelta( 0 ,1 ,0 );
			break;
		case 'A'://向左移動
			par->mLightByDelta(-1 ,0 ,0 );
			break;
		case 'D'://向右移動
			par->mLightByDelta(1 ,0 ,0 );
			break;
		case 'S'://向下移動
			if ( zoomlight ) par->mLightByDelta(0 ,0 ,1 );
			else par->mLightByDelta(0 ,-1 ,0 );
			break;
		case 'Z':
			zoomlight = ( zoomlight )? false : true;
			if (zoomlight) cout<<"現在將light zoom in zoom out\n";
			else cout<<"回到light 上下左右 mode\n";
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
static bool movez = false;//對camera z軸移動
void mouse(int button, int state, int x, int y) {
	cout<<x<<" " <<y <<"\n";
	if ( button == GLUT_LEFT_BUTTON ) {//左鍵
		if ( state == GLUT_DOWN ) {//壓下
			startx = x;
			starty = y;
			if ( glutGetModifiers() == GLUT_ACTIVE_CTRL ) {
				movez = true;
				cout<<"camera 往z軸移動\n";
			}
		}
		else {
			prex = NOTANUMBER;
			prey = NOTANUMBER;
		}
			
	}
}

void motion(int x, int y) {

	//若是prex沒有初值 就設為壓下去的(x ,y)
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
   設定model view
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
/*設定perspectve*/
void setPerspectiveView() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(par->getFOV(), (GLfloat)par->getGLWindowWidth() /(GLfloat)par->getGLWindowHeight(), 
		           par->getNearPlane() ,par->getFarPlane());
}

