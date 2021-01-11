/********************************************
Purpose
	�������O���w�q

Author
    9757553 �C����
*********************************************/
#include "Camera.h"
#include "math.h"

/*default constructor
  ��]�w�O�H�@�ɮy�Шt�i��]�w
  up = (0 ,0 ,1) center = (0 ,0 ,0) eye = (1 ,0 ,0)
*/
Camera::Camera() :PI(3.1415926){
	setCamera(0.0 ,0.0 ,0.0);
	setCenter(0.0f, 0.0f, -1.0f);
	setUp( 0.0 ,1.0 ,0.0);

}

Camera::Camera(double ex ,double ey ,double ez ,double cx ,double cy ,double cz ,double ux ,double uy ,double uz) :PI(3.1415926){
   setCamera(ex ,ey ,ez);
   setCenter(cx ,cy ,cz);
   setUp(ux ,uy ,uz);

}
//�]�w���������Ѽ�
/**
 *�]�w�����ݥX�h���V�q
 */
void 
Camera::setCamera(double ex ,double ey ,double ez) {
	eyex = ex;
	eyey = ey;
	eyez = ez;
}
/**
 *�]�w�������ߦ�m�b����
*/
void 
Camera::setCenter(double cx ,double cy ,double cz) {
	centerx = cx;
	centery = cy;
	centerz = cz;
}
/**
 *�����¤W����m����
*/
void 
Camera::setUp(double ux ,double uy ,double uz) {
	upx = ux;
	upy = uy;
	upz = uz;
}
//�o�체�������Ѽ�
double 
Camera::getEyeX() const{
	return eyex;
}
double 
Camera::getEyeY() const{
	return eyey;
}
double 
Camera::getEyeZ() const{
	return eyez;
}
double 
Camera::getCenterX()const {
	return centerx;
}
double 
Camera::getCenterY() const{
	return centery;
}
double 
Camera::getCenterZ() const{
	return centerz;
}
double 
Camera::getUpX() const{
	return upx;
}
double 
Camera::getUpY() const {
	return upy;
}
double 
Camera::getUpZ() const {
	return upz;
}
/**�]�w����zoom in �ﲴ������V��zoom in
  *�ǤJ�G�nzoom in�h�ֶZ��
*/
void 
Camera::zoomIn(double dist) {
	
	eyez -=dist;
}
/**�]�w����zoom out
  *�ǤJ�G�nzoom out �h�ֶZ��
*/
void 
Camera::zoomOut(double dist) {
	eyez += dist;
}
/**�V��
  *�ǤJ�G�n�V���h�ֶZ��
  */
void 
Camera::moveLeft(double dist) {
	centerx -= dist;
}
/**�V�k
  *�ǤJ�G�V�k�h�ֶZ��
  */
void 
Camera::moveRight(double dist) {
	centerx +=dist;
}
/**Camera�H�۪��餤�߱���
	  *�ǤJ�G���������ܤƶq ���������ܤƶq
	  */
void 
Camera::rotate(double rx ,double ry) {
	double PI = 3.1415926;
	//���o�����쪫�餤�����V�q
	double lx = eyex - centerx;
	double ly = eyey - centery;
	double lz = eyez - centerz;

	/*��Xup�M�����Ҭݨ��V�W�������H��K�ڭ̥H���Up��*/
	double lupx = upx - eyex;
	double lupy = upy - eyey;
	double lupz = upz - eyez;
	double nx = lupy * lz - lupz * ly;
	double ny = lupz * lx - lz * lupx;
	double nz = lupx * ly - lx * lupy;
	/*�䧹����*/
	/*�p��s��������m*/
	double r = sqrt (  lx*lx + ly*ly + lz*lz );
	
	double thetax = atan ( ly / lx );
	double thetay = asin ( lz / r );

	thetax += rx * PI / 180;
	thetay += ry * PI / 180;

	lz = r * sin( thetay );
	lx = r * cos( thetay ) * cos( thetax );
	ly = r * cos( thetay ) * sin( thetax );

	eyex = lx + centerx;
	eyey = ly + centery;
	eyez = lz + centerz;

	/*��XUp�۹�������m�b����*/
	lupx = ny * lz - ly * nz;
	lupy = nz * lx - lz * nx;
	lupx = nx * ly - lx * ny;
	upx = lupx + eyex;
	upy = lupy + eyey;
	upz = lupz + eyez;

	
}

double 
Camera::distanceToObject() {
	//���o�����쪫�餤�����V�q
	double lx = eyex - centerx;
	double ly = eyey - centery;
	double lz = eyez - centerz;

	return sqrt (  lx*lx + ly*ly + lz*lz );
}