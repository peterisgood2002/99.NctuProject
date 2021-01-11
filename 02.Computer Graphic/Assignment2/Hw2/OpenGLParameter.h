/******************************************
FILE
    OpenGLParameter.h
PURPOSE
    處理OpenGL 相關眾多參數

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _OPENGLPARAMETER_
#define _OPENGLPARAMETER_

#include "Struct.h"

class OpenGLParameter {
private:
	//OpenGL產生的視窗大小
	int opwwidth;
	int opwheight;
	//OpenGL位置
	int opwx;
	int opwy;
	Camera camera;
	Light light;
	double fov;//Field of view
	double near;//最接近的plane
	double far;//最遠的plane

public:
	/*
	 Constructor
	 */
	OpenGLParameter();
	OpenGLParameter(int width , int height ,int x ,int y);
	/*
	 Destructor
	*/
	~OpenGLParameter();
	/*set data membet*/
	void setGLWindowWidth(int width);
	void setGLWindowHeight(int height);
	void setGLWindowX(int x );
	void setGLWindowY(int y);
	void setCameraPosition(double ex ,double ey ,double ez ,double cx ,double cy ,double cz ,double ux ,double uy ,double uz);
    void setClipPlane(double fv ,double n ,double f);
	void setLightPostion(float x ,float y ,float z ,float r );
	void setLightSpecular(float r ,float g ,float b ,float a);
	void setLightDiffuse(float r ,float g ,float b ,float a);
	void setLightAmbient(float r ,float g ,float b ,float a);
	/*
	get data member
	*/
	int getGLWindowWidth() const;
	int getGLWindowHeight() const;
	int getGLWindowX() const;
	int getGLWindowY() const;
	Camera getCameraPosition() const;
	double getFOV() const;
	double getNearPlane() const;
	double getFarPlane() const;
	const Light& getLight() const;
	/*
	 move the camera by (deltaX ,deltaY ,deltaZ)
	 傳入：deltaX deltaY deltaZ
	*/
	void mCameraByDelta(int deltaX ,int deltaY ,int deltaZ );
	/*
	 move the light by (deltaX deltaY deltaZ)
	 傳入：delatX deltaY deltaZ
	*/
	void mLightByDelta(int deltaX ,int deltaY ,int deltaZ );


};

#endif