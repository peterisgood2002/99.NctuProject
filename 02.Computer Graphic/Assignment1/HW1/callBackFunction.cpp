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
	glutKeyboardFunc(keyboard);//對keyboard有反應
	glutSpecialFunc(specialkey);
	glutMouseFunc(mouse);//對mouse 有反應
	glutMotionFunc(motion);//對mouse移動有反應
	glutDisplayFunc(display);//顯示用
	glutReshapeFunc(reshape);
}

enum Mode { DEFAULT = 0 ,P1 ,P2  };//此程式的相關模式
Mode mode =  DEFAULT;

void display() {

	//設定model view
	setModelView();
	
	//設定perceptive project 
	setPerspectiveView();

	//設定clip the view 
	glViewport(0, 0, windowwidth, windowheight);

	//所有效果還原
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glUseProgramObjectARB(0);//設回預設值
	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);	

	
	//根據模式設定polygon
	switch ( mode) {
		case DEFAULT:
			drawPolygon( false);
			break;
		case P1:
			if (openPhongShading) {
				glUseProgramObjectARB( phongShading );
			}
			else {
				glUseProgramObjectARB(0);//設回預設值
 			}
			
			program1();
			break;

		case P2:
			if ( openBumpmapping) {
				glUseProgramObjectARB( bumpMapping );
			}
			else {
				glUseProgramObjectARB(0);//設回預設值
 			}
			program2();
			break;
	}
		
	

	glutSwapBuffers();//double buffer使用

}

static double dispx = 0;
static double dispy = 0;
static double dispz = -23;
static int DeltaR = 30;
static double theta = 0;//為了以後滑鼠使用
static double htheta = 0;//camera垂直旋轉
static double vtheta = 0;//camera水平旋轉
static double modeltheta = 0;
static bool zoommode = false;
static bool camerar = false;//Camera是否旋轉
/**
  鍵盤設定主要控制camera
      Q W E R
	  A S D

	  Z  
  W向上移動或Zoom in Camera向上轉動
  A向右移動 Camera向右轉動
  D向左移動 Camera向左轉動
  S向下移動或Zoom out Camera向下轉動
  Q model向左rotate
  E model向右rotate
  R將鍵盤切換為使用Camera旋轉功能
  
  若想要Zoom in Zoom out請按下 Z
*/
void keyboard(unsigned char key, int x, int y) {
	char k = toupper(key);//將所有值改成小寫 

	switch (k) {
		case 'W'://向上移動 物體向前移動
			if ( zoommode ) dispz--;
			else if ( camerar ) {
				htheta += DeltaR;
				
			}
			else dispy++;
			break;
		case 'A'://向左移動
			if ( camerar ) 
				vtheta -= DeltaR;
			else dispx--;
			break;
		case 'D'://向右移動
			if ( camerar ) 
				vtheta += DeltaR;
			else dispx++;
			break;
		case 'S'://向下移動
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
			if (zoommode) cout<<"現在可以zoom in zoom out\n";
			else cout<<"回到上下左右 mode\n";
			break;
		case 'R':
			camerar = (camerar)? false : true;
			if ( camerar) cout<<"可旋轉camera\n";
			else cout<<"回到上下左右mode\n";
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
				glUseProgramObjectARB(0);//設回預設值
			cout<<"目前處於Gouraud shading program\n";
			openPhongShading = false;
			mode = P1;
			break;

		case GLUT_KEY_F2:
			cout<<"目前處於 Gouraud shading with texture\n";
			if ( openPhongShading )
				glUseProgramObjectARB(0);//設回預設值
			openBumpmapping = false;
			mode = P2;
			break;

		case GLUT_KEY_F3:
			cout<<"目前處於 Bump mapping\n";
			openBumpmapping = true;
			mode = P2;
			break;

		case GLUT_KEY_F4:
			cout<<"目前處於Phong Shading\n";
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
 滑鼠的相關事件 
 1.壓下左鍵拖曳可上下左右移動
 2.按下右鍵拖曳可旋轉Camera
   若壓下Ctrl鍵可旋轉model
 *
*/
static int startx = 0;
static int starty = 0;
const int NOTANUMBER = -1;
const double PI = 3.1415926;
static int prex = NOTANUMBER;
static int prey = NOTANUMBER;
static bool setmodel = false;//model是否旋轉
void mouse(int button, int state, int x, int y) {
	if ( button == GLUT_MIDDLE_BUTTON ) {//中間的button用來Zoom in zoom out
		
	}
	else if ( button == GLUT_LEFT_BUTTON ) {//左鍵
		if ( state == GLUT_DOWN ) {//壓下
			startx = x;
			starty = y;
			printf("(%d ,%d )" , x ,y);
		}
		else {
			prex = NOTANUMBER;
			prey = NOTANUMBER;
		}
			
	}
	else {//右鍵
		if ( state == GLUT_DOWN ) {
			startx = x;
			starty = y;
			if ( glutGetModifiers() == GLUT_ACTIVE_CTRL ) {
				setmodel = true;
				cout<<"model旋轉開始\n";
			}
			else {
				camerar = true;
				cout<<"Camera旋轉開始\n";
			}
		}
		else {
			cout<<"右鍵解除\n";
			prex = NOTANUMBER;
			prey = NOTANUMBER;
			camerar = false;
			setmodel = false;
		}
	}

	//glutPostRedisplay();

}

void motion(int x, int y) {

	//旋轉用資料
	int vpx = prex - startx;//先前的向量
	int vpy = prey - starty;
	int vnx = x - startx;//現在的向量
	int vny = y - starty;

	//若是prex沒有初值 就設為壓下去的(x ,y)
	if (prex == NOTANUMBER ) prex = startx;
	if (prey == NOTANUMBER) prey  = starty;
	//printf("new (%d ,%d )\n" , x ,y);
	if ( !camerar && !setmodel) {//不要camera旋轉時
		
		dispx = dispx + (x - prex)/3;
		dispy = dispy + (prey - y)/3;
		printf("new (%d ,%d )\n" , x ,y);
		printf("dispx dispy = %lf %lf \n",dispx ,dispy);

	}
	else if ( camerar ){ //camera旋轉 
		/*在平面上以等腰三角形的方式 我們可以求得htheta 以及vtheta*/
		//取得球體半徑
		double r = eye->distanceToObject();
		//求htheta以及vtheta量
		vpx = x - prex;
		vpy = y - prey;
		double cost = ( r * r - vpx * vpx) / (2 * r * r);
		if ( -1 <= cost && cost <= 1) {
			double theta= 180 * acos ( cost ) /PI;
			/*cout << r <<"\n";
			printf("( x , y) = %d ,%d ( prex ,prey) = %d ,%d (vpx ,vpy) = %d %d " ,x ,y ,prex ,prey ,vpx ,vpy);
			cout<<"水平轉動:"<<theta<<" "<< acos ( cost ) << " " << cost << "\n";*/
			htheta = htheta + ( theta )/39;
		}
		cost = ( r * r - vpy * vpy) / (2 * r * r);
		if ( -1 <= cost && cost <= 1) {
			theta= 180 * acos ( cost ) /PI;
			//cout<<"垂直轉動:"<<theta<<" "<< acos ( cost ) <<" "<< cost <<"\n";
			vtheta = vtheta + ( theta )/39;
			/*printf("htheta ,vtheta = %lf %lf \n" ,htheta , vtheta ); 
			cout<<" ===============\n";*/
		}
	}
	else if ( setmodel ) {//model旋轉
		static int m = 0;
		if ( (m++) % 25 == 0 ) {
			//外積>=0逆時鐘方向旋轉 外積<0順時鐘方向旋轉
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
   設定model view
   OpenGL會一直乘到後面去
   teapot在設定Lookat的時候總是怪怪的 所以我們不使用Lookat以方便呈現
   他的座標轉換一直讓我們很困擾
   z
   |
   |
   |
   |
   ------------------->x
  

   主要是 model轉 移動 camera轉(camera以水平旋轉再以垂直旋轉
*/
void setModelView() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 

	printf("%lf %lf %lf\n" ,dispx ,dispy ,dispz );
	glTranslatef(dispx , dispy ,dispz);//camera平移
	
	gluLookAt( eye->getEyeX()     ,eye->getEyeY()    ,eye->getEyeZ() //eye
			  ,eye->getCenterX()  ,eye->getCenterY() ,eye->getCenterZ()//center of object
			  ,eye->getUpX()      ,eye->getUpY()     ,eye->getUpZ()    ); //up  

	glRotatef(modeltheta ,0.0 ,0.0 ,1.0);
	
	glRotatef( htheta, 1.0 ,0.0, 0.0 );//先對x軸旋轉
	glRotatef( vtheta, 0.0, 1.0, 0.0 );//在對z軸旋轉 M = T * Rm * L * Rx * Rz
}
/*設定perspectve*/
void setPerspectiveView() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)windowwidth/(GLfloat)windowheight, 0.1 ,30);
}
