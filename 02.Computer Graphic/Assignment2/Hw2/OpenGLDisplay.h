/******************************************
FILE
    CallBackFunction.h
Purpose
    �Ҧ�Call back function�H�ΩҦ�Call back function�ݭn�Ψ쪺�禡

NOTE
	

Author
    9757553 �C����
*******************************************/

#ifndef _CALL_BACK_FUNC_

#define _CALL_BACK_FUNC_

#include "glut.h"
#include "OpenGLParameter.h"
#include "Scene.h"

#ifndef _MODE_
#define _MODE_
enum Mode { DEFAULT = 0 ,DLIGHTING ,MIRROR ,INDIRECT };//���{���������Ҧ�
#endif


/*�NmodelŪ�J*/
/*��l�ƾ��OpenGL�{��
  �ǤJ�GOpenGL�ѼƩM����
 */
void Initialize(int argv ,char** argc ,OpenGLParameter& par ,Scene& scene);
void setCallBackFunc();
void display();
void setPerspectiveView();
void setModelView();
void reshape(GLsizei w,GLsizei h); 
/*����keyboard��T �ð��X�۹������欰*/
void keyboard(unsigned char key, int x, int y);
void specialkey ( int key ,int x ,int y);
/*����mouse��T*/
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
/*Default����X ���F�T�wmodel�O�_���TŪ�J*/
void defaultDisplay();



#endif