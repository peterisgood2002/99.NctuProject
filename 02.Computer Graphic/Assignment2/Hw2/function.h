/******************************************
FILE
    program.h
Purpose

NOTE
	

Author
    9757553 顏志晟
*******************************************/

#include "mesh.h"
#include "OpenGLParameter.h"
#include "Ray.h"

extern OpenGLParameter* par;

/*初始化Canvas好畫東西*/
void initCanvas();
/*default打光用*/
void lighting();
/*inner product*/
double innerProduct(const double* v1 ,const double* v2);
/*反射
  傳入：起始位置 入射線 法線
  傳出：反射線
*/
Ray reflection( const double * p ,const double* ptoe ,const double* normal );

