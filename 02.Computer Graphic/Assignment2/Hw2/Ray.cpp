/******************************************
FILE
    Ray.h
Purpose
    

NOTE
	

Author
    9757553 顏志晟
*******************************************/

#include "Ray.h"
#include "math.h"
/*Constructor
  ---------------------> pixelx
  |
  |
  |
 \|/
 pixely

 但是課本上的座標系為
  y
 /|\
  |
  |
  |
  --------> x   所以y要進行顛倒動作
  算出Ray    [-0.5 nx-0.5] * [-0.5 ny-0.5] -->[l r] [b t]
  u = l + ( r - l ) * ( i+0.5 ) /nx
  v = b + ( t - b ) * ( j+0.5 ) /ny
  傳入：pixel座標值 field of view near plane在哪裡
*/

const double PI = 3.141615926;
Ray::Ray(int pixelx, int pixely ,const OpenGLParameter* const par) {
	
	double radian = ( par->getFOV() / 2 ) * PI / 180;//radian
	int width = par->getGLWindowWidth();
	int height = par->getGLWindowHeight();
	double n = par->getNearPlane();
	double planeh =  n * tan( radian );//取絕對值
	double planew = width / height * planeh;

	//設定data member
	e[0] = par->getCameraPosition().eyex;
	e[1] = par->getCameraPosition().eyey; 
	e[2] = par->getCameraPosition().eyez;
	pixely = height - pixely;
	b[0] = -planew + 2 * planew * ( pixelx + 0.5 ) / width;
	b[1] = -planeh + 2 * planeh * ( pixely + 0.5 ) / height;
	b[2] = -n;
	Camera camera = par->getCameraPosition();
	double * c = camera.C2W(b);
	b[0] = c[0] - e[0];
	b[1] = c[1] - e[1];
	b[2] = c[2] - e[2];
	t = par->getFarPlane();
}
/*Constructor
  根據3D空間座標 和某一個點產生一條Ray
  傳入：起始座標和終止座標
*/
Ray::Ray(const double*  s ,const double* end ,bool isray ,double infinity) {
	for ( int i = 0; i < 3; i++ ) {
		e[i] = s[i];
		b[i] = end[i] - s[i];
	}
	if ( !isray)
		t = 1;//表示這一條線只到終點
	else t =infinity;

}
/*Copy Constructor
  傳入一個Ray
*/
Ray::Ray( const Ray& r) {
	for ( int i = 0; i < 3; i++ ) {
		e[i] = r.e[i];
		b[i] = r.b[i];
	}

	t = r.t;
}
/*對vector B進行normalize的動作 以方便以後內積十部會出現太多麻煩
*/
double* 
Ray::normalizeB() {
	double* t = new double[3];
	double l = sqrt( b[0] * b[0] + b[1] * b[1] + b[2] * b[2] );
	for ( int i = 0; i < 3; i++ )
		t[i] = b[i] / l;

	return t;
}

/*得到最終點*/
const double* 
Ray::getEndPoint() {
	double* p = new double[3];
	for ( int i = 0; i < 3; i++ ) 
		p[i] = e[i] + t * b[i];

	return p;
}


