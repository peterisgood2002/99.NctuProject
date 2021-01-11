/********************************************
Purpose
	眼睛類別的定義

Author
    9757553 顏志晟
*********************************************/
#include "Camera.h"
#include "math.h"

/*default constructor
  其設定是以世界座標系進行設定
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
//設定眼睛相關參數
/**
 *設定眼睛看出去的向量
 */
void 
Camera::setCamera(double ex ,double ey ,double ez) {
	eyex = ex;
	eyey = ey;
	eyez = ez;
}
/**
 *設定眼睛中心位置在哪裡
*/
void 
Camera::setCenter(double cx ,double cy ,double cz) {
	centerx = cx;
	centery = cy;
	centerz = cz;
}
/**
 *眼睛朝上的位置為何
*/
void 
Camera::setUp(double ux ,double uy ,double uz) {
	upx = ux;
	upy = uy;
	upz = uz;
}
//得到眼睛相關參數
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
/**設定眼睛zoom in 對眼睛的方向做zoom in
  *傳入：要zoom in多少距離
*/
void 
Camera::zoomIn(double dist) {
	
	eyez -=dist;
}
/**設定眼睛zoom out
  *傳入：要zoom out 多少距離
*/
void 
Camera::zoomOut(double dist) {
	eyez += dist;
}
/**向左
  *傳入：要向左多少距離
  */
void 
Camera::moveLeft(double dist) {
	centerx -= dist;
}
/**向右
  *傳入：向右多少距離
  */
void 
Camera::moveRight(double dist) {
	centerx +=dist;
}
/**Camera隨著物體中心旋轉
	  *傳入：水平旋轉變化量 垂直旋轉變化量
	  */
void 
Camera::rotate(double rx ,double ry) {
	double PI = 3.1415926;
	//取得眼睛到物體中間的向量
	double lx = eyex - centerx;
	double ly = eyey - centery;
	double lz = eyez - centerz;

	/*找出up和眼睛所看到方向上的平面以方便我們以後找Up用*/
	double lupx = upx - eyex;
	double lupy = upy - eyey;
	double lupz = upz - eyez;
	double nx = lupy * lz - lupz * ly;
	double ny = lupz * lx - lz * lupx;
	double nz = lupx * ly - lx * lupy;
	/*找完平面*/
	/*計算新的眼睛位置*/
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

	/*找出Up相對應的位置在哪裡*/
	lupx = ny * lz - ly * nz;
	lupy = nz * lx - lz * nx;
	lupx = nx * ly - lx * ny;
	upx = lupx + eyex;
	upy = lupy + eyey;
	upz = lupz + eyez;

	
}

double 
Camera::distanceToObject() {
	//取得眼睛到物體中間的向量
	double lx = eyex - centerx;
	double ly = eyey - centery;
	double lz = eyez - centerz;

	return sqrt (  lx*lx + ly*ly + lz*lz );
}