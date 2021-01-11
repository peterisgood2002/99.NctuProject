/******************************************
FILE
    function.h
Purpose
    所有題目會用到的function 用來進行
	User can set various light and material parameters 
	  to render diffuse or specular lighting effects. (10%)
    1.Render the Utah teapot using Gouraud shading. (15%)
	2.Render the Utah teapot using Gouraud shading with texture.(With mipmap) (15%)
    4.Render the Utah teapot with bump mapping.(50%)

NOTE
	

Author
    9757553 顏志晟
*******************************************/

#ifndef _FUNCTION_H_

#define _FUNCTION_H_

#include "glew.h"
#include "wglew.h"
#include "Camera.h"
#include "mesh.h"
#include "glut.h"
#include <iostream>

const int NTEX = 2;
//兩種Shading模式
extern bool openPhongShading;
extern bool openBumpmapping;
extern GLhandleARB phongShading;
extern GLhandleARB bumpMapping ;

/*將model畫出來
  傳入：是否加上texture
*/
void drawPolygon( bool withtexture );

/*讓使用者設定參數用*/
void lighting();
void setMaterialParameter(GLfloat* mat_d,GLfloat* mat_s ,GLfloat mat_sh ,GLfloat* mat_a = NULL);
void setLightingParameter(GLfloat* ,GLfloat* ,GLfloat* light_a = NULL );

/*對茶壺做Gouraud shading的lighting*/
void program1();
void drawPolygonWithoutTexture();

/*對茶壺做Gouraud shading with texture*/
void program2();
void drawPolygonWithTexture();


#endif