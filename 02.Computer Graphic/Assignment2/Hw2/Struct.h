/******************************************
FILE
    Struct.h
PURPOSE
    儲存Struct結構

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/


#ifndef _STRUCT_
#define _STRUCT_

struct Camera {
private:
	//Matrix which Translate the world coordinate to camera coordinate
	double vec[3];
	double uvec[3];
	double nvec[3];
	/*求出 從camera座標系到世界座標系的matrix*/
	void calCulateMatrixC2W();
public :
	//eye camera位置
	double eyex;
	double eyey;
	double eyez;
	//center 物體中間位置
	double centerx;
	double centery;
	double centerz;
	//up
	double upx;
	double upy;
	double upz;
	/*將world coordinate轉成 camera coordinate
	  傳入：world coordinate
	  傳出：camera coordinate
	*/
	double* W2C( const double w[4] );
	/*將world coordinate轉成 camera coordinate
	  傳出：world coordinate
	  傳入：camera coordinate
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