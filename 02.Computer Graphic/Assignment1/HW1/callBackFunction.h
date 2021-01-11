/******************************************
FILE
    callBackFunction.h
Purpose
    所有Call back function以及所有Call back function需要用到的函式

NOTE
	

Author
    9757553 顏志晟
*******************************************/

#ifndef _CALL_BACK_FUNC_

#define _CALL_BACK_FUNC_

#include "glut.h"

void setCallBackFunc();
void display();
void setPerspectiveView();
void setModelView();
void reshape(GLsizei w,GLsizei h);
/*接收keyboard資訊 並做出相對應的行為*/
void keyboard(unsigned char key, int x, int y);
void specialkey ( int key ,int x ,int y);
/*接收mouse資訊*/
void mouse(int button, int state, int x, int y);
void motion(int x, int y);



#endif