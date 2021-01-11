/******************************************
FILE
    Ray.h
Purpose
    

NOTE
	

Author
    9757553 顏志晟
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
	  根據3D空間座標 和某一個點產生一條Ray
	  傳入：起始座標和終止座標 以及此Ray是屬於射線 還是她只是一條線段 預設值為他是線段
	*/
	Ray( const double*  s ,const double* end ,bool isray = false ,double infinity = 1000000 );
	/*Constructor
	  根據pixel座標值 以及Camera位置產生Ray
	  傳入：pixel座標值 還有參考的參數值
	*/
	Ray( int pixelx ,int pixely ,const OpenGLParameter* const par);
	/*Copy Constructor
	  傳入一個Ray
	*/
	Ray( const Ray& r);
	/*得到最終點*/
	const double* getEndPoint();
	/*對向量做normalize*/
	double* normalizeB();

};
#endif