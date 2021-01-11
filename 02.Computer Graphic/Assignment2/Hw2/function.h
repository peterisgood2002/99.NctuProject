/******************************************
FILE
    program.h
Purpose

NOTE
	

Author
    9757553 �C����
*******************************************/

#include "mesh.h"
#include "OpenGLParameter.h"
#include "Ray.h"

extern OpenGLParameter* par;

/*��l��Canvas�n�e�F��*/
void initCanvas();
/*default������*/
void lighting();
/*inner product*/
double innerProduct(const double* v1 ,const double* v2);
/*�Ϯg
  �ǤJ�G�_�l��m �J�g�u �k�u
  �ǥX�G�Ϯg�u
*/
Ray reflection( const double * p ,const double* ptoe ,const double* normal );

