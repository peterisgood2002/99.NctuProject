/******************************************
FILE
    function.h
Purpose
    �Ҧ��D�ط|�Ψ쪺function �ΨӶi��
	User can set various light and material parameters 
	  to render diffuse or specular lighting effects. (10%)
    1.Render the Utah teapot using Gouraud shading. (15%)
	2.Render the Utah teapot using Gouraud shading with texture.(With mipmap) (15%)
    4.Render the Utah teapot with bump mapping.(50%)

NOTE
	

Author
    9757553 �C����
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
//���Shading�Ҧ�
extern bool openPhongShading;
extern bool openBumpmapping;
extern GLhandleARB phongShading;
extern GLhandleARB bumpMapping ;

/*�Nmodel�e�X��
  �ǤJ�G�O�_�[�Wtexture
*/
void drawPolygon( bool withtexture );

/*���ϥΪ̳]�w�Ѽƥ�*/
void lighting();
void setMaterialParameter(GLfloat* mat_d,GLfloat* mat_s ,GLfloat mat_sh ,GLfloat* mat_a = NULL);
void setLightingParameter(GLfloat* ,GLfloat* ,GLfloat* light_a = NULL );

/*�������Gouraud shading��lighting*/
void program1();
void drawPolygonWithoutTexture();

/*�������Gouraud shading with texture*/
void program2();
void drawPolygonWithTexture();


#endif