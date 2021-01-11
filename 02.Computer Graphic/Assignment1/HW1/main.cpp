/******************************************
FILE
    main.cpp
PURPOSE
    程式主要進入點

NOTE
	此程式可以經過鍵盤滑鼠操作 
	1.鍵盤設定主要控制camera
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
	2.滑鼠主要控制物體及Camera旋轉 
	  按下Ctrl鍵可以轉動model
    3.此程式主要是作業 因此可以透過鍵盤中的
	Function key 1~4進行對作業mode的切換
	F4：開啟關閉Phong Shading能力

AUTHOR
    9757553 顏志晟
*******************************************/

#include "function.h"
#include <iostream>
#include <string>
#include "callBackFunction.h"
#include "glut.h"
#include "glaux.h"
#include "ShaderLoader.h"
using std::cout;
using std::string;

int windowheight = 500;//視窗高度
int windowwidth = 500;//視窗寬度
int windowx = 100;//視窗位置
int windowy = 100;

extern const int NTEX;
GLuint texture[NTEX];//texture object 作業二以後要用到Texture Mapping用
Camera* eye;
mesh* model;
AUX_RGBImageRec* bumpmap;
bool openPhongShading = false;
bool openBumpmapping = false;
GLhandleARB phongShading = 0;
GLhandleARB bumpMapping = 0;

//讀Texture資料
void setTexture();
void LoadTexture(int textureIndex , GLuint* texture ,string filename ,bool recordimage);

int 
main(int argc ,char** argv) {

	/*設定程式物件相關初值*/
	//eye = new Camera( 0  ,0 ,0
	//				 ,0 ,-1 ,20//往(0,0,-1)方向看過去
	//				 ,0, 1,0);
	eye = new Camera();
	model = new mesh("../model/teapot.obj");

	/*initialize*/
	glutInit( &argc ,argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH );
	glutInitWindowSize( windowwidth ,windowheight);
	glutInitWindowPosition(windowx ,windowy);
	glutCreateWindow("HW1");

	/*設定Shader*/
	GLenum glew_error;//必須要 如果沒有會當掉
	if((glew_error = glewInit()) != GLEW_OK) return -1;
	//A program object represents a useable part of render pipeline.
	phongShading = glCreateProgramObjectARB();
	bumpMapping = glCreateProgramObjectARB();
	//針對Shading source code進行compile 和link動作
	if ( phongShading != 0 ) {
		ShaderLoad( phongShading ,"../Shader/PhongShading.vs" ,GL_VERTEX_SHADER_ARB );
		ShaderLoad( phongShading ,"../Shader/PhongShading.fs" ,GL_FRAGMENT_SHADER_ARB );
	}
	if ( bumpMapping != 0 ) {
		ShaderLoad( bumpMapping ,"../Shader/UseTexture.vs" ,GL_VERTEX_SHADER_ARB );
		ShaderLoad( bumpMapping ,"../Shader/UseTexture.fs" ,GL_FRAGMENT_SHADER_ARB );
	}
	
	/*讀入Texture*/
	setTexture();
	
	/*set call back function*/
	setCallBackFunc();
	
	/*啟動Windows*/
	glutMainLoop();
}


void 
setTexture() {
	glGenTextures(NTEX ,texture );//告訴OpenGL有一個Texture的陣列
#define MIPMAP//根據題目要求使用MIPMAP
	string filename = "../model/fiber.bmp";
	string tmp;
	cout<<"請輸入Texture(e表示使用預設值fiber.bmp)：";
	cin >> tmp;
	if ( tmp != "e" ) filename = tmp;
	LoadTexture(0 ,texture , filename ,false);

	//輸入Bump mapping需要用到的bump map
	filename = "../model/fiber_bump.bmp";
	cout<<"請輸入Bump mapp所需要用到的(e表示使用預設值fiber_bump.bmp)：";
	cin >> tmp;
	if ( tmp != "e" ) filename = tmp;
	LoadTexture(1 ,texture , filename ,true);
	
}
/*透過檔名讀進Texture
  傳入：Texture放置位置 Texture array 檔名 以及是否要記錄image資料以方便以後進行bump mapping
*/
void 
LoadTexture(int textureIndex, GLuint* texture ,string filename ,bool recordimage) {
	AUX_RGBImageRec* img  = auxDIBImageLoadA(filename.c_str() );	//to save image file
  
	glBindTexture(GL_TEXTURE_2D, texture[textureIndex]);//告訴texture格式
#ifdef MIPMAP
	//產生2D minmap 將image存進 texture array中
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, img->sizeX, img->sizeY, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	/*設定Texture*/
	//設定pixel如何對應到Texture上 第三個參數為texture function
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//設定Texture參數值 
    //物體比texture大時 texture的mapping方式第三個參數是使用四個相近點做平均
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//物體比Texture小時 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#else
	//使用一個2D的Image Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif

	if ( recordimage ) bumpmap = img;
	else {//釋放記憶體
		if ( img->data != NULL ) delete img->data;
		delete img;
	}
}