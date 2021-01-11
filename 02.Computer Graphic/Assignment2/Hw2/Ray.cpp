/******************************************
FILE
    Ray.h
Purpose
    

NOTE
	

Author
    9757553 �C����
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

 ���O�ҥ��W���y�Шt��
  y
 /|\
  |
  |
  |
  --------> x   �ҥHy�n�i���A�˰ʧ@
  ��XRay    [-0.5 nx-0.5] * [-0.5 ny-0.5] -->[l r] [b t]
  u = l + ( r - l ) * ( i+0.5 ) /nx
  v = b + ( t - b ) * ( j+0.5 ) /ny
  �ǤJ�Gpixel�y�Э� field of view near plane�b����
*/

const double PI = 3.141615926;
Ray::Ray(int pixelx, int pixely ,const OpenGLParameter* const par) {
	
	double radian = ( par->getFOV() / 2 ) * PI / 180;//radian
	int width = par->getGLWindowWidth();
	int height = par->getGLWindowHeight();
	double n = par->getNearPlane();
	double planeh =  n * tan( radian );//�������
	double planew = width / height * planeh;

	//�]�wdata member
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
  �ھ�3D�Ŷ��y�� �M�Y�@���I���ͤ@��Ray
  �ǤJ�G�_�l�y�ЩM�פ�y��
*/
Ray::Ray(const double*  s ,const double* end ,bool isray ,double infinity) {
	for ( int i = 0; i < 3; i++ ) {
		e[i] = s[i];
		b[i] = end[i] - s[i];
	}
	if ( !isray)
		t = 1;//��ܳo�@���u�u����I
	else t =infinity;

}
/*Copy Constructor
  �ǤJ�@��Ray
*/
Ray::Ray( const Ray& r) {
	for ( int i = 0; i < 3; i++ ) {
		e[i] = r.e[i];
		b[i] = r.b[i];
	}

	t = r.t;
}
/*��vector B�i��normalize���ʧ@ �H��K�H�᤺�n�Q���|�X�{�Ӧh�·�
*/
double* 
Ray::normalizeB() {
	double* t = new double[3];
	double l = sqrt( b[0] * b[0] + b[1] * b[1] + b[2] * b[2] );
	for ( int i = 0; i < 3; i++ )
		t[i] = b[i] / l;

	return t;
}

/*�o��̲��I*/
const double* 
Ray::getEndPoint() {
	double* p = new double[3];
	for ( int i = 0; i < 3; i++ ) 
		p[i] = e[i] + t * b[i];

	return p;
}


