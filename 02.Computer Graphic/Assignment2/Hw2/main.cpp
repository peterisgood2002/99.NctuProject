/******************************************
FILE
    main.cpp
PURPOSE
    程式主要進入點

NOTE
	場景移動：
	    請透過滑鼠左鍵上下左右移動camera
		若要拉近拉遠 請按下Ctrl進行控制
	光源移動
	    請透過鍵盤
		     W
	        ASD
		
AUTHOR
    9757553 顏志晟
*******************************************/

#include <iostream>
#include "OpenGLParameter.h"
#include "OpenGLDisplay.h"
#include "Scene.h"


using std::cout;
int main(int argv ,char** argc) {
	
	/*設定OpenGL程式該有的參數*/
	OpenGLParameter par;
	par.setGLWindowWidth( 200 );
	par.setGLWindowHeight( 200 );
	par.setCameraPosition( 0 ,20 ,72 , 0, 20, 0 , 0 , 1 , 0 );
	par.setClipPlane( 54.43 ,0.1 ,1000 );
	par.setLightPostion(0 , 30 ,15 , 1);
	//par.setLightPostion(5 , 28 ,19 , 1);
	par.setLightSpecular(1 ,1 ,1 , 1);
	par.setLightDiffuse(1, 1, 1, 1.0);
	par.setLightAmbient(0, 0, 0, 1.0);

	/*讀入場景*/
	char* str[3];
	str[0] = "../model/Cornell_box.obj";
	str[1] = "../model/tall_box.obj"; 
	str[2] = "../model/ball.obj";
	Scene scene (str ,3);
	double t[3] = {10 ,23 ,5 };
	//scene.tMeshTo(2 ,t);

	/*initialize the opgl*/
	Initialize(argv ,argc ,par ,scene);
	
	////test function
	//Ray r;
	/*double e[3] = { 8 ,31 ,6 };
	double b[3] = { 7.6 ,31.8 ,5.4 };
	Ray r(e ,b ,3 );
	scene.test(r);*/

	/*set call back function*/
	setCallBackFunc();
	glutMainLoop();
	
}