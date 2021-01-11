/******************************************
FILE
    Ray.h
Purpose
    

NOTE
	

Author
    9757553 �C����
*******************************************/

#ifndef _RAY_
#define _RAY_

#include "OpenGLParameter.h"
#include <iostream>

class Ray {
private:
	double e[3];
	double b[3];
	double t;
	friend class Model;
	friend class Scene;


public:
	/*Constructor
	  �ھ�3D�Ŷ��y�� �M�Y�@���I���ͤ@��Ray
	  �ǤJ�G�_�l�y�ЩM�פ�y�� �H�Φ�Ray�O�ݩ�g�u �٬O�o�u�O�@���u�q �w�]�Ȭ��L�O�u�q
	*/
	Ray( const double*  s ,const double* end ,bool isray = false ,double infinity = 1000000 );
	/*Constructor
	  �ھ�pixel�y�Э� �H��Camera��m����Ray
	  �ǤJ�Gpixel�y�Э� �٦��ѦҪ��Ѽƭ�
	*/
	Ray( int pixelx ,int pixely ,const OpenGLParameter* const par);
	/*Copy Constructor
	  �ǤJ�@��Ray
	*/
	Ray( const Ray& r);
	/*�o��̲��I*/
	const double* getEndPoint();
	/*��V�q��normalize*/
	double* normalizeB();

};
#endif