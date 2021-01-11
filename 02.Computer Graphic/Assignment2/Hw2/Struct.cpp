/******************************************
FILE
    Struct.h
PURPOSE 
    儲存Struct結構

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "Struct.h"
#include "math.h"

void 
Camera::calCulateMatrixC2W() {
	/*vec -->z uvec-->y nvec -->x*/
	vec[0] = centerx - eyex; 
	vec[1] = centery - eyey;
	vec[2] = centerz - eyez;
	uvec[0] = upx - eyex;
	uvec[1] = upy - eyey;
	uvec[2] = upz -eyez;

	/*求出orthonoraml camera坐標軸*/
	for ( int i = 0; i < 3; i++ ) 
		nvec[i] = uvec[ (i+1) % 3 ] * vec[ ( i+2 ) % 3 ] - vec[ (i+1) % 3 ] * uvec[ (i+2) % 3 ];
	for ( int i = 0; i < 3; i++ )
		uvec[i] = nvec[ (i+1) % 3 ] * vec[ ( i+2 ) % 3 ] - vec[ (i+1) % 3 ] * nvec[ (i+2) % 3 ];
	
	double lvec = sqrt( vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] );
	double luvec = sqrt( uvec[0] * uvec[0] + uvec[1] * uvec[1] + uvec[2] * uvec[2] );
	double lnvec = sqrt( nvec[0] * nvec[0] + nvec[1] * nvec[1] + nvec[2] * nvec[2] );
	for ( int i = 0; i < 3; i++ ) {
		vec[i] /= -lvec;
		uvec[i] /= luvec;
		nvec[i] /= lnvec;
	}
}
/*將world coordinate轉成 camera coordinate
      [ nvec nvec nvec 0 ] [1  0  0  -eyex ]  [x]   [u]
	  [ uvec uvec uvec 0 ] [0  1  0  -eyey ]  [y] = [v]
	  [ vec  vec  vec  0 ] [0  0  1  -eyez ]  [z]   [w]
	  [  0    0    0   1 ] [0  0  0     1  ]  [1]   [1]
	  傳入：world coordinate
	  傳出：camera coordinate
*/
double* 
Camera::W2C( const double w[4] ) {
	calCulateMatrixC2W();
	
	//算出camera座標系
	double* c = new double[4];
	c[0] = w[0] - eyex;
	c[1] = w[1] - eyey;
	c[2] = w[2] - eyez;
	c[3] = w[3];

	double tmp[4] = { 0 };
	tmp[0] = nvec[0] * c[0] + nvec[1] * c[1] + nvec[2] * c[2];
	tmp[1] = uvec[0] * c[0] + uvec[1] * c[1] + uvec[2] * c[2];
	tmp[2] = vec[0] * c[0] + vec[1] * c[1] + vec[2] * c[2];

	c[0] = tmp[0];
	c[1] = tmp[1];
	c[2] = tmp[2];
	
	return c;

}
/*將camera coordinate轉成 world coordinate
	  [1 0 0 eyex][ nvec uvec vec 0 ][u]   [x]
	  [0 1 0 eyey][ nvec uvec vec 0 ][v] = [y]
	  [0 0 1 eyez][ nvec uvec vec 0 ][w]   [z]
	  [0 0 0   1 ][  0     0   0  1 ][1]   [1]
  傳出：world coordinate
  傳入：camera coordinate
*/
double* 
Camera::C2W( const double c[4] ) {
	calCulateMatrixC2W();

	//算出world座標系
	double* w = new double[4];
	for ( int i = 0; i < 3; i++ ) 
		w[i] = nvec[i] * c[0] + uvec[i] * c[1] + vec[i] * c[2];

	w[0] += eyex;
	w[1] += eyey;
	w[2] += eyez;
	w[3] = c[3];

	return w;
}
