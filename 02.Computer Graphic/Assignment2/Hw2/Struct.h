/******************************************
FILE
    Struct.h
PURPOSE
    �x�sStruct���c

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/


#ifndef _STRUCT_
#define _STRUCT_

struct Camera {
private:
	//Matrix which Translate the world coordinate to camera coordinate
	double vec[3];
	double uvec[3];
	double nvec[3];
	/*�D�X �qcamera�y�Шt��@�ɮy�Шt��matrix*/
	void calCulateMatrixC2W();
public :
	//eye camera��m
	double eyex;
	double eyey;
	double eyez;
	//center ���餤����m
	double centerx;
	double centery;
	double centerz;
	//up
	double upx;
	double upy;
	double upz;
	/*�Nworld coordinate�ন camera coordinate
	  �ǤJ�Gworld coordinate
	  �ǥX�Gcamera coordinate
	*/
	double* W2C( const double w[4] );
	/*�Nworld coordinate�ন camera coordinate
	  �ǥX�Gworld coordinate
	  �ǤJ�Gcamera coordinate
	*/
	double* C2W( const double c[4] );
	
	
};

struct Light {
public:
	float lightp[4];
	float lspecular[4];
	float ldiffuse[4];
	float lambient[4];

};

struct Color {
public:
	float r;
	float g;
	float b;
	float a;
};

#endif