/******************************************
FILE
    main.cpp
PURPOSE
    �{���D�n�i�J�I

NOTE
	���{���i�H�g�L��L�ƹ��ާ@ 
	1.��L�]�w�D�n����camera
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
	2.�ƹ��D�n������Camera���� 
	  ���UCtrl��i�H���model
    3.���{���D�n�O�@�~ �]���i�H�z�L��L����
	Function key 1~4�i���@�~mode������
	F4�G�}������Phong Shading��O

AUTHOR
    9757553 �C����
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

int windowheight = 500;//��������
int windowwidth = 500;//�����e��
int windowx = 100;//������m
int windowy = 100;

extern const int NTEX;
GLuint texture[NTEX];//texture object �@�~�G�H��n�Ψ�Texture Mapping��
Camera* eye;
mesh* model;
AUX_RGBImageRec* bumpmap;
bool openPhongShading = false;
bool openBumpmapping = false;
GLhandleARB phongShading = 0;
GLhandleARB bumpMapping = 0;

//ŪTexture���
void setTexture();
void LoadTexture(int textureIndex , GLuint* texture ,string filename ,bool recordimage);

int 
main(int argc ,char** argv) {

	/*�]�w�{������������*/
	//eye = new Camera( 0  ,0 ,0
	//				 ,0 ,-1 ,20//��(0,0,-1)��V�ݹL�h
	//				 ,0, 1,0);
	eye = new Camera();
	model = new mesh("../model/teapot.obj");

	/*initialize*/
	glutInit( &argc ,argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH );
	glutInitWindowSize( windowwidth ,windowheight);
	glutInitWindowPosition(windowx ,windowy);
	glutCreateWindow("HW1");

	/*�]�wShader*/
	GLenum glew_error;//�����n �p�G�S���|��
	if((glew_error = glewInit()) != GLEW_OK) return -1;
	//A program object represents a useable part of render pipeline.
	phongShading = glCreateProgramObjectARB();
	bumpMapping = glCreateProgramObjectARB();
	//�w��Shading source code�i��compile �Mlink�ʧ@
	if ( phongShading != 0 ) {
		ShaderLoad( phongShading ,"../Shader/PhongShading.vs" ,GL_VERTEX_SHADER_ARB );
		ShaderLoad( phongShading ,"../Shader/PhongShading.fs" ,GL_FRAGMENT_SHADER_ARB );
	}
	if ( bumpMapping != 0 ) {
		ShaderLoad( bumpMapping ,"../Shader/UseTexture.vs" ,GL_VERTEX_SHADER_ARB );
		ShaderLoad( bumpMapping ,"../Shader/UseTexture.fs" ,GL_FRAGMENT_SHADER_ARB );
	}
	
	/*Ū�JTexture*/
	setTexture();
	
	/*set call back function*/
	setCallBackFunc();
	
	/*�Ұ�Windows*/
	glutMainLoop();
}


void 
setTexture() {
	glGenTextures(NTEX ,texture );//�i�DOpenGL���@��Texture���}�C
#define MIPMAP//�ھ��D�حn�D�ϥ�MIPMAP
	string filename = "../model/fiber.bmp";
	string tmp;
	cout<<"�п�JTexture(e��ܨϥιw�]��fiber.bmp)�G";
	cin >> tmp;
	if ( tmp != "e" ) filename = tmp;
	LoadTexture(0 ,texture , filename ,false);

	//��JBump mapping�ݭn�Ψ쪺bump map
	filename = "../model/fiber_bump.bmp";
	cout<<"�п�JBump mapp�һݭn�Ψ쪺(e��ܨϥιw�]��fiber_bump.bmp)�G";
	cin >> tmp;
	if ( tmp != "e" ) filename = tmp;
	LoadTexture(1 ,texture , filename ,true);
	
}
/*�z�L�ɦWŪ�iTexture
  �ǤJ�GTexture��m��m Texture array �ɦW �H�άO�_�n�O��image��ƥH��K�H��i��bump mapping
*/
void 
LoadTexture(int textureIndex, GLuint* texture ,string filename ,bool recordimage) {
	AUX_RGBImageRec* img  = auxDIBImageLoadA(filename.c_str() );	//to save image file
  
	glBindTexture(GL_TEXTURE_2D, texture[textureIndex]);//�i�Dtexture�榡
#ifdef MIPMAP
	//����2D minmap �Nimage�s�i texture array��
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, img->sizeX, img->sizeY, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	/*�]�wTexture*/
	//�]�wpixel�p�������Texture�W �ĤT�ӰѼƬ�texture function
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//�]�wTexture�Ѽƭ� 
    //�����texture�j�� texture��mapping�覡�ĤT�ӰѼƬO�ϥΥ|�Ӭ۪��I������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//�����Texture�p�� 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#else
	//�ϥΤ@��2D��Image Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif

	if ( recordimage ) bumpmap = img;
	else {//����O����
		if ( img->data != NULL ) delete img->data;
		delete img;
	}
}