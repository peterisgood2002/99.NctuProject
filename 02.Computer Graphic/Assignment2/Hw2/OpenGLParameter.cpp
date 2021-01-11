/******************************************
FILE
    OpenGLParameter.cpp
PURPOSE
    北OpenGL 闽ㄧΑ﹚竡 OpenGLcall back function单单

NOTE
	

AUTHOR
    9757553 肅в言
*******************************************/

#include "OpenGLParameter.h"
#include <iostream>


/*Constructor*/
OpenGLParameter::OpenGLParameter() {
	setGLWindowWidth(500);
	setGLWindowHeight(500);
	setGLWindowX(100);
	setGLWindowY(100);
	
	setCameraPosition(1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1); 
	setClipPlane(45 ,0.1 ,1000);

	setLightPostion( 0 ,0 ,1 ,1 );
	setLightSpecular( 1 ,1 ,1 ,1);
	setLightDiffuse( 1, 1, 1, 1);
	setLightAmbient( 1, 1, 1, 1);
}

OpenGLParameter::OpenGLParameter(int w, int h, int x, int y) {
	setGLWindowWidth(w);
	setGLWindowHeight(h);
	setGLWindowX(x);
	setGLWindowY(y);

	setCameraPosition(1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1); 
	setClipPlane(45 ,0.1 ,1000);

	setLightPostion( 0 ,0 ,1 ,1 );
	setLightSpecular( 1 ,1 ,1 ,1);
	setLightDiffuse( 1, 1, 1, 1);
	setLightAmbient( 1, 1, 1, 1);

}

/*Destructor*/
OpenGLParameter::~OpenGLParameter() {
	
}
/*set data member*/
void 
OpenGLParameter::setGLWindowHeight(int height) {
	opwheight = height;
}
void
OpenGLParameter::setGLWindowWidth(int width) {
	opwwidth = width;
}
void 
OpenGLParameter::setGLWindowX(int x) {
	opwx = x;
}
void 
OpenGLParameter::setGLWindowY(int y) {
	opwy = y;
}
void 
OpenGLParameter::setClipPlane(double fv ,double n ,double f) {
	fov =fv;
	near = n;
	far = f;
}
void 
OpenGLParameter::setCameraPosition(double ex ,double ey ,double ez ,double cx ,double cy ,double cz ,double ux ,double uy ,double uz) {
	camera.eyex = ex;
	camera.eyey = ey;
	camera.eyez = ez;
	camera.centerx = cx;
	camera.centery = cy;
	camera.centerz = cz;
	camera.upx = ux;
	camera.upy = uy;
	camera.upz = uz;
}
/*砞﹚light闽把计*/
void 
OpenGLParameter::setLightPostion( float x ,float y ,float z ,float r) {
	
	light.lightp[0] = x;
	light.lightp[1] = y;
	light.lightp[2] = z;
	light.lightp[3] = r;
	
}
void
OpenGLParameter::setLightSpecular(float r ,float g ,float b ,float a) {
	light.lspecular[0] = r;
	light.lspecular[1] = g;
	light.lspecular[2] = b;
	light.lspecular[3] = a;
}
void
OpenGLParameter::setLightDiffuse(float r ,float g ,float b ,float a) {
	light.ldiffuse[0] = r;
	light.ldiffuse[1] = g;
	light.ldiffuse[2] = b;
	light.ldiffuse[3] = a;
}
void
OpenGLParameter::setLightAmbient(float r ,float g ,float b ,float a) {
	light.lambient[0] = r;
	light.lambient[1] = g;
	light.lambient[2] = b;
	light.lambient[3] = a;
}
/*Get data member*/
int 
OpenGLParameter::getGLWindowHeight() const{
	return opwheight;
}
int 
OpenGLParameter::getGLWindowWidth() const{
	return opwwidth;
}
int 
OpenGLParameter::getGLWindowX() const{
	return opwx;
}
int 
OpenGLParameter::getGLWindowY() const{
	return opwy;
}
Camera
OpenGLParameter::getCameraPosition() const{
	return camera;
}
double 
OpenGLParameter::getFOV() const {
	return fov;
}
double 
OpenGLParameter::getNearPlane() const {
	return near;
}
double 
OpenGLParameter::getFarPlane() const {
	return far;
}
const Light&
OpenGLParameter::getLight() const{
	return light;
}

/*
  move the camera by (deltaX ,deltaY ,deltaZ)
  肚deltaX deltaY deltaZ
*/
void 
OpenGLParameter::mCameraByDelta(int deltaX ,int deltaY ,int deltaZ ) {

	double tmp[4] = { camera.eyex ,camera.eyey ,camera.eyez ,1 };
	double* ceye = camera.W2C( tmp);
	ceye[0] += deltaX;
	ceye[1] += deltaY;
	ceye[2] += deltaZ;

	double* eye = camera.C2W( ceye );
	camera.eyex = eye[0];
	camera.eyey = eye[1];
	camera.eyez = eye[2];

	delete ceye;
	delete eye;

	
}
/*
  move the light by (deltaX deltaY deltaZ)
  肚delatX deltaY deltaZ
*/
void 
OpenGLParameter::mLightByDelta(int deltaX ,int deltaY ,int deltaZ ) {
	double tmp[4] = { 0 };
	for ( int i = 0; i < 4; i++)
		tmp[i] = light.lightp[i];

	double* lightp = camera.W2C ( tmp );
	lightp[0] += deltaX;
	lightp[1] += deltaY;
	lightp[2] += deltaZ;

	double* ltmp = camera.C2W( lightp);
	for ( int i = 0; i < 4; i++ )
		light.lightp[i] = (float)ltmp[i];

	delete lightp;
	delete ltmp;
}

